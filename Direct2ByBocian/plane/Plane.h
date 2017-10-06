#pragma once

#include <utility>
#include "Waypoint.h"

class Plane
{
private:
    std::pair<double, double> xoyPosition;
    unsigned altitude;

public:
    Plane();

    std::pair<double, double> getXOYPosition() const;
    unsigned getAltitude() const;
    Waypoint getPosition() const;

    void setXOYPosition(const std::pair<double, double> &newXOYPosition);
    void setAltitude(unsigned altitude);
    void setPosition(const Waypoint &waypoint);
};
