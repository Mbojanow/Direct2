#include "RouteSimulation.h"
#include "SimulationWaypointGenerator.h"
#include "SimulationConstants.h"
#include "SimulationInitalizationException.h"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <thread>

#define LOG std::cout << "LOG: "

RouteSimulation::RouteSimulation()
{
    planeBoard = PlaneBoard::instance();
    simulationState = std::make_shared<SimulationState>();
    finishCleaning = false;
    simulationThreadJoiner = std::make_shared<std::thread>(std::thread(&RouteSimulation::cleanSimulationThreads, this));
    mtx = std::make_shared<std::mutex>();
}

RouteSimulation::~RouteSimulation()
{
    finishCleaning = true;
    if (simulationThreadJoiner && simulationThreadJoiner->joinable())
    {
        simulationThreadJoiner->join();
    }
}

std::shared_ptr<PlaneBoard> RouteSimulation::getPlaneBoard() const
{
    return planeBoard;
}

WaypointsDequePtr RouteSimulation::generateFlightPlan()
{
    LOG << "Initializing simulation. Generating flight plan\n" << std::flush;
    simulationState->setFinishedState(false);
    simulationState->setPauseState(false);
    initRouteWaypoints();
    initPlanePosition();
    initVisitedPoints();
    LOG << "Simulation initialization finished.\n" << std::flush;
    return planeBoard->getToReachWaypoints();
}

void RouteSimulation::start()
{
    LOG << "Starting simulation\n" << std::flush;
    if (planeBoard->getToReachWaypoints()->empty())
    {
        throw SimulationInitializationException(
                    "Failed to start simulation because of incorrect initalization. No flight plan found!");
    }
    simulationThread = std::make_shared<std::thread>(std::thread(&RouteSimulation::run, this));
}

void RouteSimulation::pause()
{
    LOG << "Pausing simulation\n" << std::flush;
    simulationState->setPauseState(true);
}

void RouteSimulation::unpause()
{
    LOG << "Unpausing simulation\n" << std::flush;
    simulationState->setPauseState(false);
}

void RouteSimulation::reset()
{
    simulationState->setFinishedState(false);
    simulationState->setPauseState(false);
    simulationState->resetTimer();
    planeBoard->getNextWaypointPoints()->clear();
    planeBoard->getReachedWaypoints()->clear();
    planeBoard->getToReachWaypoints()->clear();
    planeBoard->getVisitedPoints()->clear();
    planeBoard->getAlternativeWaypointsToReach()->clear();
}

void RouteSimulation::changeSpeed(const SimulationSpeed &simulationSpeed)
{
    simulationState->setStepSleepTime(simulationSpeed);
}

std::pair<WaypointsDequePtr, bool> RouteSimulation::generateFlightPlanAlternative()
{
    LOG << "Generating flight plan alternative\n" << std::flush;
    planeBoard->getAlternativeWaypointsToReach()->clear();
    std::srand(std::chrono::system_clock::now().time_since_epoch().count());
    std::copy_if(planeBoard->getToReachWaypoints()->begin(),
              planeBoard->getToReachWaypoints()->end() - 1,
              std::back_inserter(*planeBoard->getAlternativeWaypointsToReach()),
                 [](Waypoint &)
    {
        const double notRemoveProbability = (std::rand() % 100 + 1.0) / 100.0;
        return notRemoveProbability > NON_MANDATORY_PROBABILITY;
    });
    // push last waypoint that should never be removed
    planeBoard->getAlternativeWaypointsToReach()->push_back(planeBoard->getToReachWaypoints()->at(planeBoard->getToReachWaypoints()->size() - 1));

    bool noWaypointsRemoved =
            (planeBoard->getToReachWaypoints()->size() == planeBoard->getAlternativeWaypointsToReach()->size());
    return std::make_pair(planeBoard->getAlternativeWaypointsToReach(), noWaypointsRemoved);
}

