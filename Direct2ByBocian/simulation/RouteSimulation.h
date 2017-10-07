#pragma once

#include "plane/PlaneBoard.h"
#include "SimulationSpeed.h"
#include "SimulationState.h"

#include <memory>

class RouteSimulation
{
private:
    std::shared_ptr<PlaneBoard> planeBoard;
    std::unique_ptr<SimulationState> simulationState;

public:
    RouteSimulation();

    void init();
    void start();
    void run();
    void pause();
    void unpause();
    void reset();
    void generateFlightPlan();
    void changeSpeed(const SimulationSpeed &simulationSpeed);

    void generateFlightPlanAlternative();
    void acceptFlightPlanAlternative();
    void rejectFlightPlanAlternative();

private:
    void initRouteWaypoints();
    void initPlanePosition();
    void initVisitedPoints();
    void setNextWaypointRoutePoints(const Waypoint *const fromPosition);
    void executeStep();

    void sleepTillNextStep() const;
    Waypoint *getLastVisitedWaypoint() const;
};
