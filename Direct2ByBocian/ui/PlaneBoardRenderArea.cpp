#include <vector>
#include <QPoint>
#include <QTextItem>

#include "PlaneBoardRenderArea.h"
#include "plane/WaypointConverter.h"

const QColor PlaneBoardRenderArea::ROUTE_BEHIND_COLOR = QColor(0, 0, 60);
const QColor PlaneBoardRenderArea::ROUTE_IN_FRONT_COLOR = QColor(0, 130, 255);


void PlaneBoardRenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawPlaneBoardStatus(painter);
}

PlaneBoardRenderArea::PlaneBoardRenderArea(std::shared_ptr<PlaneBoard> planeBoard, QWidget *parent) : QWidget(parent)
{
    this->planeBoard = planeBoard;
    setFixedSize(GRID_SIDE_SIZE + PlaneBoard::BOARD_SIZE, GRID_SIDE_SIZE + PlaneBoard::BOARD_SIZE);
}

bool PlaneBoardRenderArea::shouldRender() const
{
    return (planeBoard->getToReachWaypoints() > 0 || planeBoard->getVisitedPoints()->size() > 1);
}

void PlaneBoardRenderArea::renderPlane(QPainter &painter)
{
    painter.drawEllipse(WaypointConverter::toQPointF(planeBoard->getPlane()->getPosition(), GRID_SIZE_MULTIPLIER), 10, 10);
}

void PlaneBoardRenderArea::renderRoadTraveled(QPainter &painter)
{
    drawPolyline(planeBoard->getReachedWaypoints(), painter, ROUTE_BEHIND_COLOR);
    if (!planeBoard->getReachedWaypoints()->empty())
    {
        connectWithPlane(planeBoard->getReachedWaypoints()->at(planeBoard->getReachedWaypoints()->size() - 1),
                        painter, ROUTE_BEHIND_COLOR);
    }
}

void PlaneBoardRenderArea::renderRoadAhead(QPainter &painter)
{
    drawPolyline(planeBoard->getToReachWaypoints(), painter, ROUTE_IN_FRONT_COLOR);
    if (!planeBoard->getToReachWaypoints()->empty())
    {
        connectWithPlane(planeBoard->getToReachWaypoints()->at(0),
                        painter, ROUTE_IN_FRONT_COLOR);
    }
}

void PlaneBoardRenderArea::renderAlternativeRoute(QPainter &painter)
{
    // TODO: implement
}

void PlaneBoardRenderArea::drawPolyline(WaypointsDequePtr points, QPainter &painter, const QColor &color)
{
    std::vector<QPointF> pointsToDraw;
    pointsToDraw.reserve(points->size() + 1);
    for (auto &waypoint : *points)
    {
        QPointF drawPoint = WaypointConverter::toQPointF(waypoint, GRID_SIZE_MULTIPLIER);
        pointsToDraw.push_back(drawPoint);
        // TODO: name magic numbers
        painter.drawText(drawPoint.x(), drawPoint.y(), 50, 10, 0, QString(waypoint.getLabel().c_str()));
    }

    pen = QPen();
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawPolyline(pointsToDraw.data(), pointsToDraw.size());
}

void PlaneBoardRenderArea::connectWithPlane(const Waypoint &waypoint, QPainter &painter, const QColor &color)
{
    QPointF planePosition = WaypointConverter::toQPointF(planeBoard->getPlane()->getPosition(), GRID_SIZE_MULTIPLIER);
    QPointF connectionPoint = WaypointConverter::toQPointF(waypoint, GRID_SIZE_MULTIPLIER);
    pen.setColor(color);
    painter.drawLine(planePosition, connectionPoint);
    // TODO: draw waypoint labels
}

void PlaneBoardRenderArea::drawPlaneBoardStatus(QPainter &painter)
{
    renderPlane(painter);
    renderRoadAhead(painter);
    renderRoadTraveled(painter);
    renderAlternativeRoute(painter);
}
