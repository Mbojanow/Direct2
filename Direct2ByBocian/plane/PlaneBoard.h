#pragma once

#include <deque>
#include <memory>

#include "Waypoint.h"
#include "Plane.h"


typedef std::shared_ptr<std::deque<Waypoint>> WaypointsDequePtr;

/*
 * PlaneBoard class declaration. PlaneBoard is a singleton.
 * Board contains:
 *  - plane position
 *  - waypoints that need to be reached
 *  - waypoints that have already been reached by the plane
 *  - alternative waypoints to reach in case alternative route appears
 *  - points visited by plane (in each simulation unit)
 *  - points to next waypoint (route for each simulation unit till next endpoint is reached by the plane)
 */
class PlaneBoard
{
public:
    static const int BOARD_SIZE = 110;

private:
    PlaneBoard();
    std::shared_ptr<Plane> plane;
    WaypointsDequePtr waypointsReached;
    WaypointsDequePtr waypointsToReach;
    WaypointsDequePtr alternativeWaypointsToReach;
    WaypointsDequePtr pointsVisited;
    WaypointsDequePtr pointsToNextWaypoint;

public:
    PlaneBoard(PlaneBoard const&) = delete;
    PlaneBoard(PlaneBoard&&) = delete;
    PlaneBoard& operator=(PlaneBoard const&) = delete;
    PlaneBoard& operator=(PlaneBoard &&) = delete;

    static std::shared_ptr<PlaneBoard> instance();

    std::shared_ptr<Plane> getPlane() const;
    WaypointsDequePtr getReachedWaypoints() const;
    WaypointsDequePtr getToReachWaypoints() const;
    WaypointsDequePtr getVisitedPoints() const;
    WaypointsDequePtr getNextWaypointPoints() const;
    WaypointsDequePtr getAlternativeWaypointsToReach() const;
};
