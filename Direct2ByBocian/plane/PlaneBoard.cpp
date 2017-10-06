#include "PlaneBoard.h"


PlaneBoard::PlaneBoard()
{
    plane = std::make_shared<Plane>(Plane());
    waypointsReached = std::make_shared<std::vector<Waypoint>>(0);
    waypointsToReach = std::make_shared<std::vector<Waypoint>>(0);
    pointsVisited = std::make_shared<std::vector<Waypoint>>(0);
    pointsToNextWaypoint = std::make_shared<std::vector<Waypoint>>(0);
}

std::shared_ptr<PlaneBoard> PlaneBoard::instance()
{
    static std::shared_ptr<PlaneBoard> planeBoard = std::shared_ptr<PlaneBoard>(new PlaneBoard());
    return planeBoard;
}

std::shared_ptr<Plane> PlaneBoard::getPlane() const
{
    return plane;
}

std::shared_ptr<std::vector<Waypoint>> PlaneBoard::getReachedWaypoints() const
{
    return waypointsReached;
}

std::shared_ptr<std::vector<Waypoint>> PlaneBoard::getToReachWaypoints() const
{
    return waypointsToReach;
}

std::shared_ptr<std::vector<Waypoint>> PlaneBoard::getVisitedPoints() const
{
    return pointsVisited;
}

std::shared_ptr<std::vector<Waypoint>> PlaneBoard::getNextWaypointPoints() const
{
    return pointsToNextWaypoint;
}
