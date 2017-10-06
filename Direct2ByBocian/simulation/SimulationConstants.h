#pragma once

// WAYPOINTS DATA

#define MAX_WAYPOINT_INDEX 11
#define AXIS_LOWER_BOUND_DIFF -2
#define AXIS_UPPER_BOUND_DIFF 2
#define X_AXIS_MULTIPLIER 10

#define ALTITUDE_LOWER_BOUND_DIFF -500
#define ALTITUDE_UPPER_BOUND_DIFF 500

#define WAYPOINT_ALTITUDE_BASE 10000

#define DEPARTURE_X 0
#define DEPARTURE_Y 0
#define DEPARTURE_ALTITUDE 0
#define DEPARTURE_WAYPOINT_INDEX 0

#define TOP_OF_CLIMB_X 10
#define TOP_OF_CLIMB_Y 10
#define TOP_OF_CLIMB_ALTITUDE 10000
#define TOP_OF_CLIMB_WAYPOINT_INDEX 1

#define TOP_OF_DESCENT_X 100
#define TOP_OF_DESCENT_Y 100
#define TOP_OF_DESCENT_ALTITUDE 10000
#define TOP_OF_DESCENT_WAYPOINT_INDEX (MAX_WAYPOINT_INDEX - 1)

#define ARRIVAL_X 110
#define ARRIVAL_Y 110
#define ARRIVAL_ALTITUDE 0
#define ARRIVAL_WAPOINT_INDEX MAX_WAYPOINT_INDEX

#define WAYPOINT_REACHED_DIFF 2

// MOVE DATA
#define MOVE_DIST_PER_SIMULATION_UNIT 1



