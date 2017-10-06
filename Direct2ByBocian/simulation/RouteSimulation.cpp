#include "RouteSimulation.h"
#include "SimulationWaypointGenerator.h"
#include "SimulationConstants.h"

#include <cmath>
#include <iostream>
#include <algorithm>

#define LOG std::cout << "\tLOG: "

RouteSimulation::RouteSimulation()
{
    planeBoard = PlaneBoard::instance();
    simulationSpeed = SimulationSpeed::NORMAL;
    isActive = false;
    isFinished = false;
}

void RouteSimulation::init()
{
    LOG << "Initializing simulation.\n" << std::flush;
    isActive = false;
    isFinished = false;
    initRouteWaypoints();
    initPlanePosition();
    initVisitedPoints();
    setNextWaypointRoutePoints(getLastVisitedWaypoint());

    LOG << "Simulation initialization finished.\n" << std::flush;
}

void RouteSimulation::start()
{
    LOG << "Starting simulation\n" << std::flush;
    isActive = true;
    init();
}

//
// private methods
//
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

    const Waypoint *previousPoint = fromPosition;

    for (int pointIndex = 0; pointIndex < numPointsToGenerate; pointIndex++)
    {
        double xRoutePointPos = MOVE_DIST_PER_SIMULATION_UNIT * cos(moveDirectionAngle)
                + previousPoint->getXOYPosition().first;
        double yRoutePointPos = MOVE_DIST_PER_SIMULATION_UNIT * sin(moveDirectionAngle)
                + previousPoint->getXOYPosition().second;
        double altitudePointPos = previousPoint->getAltitude() + interpolatedAltitudeDiffBetweenPoints;

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
    planeBoard->getVisitedPoints()->push_back(planeBoard->getPlane()->getPosition());

    Waypoint *nextRoutePoint = &planeBoard->getNextWaypointPoints()->at(0);
    planeBoard->getPlane()->setPosition(*nextRoutePoint);
    LOG << "Plane moved to:\n" << *nextRoutePoint << std::endl << std::flush;

    planeBoard->getNextWaypointPoints()->erase(planeBoard->getNextWaypointPoints()->begin());

    // reached last point just next to next waypoint -> snap plane to exact waypoint position
    if (planeBoard->getNextWaypointPoints()->empty())
    {
        Waypoint *currentWaypoint = &planeBoard->getToReachWaypoints()->at(0);

        planeBoard->getVisitedPoints()->push_back(planeBoard->getPlane()->getPosition());
        planeBoard->getPlane()->setPosition(*currentWaypoint);

        planeBoard->getReachedWaypoints()->push_back(planeBoard->getToReachWaypoints()->at(0));
        planeBoard->getToReachWaypoints()->erase(planeBoard->getToReachWaypoints()->begin());

        LOG << "Waypoint reached! Plane moved to:\n" << planeBoard->getPlane()->getPosition()
            << std::endl << std::flush;

        if (planeBoard->getToReachWaypoints()->empty())
        {
            isFinished = true;
            isActive = false;
            LOG << "Plane landed successfully! Destination reached!\n" << std::flush;
            // TODO: signal that simulation ended;
        }
        else
        {
            setNextWaypointRoutePoints(getLastVisitedWaypoint());
        }
    }

}

/*
void start();
void pause();
void reset();
void generateFlightPlan();
void changeSpeed(const SimulationSpeed &simulationSpeed);

void generateFlightPlanAlternative();
void acceptFlightPlanAlternative();
void rejectFlightPlanAlternative();*/
