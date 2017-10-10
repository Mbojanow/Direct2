#pragma once

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QColor>
#include <memory>

#include "plane/PlaneBoard.h"

/*
 * PlaneBoardRenderArea class declaration.
 * PlaneBoardRenderArea is a widget that renders plane route based on items received
 * in planeBoard in constructor.
 */
class PlaneBoardRenderArea : public QWidget
{
    Q_OBJECT
private:
    static const int GRID_SIZE_MULTIPLIER = 8;
    static const int GRID_SIDE_SIZE = GRID_SIZE_MULTIPLIER * PlaneBoard::BOARD_SIZE;
    static const QColor ROUTE_BEHIND_COLOR;
    static const QColor ROUTE_IN_FRONT_COLOR;
    static const QColor PURE_BLACK;
    static const int WAYPOINT_CIRCLE_RADIUS = 2;
    // TODO : change it to triangle in the future
    static const int PLANE_CIRCLE_RADIUS = 5;

    static const int LABEL_WIDTH = 80;
    static const int LABEL_HEIGHT = 15;
    static const int LABEL_AXIS_TRANSLATION = 15;
    static const int LABEL_FLAGS = 0;

    std::shared_ptr<PlaneBoard> planeBoard;
    QPen pen;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    PlaneBoardRenderArea(std::shared_ptr<PlaneBoard> planeBoard, QWidget *parent = nullptr);
    bool shouldRender() const;

private:
    void renderPlane(QPainter &painter);
    void renderRoadTraveled(QPainter &painter);
    void renderRoadAhead(QPainter &painter);
    void renderAlternativeRoute(QPainter &painter);
    void renderWaypoints(QPainter &painter);
    void renderWaypoints(QPainter &painter, WaypointsDequePtr waypoints);

    void drawPolyline(WaypointsDequePtr points, QPainter &painter, const QColor &color);
    void connectWithPlane(const Waypoint &waypoint, QPainter &painter, const QColor &color);
    void drawPlaneBoardStatus(QPainter &painter);


};
