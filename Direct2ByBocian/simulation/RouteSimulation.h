#pragma once

#include "plane/PlaneBoard.h"
#include "SimulationSpeed.h"

#include <memory>

class RouteSimulation
{
private:
    std::shared_ptr<PlaneBoard> planeBoard;

    SimulationSpeed simulationSpeed;
    bool isActive;
    bool isFinished;

public:
    RouteSimulation();

    void init();
    void start();
    void pause();
    void reset();
    void generateFlightPlan();
    void changeSpeed(const SimulationSpeed &simulationSpeed);

    void generateFlightPlanAlternative();
    void acceptFlightPlanAlternative();
    void rejectFlightPlanAlternative();
    void executeStep();
private:
    void initRouteWaypoints();
    void initPlanePosition();
    void initVisitedPoints();
    void setNextWaypointRoutePoints(const Waypoint *const fromPosition);

    Waypoint *getLastVisitedWaypoint() const;
};
