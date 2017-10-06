#pragma once

#include "plane/Waypoint.h"

class SimulationWaypointGenerator
{
private:
    static Waypoint generateGenericWaypoint(unsigned waypointIndex);
    static Waypoint generateDepartureWaypoint();
    static Waypoint generateTopOfClimbWaypoint();
    static Waypoint generateTopOfDescentWaypoint();
    static Waypoint generateArrivalWaypoint();

public:
    static Waypoint generate(unsigned waypointIndex);
};
