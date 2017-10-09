#pragma once

#include <QPointF>
#include <utility>

#include "Waypoint.h"

/*
 * WaypointConverter class declaration. Utility to get other type of point from Waypoint class.
 */

class WaypointConverter
{
public:
    static QPointF toQPointF(const Waypoint &waypoint, unsigned scale = 1);
};
