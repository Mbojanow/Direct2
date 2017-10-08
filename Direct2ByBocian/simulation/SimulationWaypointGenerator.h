#pragma once

#include "plane/Waypoint.h"

/*
 * SimulationWaypointGenerator class declaration.
 * SimulationWaypointGenerator is a utility to generate n-th waypoint for simulation
 * considering all simulation assumptions.
 */
class SimulationWaypointGenerator
{
private:
    static Waypoint generateGeneric(unsigned waypointIndex);
    static Waypoint generateDeparture();
    static Waypoint generateTopOfClimb();
    static Waypoint generateTopOfDescent();
    static Waypoint generateArrival();

public:
    static Waypoint generate(unsigned waypointIndex);
};
