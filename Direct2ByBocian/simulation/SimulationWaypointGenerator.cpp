#include "SimulationWaypointGenerator.h"
#include "SimulationConstants.h"
#include "plane/PlaneBoard.h"

#include <random>

Waypoint SimulationWaypointGenerator::generateDeparture()
{
    return Waypoint(std::make_pair(DEPARTURE_X,
                                   DEPARTURE_Y),
                    DEPARTURE_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateTopOfClimb()
{
    return Waypoint(std::make_pair(TOP_OF_CLIMB_X,
                                   TOP_OF_CLIMB_Y),
                    TOP_OF_CLIMB_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateTopOfDescent()
{
    return Waypoint(std::make_pair(TOP_OF_DESCENT_X,
                                   TOP_OF_DESCENT_Y),
                    TOP_OF_DESCENT_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateArrival()
{
    return Waypoint(std::make_pair(ARRIVAL_X,
                                   ARRIVAL_Y),
                    ARRIVAL_ALTITUDE);
}

Waypoint SimulationWaypointGenerator::generateGeneric(unsigned waypointIndex)
{
    // TODO: seems like mingw has an issue with std::random_device and it's not that random
    // check if can do seeding somehow.
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
            return generateDeparture();
            break;
        case TOP_OF_CLIMB_WAYPOINT_INDEX:
            return generateTopOfClimb();
            break;
        case TOP_OF_DESCENT_WAYPOINT_INDEX:
            return generateTopOfDescent();
            break;
        case ARRIVAL_WAPOINT_INDEX:
            return generateArrival();
            break;
        default:
            return generateGeneric(waypointIndex);;
    }
}