void RouteSimulation::acceptFlightPlanAlternative()
{
    if (planeBoard->getAlternativeWaypointsToReach()->empty())
    {
        throw std::runtime_error("Unable to accept alternative flight plan because there is none!");
    }

    // Tricky part. If during plan generation and accepting, plane reached waypoint we need to remove it from
    // the alternative points route
    std::lock_guard<std::mutex> lock(*mtx);
    /*int numWaypointsToRemove = 0;
    for (auto alternativeWaypoint : *planeBoard->getAlternativeWaypointsToReach())
    {
        if (std::find(planeBoard->getToReachWaypoints()->begin(),
                      planeBoard->getToReachWaypoints()->end(),
                      alternativeWaypoint) != planeBoard->getToReachWaypoints()->end())
        {
            numWaypointsToRemove++;
        }
        else
        {
            break;
        }
    }
    LOG << numWaypointsToRemove << " waypoints will be removed\n" << std::flush;

    for (int waypointToRemoveIndex = 0; waypointToRemoveIndex < numWaypointsToRemove; waypointToRemoveIndex++)
    {
        planeBoard->getAlternativeWaypointsToReach()->pop_front();
    }*/
    planeBoard->getToReachWaypoints()->clear();
    std::copy(planeBoard->getAlternativeWaypointsToReach()->begin(),
              planeBoard->getAlternativeWaypointsToReach()->end(),
              std::back_inserter(*planeBoard->getToReachWaypoints()));
    //planeBoard->getToReachWaypoints() = planeBoard->getAlternativeWaypointsToReach();

    Waypoint currentPlanePosition = planeBoard->getPlane()->getPosition();
    planeBoard->getNextWaypointPoints()->clear();
    setNextWaypointRoutePoints(&currentPlanePosition);
    LOG << "Alternative flight plan accepted.\n" << std::flush;
}

void RouteSimulation::rejectFlightPlanAlternative()
{
    planeBoard->getAlternativeWaypointsToReach()->clear();
    LOG << "Alternative flight plan rejected.\n" << std::flush;
}

int RouteSimulation::getElapsedTime() const
{
    return simulationState->getExecutionTime();
}

bool RouteSimulation::getFinishedState() const
{
    return simulationState->isFinished();
}

