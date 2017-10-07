#pragma once

#include <utility>
#include <iostream>
#include <string>
#include "Axis.h"

class Waypoint
{
private:
    static const bool DEFAULT_MANDATORY_POLICY = true;
    static const int LABEL_LEN = 3;

    std::pair<double, double> xoyPosition;
    unsigned altitude;
    bool mandatory;
    std::string label;

public:
    Waypoint();
    Waypoint(const std::pair<double, double> &xoyPosition, unsigned altitude);

    std::pair<double, double> getXOYPosition() const;
    unsigned getAltitude() const;
    bool isMandatory() const;
    std::string getLabel() const;

    void setMandatory(bool mandatory);

    static double getSlopeAngle(const Waypoint &pointA, const Waypoint &pointB);
    static double getDistanceBetween(const Waypoint &pointA, const Waypoint &pointB);
    static double getAxisAbsDiff(const Axis &axisType, const Waypoint &pointA, const Waypoint &pointB);

    friend std::ostream &operator<<(std::ostream &os, const Waypoint &obj);

private:
    static std::string generateLabel();
};
