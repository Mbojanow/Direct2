#pragma once

#include <utility>
#include <iostream>
#include "Axis.h"

class Waypoint
{
private:
    static const bool DEFAULT_MANDATORY_POLICY = true;

    std::pair<double, double> xoyPosition;
    unsigned altitude;
    bool mandatory;

public:
    Waypoint();
    Waypoint(const std::pair<double, double> &xoyPosition, unsigned altitude);

    std::pair<double, double> getXOYPosition() const;
    unsigned getAltitude() const;
    bool isMandatory() const;

    void setMandatory(bool mandatory);

    static double getSlopeAngle(const Waypoint &pointA, const Waypoint &pointB);
    static double getDistanceBetween(const Waypoint &pointA, const Waypoint &pointB);
    static double getAxisAbsDiff(const Axis &axisType, const Waypoint &pointA, const Waypoint &pointB);

    friend std::ostream &operator<<(std::ostream &os, const Waypoint &obj);
};
