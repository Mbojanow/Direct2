#include "PlaneBoard.h"


PlaneBoard::PlaneBoard()
{
    plane = std::make_shared<Plane>(Plane());
    waypointsReached = std::make_shared<std::deque<Waypoint>>(0);
    waypointsToReach = std::make_shared<std::deque<Waypoint>>(0);
    pointsVisited = std::make_shared<std::deque<Waypoint>>(0);
    pointsToNextWaypoint = std::make_shared<std::deque<Waypoint>>(0);
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

WaypointsDequePtr PlaneBoard::getReachedWaypoints() const
{
    return waypointsReached;
}

WaypointsDequePtr PlaneBoard::getToReachWaypoints() const
{
    return waypointsToReach;
}

WaypointsDequePtr PlaneBoard::getVisitedPoints() const
{
    return pointsVisited;
}

WaypointsDequePtr PlaneBoard::getNextWaypointPoints() const
{
    return pointsToNextWaypoint;
}
