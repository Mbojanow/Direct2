#pragma once


#include <utility>
#include <iostream>
#include <string>

#include "Axis.h"

/*
 * Waypoint class declaration. Waypoint is a point in 3D space.
 * Waypoint contains
 *  - information about XOY position as doubles
 *  - information about altitude (natural int)
 *  - LABEL_LEN long label
 */
class Waypoint
{
private:
    static const int LABEL_LEN = 3;

    std::pair<double, double> xoyPosition;
    unsigned altitude;
    std::string label;

public:
    Waypoint();
    Waypoint(const std::pair<double, double> &xoyPosition, unsigned altitude);

    std::pair<double, double> getXOYPosition() const;
    unsigned getAltitude() const;
    std::string getLabel() const;

    // Computes angle for y=ax+b where tg(angle)=a based on XOY position on point A and B;
    static double getSlopeAngle(const Waypoint &pointA, const Waypoint &pointB);
    static double getDistanceBetween(const Waypoint &pointA, const Waypoint &pointB);
    static double getAxisAbsDiff(const Axis &axisType, const Waypoint &pointA, const Waypoint &pointB);

    friend bool operator==(const Waypoint &lhs, const Waypoint &rhs);
    friend std::ostream &operator<<(std::ostream &os, const Waypoint &obj);

private:
    static std::string generateLabel();
};
