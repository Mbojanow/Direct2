#include "SimulationWaypointGenerator.h"
#include "SimulationConstants.h"
#include "plane/PlaneBoard.h"

#include <random>

Waypoint SimulationWaypointGenerator::generateDepartureWaypoint()
{
    return Waypoint(std::make_pair(DEPARTURE_X,
                                   DEPARTURE_Y),
                    DEPARTURE_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateTopOfClimbWaypoint()
{
    return Waypoint(std::make_pair(TOP_OF_CLIMB_X,
                                   TOP_OF_CLIMB_Y),
                    TOP_OF_CLIMB_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateTopOfDescentWaypoint()
{
    return Waypoint(std::make_pair(TOP_OF_DESCENT_X,
                                   TOP_OF_DESCENT_Y),
                    TOP_OF_DESCENT_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateArrivalWaypoint()
{
    return Waypoint(std::make_pair(ARRIVAL_X,
                                   ARRIVAL_Y),
                    ARRIVAL_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateGenericWaypoint(unsigned waypointIndex)
{
    static std::random_device randomDevice;
    static std::mt19937 randomGenerator(randomDevice());
    static std::uniform_real_distribution<> altitudeDistributionRange(
                ALTITUDE_LOWER_BOUND_DIFF,
                ALTITUDE_UPPER_BOUND_DIFF);
    static std::uniform_real_distribution<> axisDistributionRange(
                AXIS_LOWER_BOUND_DIFF,
                AXIS_UPPER_BOUND_DIFF);


    double xAxisValue = X_AXIS_MULTIPLIER * waypointIndex;
    double yAxisValue = xAxisValue + axisDistributionRange(randomGenerator);
    double altitude = WAYPOINT_ALTITUDE_BASE + altitudeDistributionRange(randomGenerator);

    return Waypoint(std::make_pair(xAxisValue, yAxisValue), altitude);
}

Waypoint SimulationWaypointGenerator::generate(unsigned waypointIndex)
{
    if (waypointIndex > MAX_WAYPOINT_INDEX)
    {
        throw std::runtime_error("Unexpected waypoint index provided to generator!");
    }

    switch (waypointIndex)
    {
        case DEPARTURE_WAYPOINT_INDEX:
            return generateDepartureWaypoint();
            break;
        case TOP_OF_CLIMB_WAYPOINT_INDEX:
            return generateTopOfClimbWaypoint();
            break;
        case TOP_OF_DESCENT_WAYPOINT_INDEX:
            return generateTopOfDescentWaypoint();
            break;
        case ARRIVAL_WAPOINT_INDEX:
            return generateArrivalWaypoint();
            break;
        default:
            return generateGenericWaypoint(waypointIndex);;
    }
}
