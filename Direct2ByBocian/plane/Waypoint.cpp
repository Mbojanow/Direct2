#include "Waypoint.h"
#include <cmath>

// not sure why but abs from cmath does not work well with mingw 5 on win10
double getAbsValue(double value)
{
    return value > 0.0 ? value : -value;
}

Waypoint::Waypoint()
{
    xoyPosition = std::make_pair(0, 0);
    altitude = 0;
    mandatory = DEFAULT_MANDATORY_POLICY;
}

Waypoint::Waypoint(const std::pair<double, double> &xoyPosition, unsigned altitude)
{
    this->xoyPosition = xoyPosition;
    this->altitude = altitude;
    this->mandatory = DEFAULT_MANDATORY_POLICY;
}

std::pair<double, double> Waypoint::getXOYPosition() const
{
    return xoyPosition;
}

unsigned Waypoint::getAltitude() const
{
    return altitude;
}

bool Waypoint::isMandatory() const
{
    return mandatory;
}

void Waypoint::setMandatory(bool mandatory)
{
    this->mandatory = mandatory;
}

double Waypoint::getAxisAbsDiff(const Axis &axisType, const Waypoint &pointA, const Waypoint &pointB)
{
    if (axisType == Axis::X)
    {
        return getAbsValue(pointA.getXOYPosition().first - pointB.getXOYPosition().first);
    }

    if (axisType == Axis::Y)
    {
        return getAbsValue(pointA.getXOYPosition().second - pointB.getXOYPosition().second);
    }

    if (axisType == Axis::Z)
    {
        return getAbsValue(pointA.getAltitude() - pointB.getAltitude());
    }

    throw std::runtime_error("Unsupported axis type. Cannot compute difference.");
}

double Waypoint::getSlopeAngle(const Waypoint &pointA, const Waypoint &pointB)
{
    double xAxisDiff = getAxisAbsDiff(Axis::X, pointA, pointB);
    double yAxisDiff = getAxisAbsDiff(Axis::Y, pointA, pointB);
    return atan(yAxisDiff / xAxisDiff);
}

double Waypoint::getDistanceBetween(const Waypoint &pointA, const Waypoint &pointB)
{
    double xAxisDiff = getAxisAbsDiff(Axis::X, pointA, pointB);
    double yAxisDiff = getAxisAbsDiff(Axis::Y, pointA, pointB);
    return sqrt(xAxisDiff * xAxisDiff + yAxisDiff * yAxisDiff);
}

std::ostream &operator<<(std::ostream &os, const Waypoint &obj)
{
    return os << "*** Waypoint ***\n"
              << "x = " << obj.xoyPosition.first
              << "\ny = " << obj.xoyPosition.second
              << "\naltitude = " << obj.altitude << "\n";
}
