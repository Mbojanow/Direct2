#pragma once

#include <deque>
#include <memory>

#include "Waypoint.h"
#include "Plane.h"

typedef std::shared_ptr<std::deque<Waypoint>> WaypointsDequePtr;

class PlaneBoard
{
public:
    static const int BOARD_SIZE = 110;

private:
    PlaneBoard();
    std::shared_ptr<Plane> plane;
    WaypointsDequePtr waypointsReached;
    WaypointsDequePtr waypointsToReach;
    WaypointsDequePtr pointsVisited;
    WaypointsDequePtr pointsToNextWaypoint;

public:
    PlaneBoard(PlaneBoard const&) = delete;
    PlaneBoard(PlaneBoard&&) = delete;
    PlaneBoard& operator=(PlaneBoard const&) = delete;
    PlaneBoard& operator=(PlaneBoard &&) = delete;

    static std::shared_ptr<PlaneBoard> instance();

    std::shared_ptr<Plane> getPlane() const;
    std::shared_ptr<std::deque<Waypoint>> getReachedWaypoints() const;
    std::shared_ptr<std::deque<Waypoint>> getToReachWaypoints() const;
    std::shared_ptr<std::deque<Waypoint>> getVisitedPoints() const;
    std::shared_ptr<std::deque<Waypoint>> getNextWaypointPoints() const;
};
