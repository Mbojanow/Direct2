#include "Plane.h"

Plane::Plane()
{
    xoyPosition = std::make_pair<double, double>(0, 0);
    altitude = 0;
}

std::pair<double, double> Plane::getXOYPosition() const
{
    return xoyPosition;
}

unsigned Plane::getAltitude() const
{
    return altitude;
}

Waypoint Plane::getPosition() const
{
    return Waypoint(xoyPosition, altitude);
}

void Plane::setXOYPosition(const std::pair<double, double> &newXOYPosition)
{
    xoyPosition = newXOYPosition;
}

void Plane::setAltitude(unsigned altitude)
{
    this->altitude = altitude;
}

void Plane::setPosition(const Waypoint &waypoint)
{
    setXOYPosition(waypoint.getXOYPosition());
    setAltitude(waypoint.getAltitude());
}

