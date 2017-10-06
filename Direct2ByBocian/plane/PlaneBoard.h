#pragma once

#include <vector>
#include <memory>

#include "Waypoint.h"
#include "Plane.h"

class PlaneBoard
{
public:
    static const int BOARD_SIZE = 110;

private:
    PlaneBoard();
    std::shared_ptr<Plane> plane;
    // TODO: consider typedefs here
    std::shared_ptr<std::vector<Waypoint>> waypointsReached;
    std::shared_ptr<std::vector<Waypoint>> waypointsToReach;

    std::shared_ptr<std::vector<Waypoint>> pointsVisited;
    std::shared_ptr<std::vector<Waypoint>> pointsToNextWaypoint;

public:
    PlaneBoard(PlaneBoard const&) = delete;
    PlaneBoard(PlaneBoard&&) = delete;
    PlaneBoard& operator=(PlaneBoard const&) = delete;
    PlaneBoard& operator=(PlaneBoard &&) = delete;

    static std::shared_ptr<PlaneBoard> instance();

    std::shared_ptr<Plane> getPlane() const;
    std::shared_ptr<std::vector<Waypoint>> getReachedWaypoints() const;
    std::shared_ptr<std::vector<Waypoint>> getToReachWaypoints() const;
    std::shared_ptr<std::vector<Waypoint>> getVisitedPoints() const;
    std::shared_ptr<std::vector<Waypoint>> getNextWaypointPoints() const;
};