//
// private methods
//
void RouteSimulation::run()
{
    planeBoard->getAlternativeWaypointsToReach()->clear();
    setNextWaypointRoutePoints(getLastVisitedWaypoint());
    while (!simulationState->isFinished())
    {
        executeStep();
        sleepTillNextStep();
        simulationState->incrementExecutionTime();
        while(simulationState->isPaused())
        {
            // Sleep for short amount of time to lower cpu work
            // while simulation is paused.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void RouteSimulation::initRouteWaypoints()
{
    planeBoard->getReachedWaypoints()->clear();
    planeBoard->getReachedWaypoints()->push_back(
                SimulationWaypointGenerator::generate(DEPARTURE_WAYPOINT_INDEX));

    planeBoard->getToReachWaypoints()->clear();
    for (unsigned waypointIndex = 1; waypointIndex <= MAX_WAYPOINT_INDEX; waypointIndex++)
    {
        planeBoard->getToReachWaypoints()->push_back(
                    SimulationWaypointGenerator::generate(waypointIndex));
    }

    LOG << "Waypoints to reach initialized:" << std::flush;
    for (auto &waypoint : *(planeBoard->getToReachWaypoints()))
    {
        LOG << waypoint << std::endl << std::flush;
    };
}

void RouteSimulation::initPlanePosition()
{
    planeBoard->getPlane()->setXOYPosition(std::make_pair(DEPARTURE_X, DEPARTURE_Y));
    planeBoard->getPlane()->setAltitude(DEPARTURE_ALTITUDE);
}

void RouteSimulation::initVisitedPoints()
{
    planeBoard->getVisitedPoints()->clear();
    planeBoard->getVisitedPoints()->push_back(SimulationWaypointGenerator::generate(DEPARTURE_WAYPOINT_INDEX));
}

void RouteSimulation::setNextWaypointRoutePoints(const Waypoint *const fromPosition)
{
    Waypoint *nextWaypoint = &planeBoard->getToReachWaypoints()->at(0);

    double moveDirectionAngle = Waypoint::getSlopeAngle(*nextWaypoint, *fromPosition);
    LOG << "Plane will now move at an angle of " << moveDirectionAngle << " radians\n"
        << "calculated based on " << *nextWaypoint << " and " << *fromPosition << std::flush;

    double waypointsBetweenDistance = Waypoint::getDistanceBetween(*nextWaypoint, *fromPosition);
    int numPointsToGenerate = ceil(waypointsBetweenDistance) - WAYPOINT_REACHED_DIFF;

    double waypointAltitudeDiff = Waypoint::getAxisAbsDiff(Axis::Z, *fromPosition, *nextWaypoint);
    double interpolatedAltitudeDiffBetweenPoints = waypointAltitudeDiff / numPointsToGenerate;
    if (nextWaypoint->getAltitude() < fromPosition->getAltitude())
    {
        interpolatedAltitudeDiffBetweenPoints = -interpolatedAltitudeDiffBetweenPoints;
    }

    const Waypoint *previousPoint = fromPosition;

    for (int pointIndex = 0; pointIndex < numPointsToGenerate; pointIndex++)
    {
        double xRoutePointPos = MOVE_DIST_PER_SIMULATION_UNIT * cos(moveDirectionAngle)
                + previousPoint->getXOYPosition().first;
        double yRoutePointPos = MOVE_DIST_PER_SIMULATION_UNIT * sin(moveDirectionAngle)
                + previousPoint->getXOYPosition().second;
        unsigned altitudePointPos = previousPoint->getAltitude() + interpolatedAltitudeDiffBetweenPoints;

        // fix interpolation rounding errors -> this is because it was decided to use naturals as altitude
        if (altitudePointPos > fromPosition->getAltitude() && altitudePointPos > nextWaypoint->getAltitude())
        {
            altitudePointPos = 0;
        }

        planeBoard->getNextWaypointPoints()->push_back(
                    Waypoint(std::make_pair(xRoutePointPos, yRoutePointPos), altitudePointPos));
        previousPoint = &planeBoard->getNextWaypointPoints()->at(planeBoard->getNextWaypointPoints()->size() - 1);
    }

    LOG << "In order to reach\n" << *nextWaypoint << " in distance of " << waypointsBetweenDistance
        << " plane must move past " << planeBoard->getNextWaypointPoints()->size() << " following points:\n";
    for (auto &routePoint : *(planeBoard->getNextWaypointPoints()))
    {
        LOG << "Point to reach:\n" << routePoint << std::endl << std::flush;
    }
}

Waypoint *RouteSimulation::getLastVisitedWaypoint() const
{
    if (planeBoard->getReachedWaypoints()->empty())
    {
        throw std::runtime_error("No visited endpoints found!");
    }
    return &planeBoard->getReachedWaypoints()->at(planeBoard->getReachedWaypoints()->size() - 1);
}

void RouteSimulation::executeStep()
{
    std::lock_guard<std::mutex> lock(*mtx);
    planeBoard->getVisitedPoints()->push_back(planeBoard->getPlane()->getPosition());

    Waypoint *nextRoutePoint = &planeBoard->getNextWaypointPoints()->at(0);
    planeBoard->getPlane()->setPosition(*nextRoutePoint);
    LOG << "Plane moved to:\n" << *nextRoutePoint << std::endl << std::flush;

    planeBoard->getNextWaypointPoints()->pop_front();

    // reached last point just next to next waypoint -> snap plane to exact waypoint position
    if (planeBoard->getNextWaypointPoints()->empty())
    {
        Waypoint *currentWaypoint = &planeBoard->getToReachWaypoints()->at(0);

        planeBoard->getVisitedPoints()->push_back(planeBoard->getPlane()->getPosition());
        planeBoard->getPlane()->setPosition(*currentWaypoint);

        planeBoard->getReachedWaypoints()->push_back(planeBoard->getToReachWaypoints()->at(0));
        planeBoard->getToReachWaypoints()->pop_front();

        LOG << "Waypoint reached! Plane moved to:\n" << planeBoard->getPlane()->getPosition()
            << std::endl << std::flush;

        if (planeBoard->getToReachWaypoints()->empty())
        {
            simulationState->setFinishedState(true);
            simulationState->resetTimer();
            LOG << "Plane landed successfully! Destination reached!\n" << std::flush;
        }
        else
        {
            setNextWaypointRoutePoints(getLastVisitedWaypoint());
        }
    }
}

void RouteSimulation::sleepTillNextStep() const
{
    std::this_thread::sleep_for(std::chrono::milliseconds(simulationState->getStepSleepTime()));
}

void RouteSimulation::cleanSimulationThreads()
{
    while (!finishCleaning)
    {
        if (simulationThread && simulationThread->joinable())
        {
            simulationThread->join();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
