#pragma once

#include "plane/PlaneBoard.h"
#include "SimulationSpeed.h"
#include "SimulationState.h"

#include <memory>
#include <thread>
#include <mutex>

class RouteSimulation
{
private:
    std::shared_ptr<PlaneBoard> planeBoard;
    std::shared_ptr<SimulationState> simulationState;

    bool finishCleaning;
    std::shared_ptr<std::thread> simulationThread;
    std::shared_ptr<std::thread> simulationThreadJoiner;
    std::shared_ptr<std::mutex> mtx;

public:
    RouteSimulation();
    ~RouteSimulation();

    WaypointsDequePtr generateFlightPlan();
    void start();
    void pause();
    void unpause();
    void reset();
    void changeSpeed(const SimulationSpeed &simulationSpeed);

    std::pair<WaypointsDequePtr, bool> generateFlightPlanAlternative();
    void acceptFlightPlanAlternative();
    void rejectFlightPlanAlternative();
    int getElapsedTime() const;

private:
    void run();
    void initRouteWaypoints();
    void initPlanePosition();
    void initVisitedPoints();
    void setNextWaypointRoutePoints(const Waypoint *const fromPosition);
    void executeStep();
    void sleepTillNextStep() const;
    Waypoint *getLastVisitedWaypoint() const;

    void cleanSimulationThreads();

    // TODO: add sigabort handler to terminate thread gracefully
};
