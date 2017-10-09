#include "WaypointConverter.h"
#include <cmath>

QPointF WaypointConverter::toQPointF(const Waypoint &waypoint, unsigned scale)
{
    if (scale == 0)
    {
        throw std::runtime_error("[ERROR] Conversion scale cannot equal to 0");
    }
    return QPointF(waypoint.getXOYPosition().first * scale,
                   waypoint.getXOYPosition().second * scale);
}
