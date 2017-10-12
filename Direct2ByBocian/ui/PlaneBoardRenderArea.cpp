#include <vector>
#include <iterator>
#include <QPoint>
#include <QTextItem>

#include "PlaneBoardRenderArea.h"
#include "plane/WaypointConverter.h"

const QColor PlaneBoardRenderArea::ROUTE_BEHIND_COLOR = QColor(0, 0, 60);
const QColor PlaneBoardRenderArea::ROUTE_IN_FRONT_COLOR = QColor(0, 130, 255);
const QColor PlaneBoardRenderArea::PURE_BLACK = QColor(0, 0, 0);
const QColor PlaneBoardRenderArea::PURE_RED = QColor(255, 0, 0);
const QColor PlaneBoardRenderArea::PURE_YELLOW = QColor(255, 255, 0);


void PlaneBoardRenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(pen);
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

QPointF PlaneBoardRenderArea::getPointOnMathTypeAxis(const QPointF &pointToSwap)
{
    return QPointF(pointToSwap.x(), GRID_SIDE_SIZE - pointToSwap.y());
}

void PlaneBoardRenderArea::renderPlane(QPainter &painter)
{
    QPointF planePosition = WaypointConverter::toQPointF(planeBoard->getPlane()->getPosition(), GRID_SIZE_MULTIPLIER);
    painter.drawEllipse(PlaneBoardRenderArea::getPointOnMathTypeAxis(planePosition), PLANE_CIRCLE_RADIUS, PLANE_CIRCLE_RADIUS);
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
    std::unique_ptr<std::vector<int>> overlapingWaypointIndexes = getOverlapingWaypointIndexesInRouteAhead();

    if (overlapingWaypointIndexes->empty())
    {
        drawPolyline(planeBoard->getToReachWaypoints(), painter, ROUTE_IN_FRONT_COLOR);
        if (!planeBoard->getToReachWaypoints()->empty())
        {
            connectWithPlane(planeBoard->getToReachWaypoints()->at(0),
                             painter, ROUTE_IN_FRONT_COLOR);
        }
    }
    else
    {
        for (int idx = 0; idx < overlapingWaypointIndexes->size() - 1; idx++)
        {
            if (overlapingWaypointIndexes->at(idx + 1) - overlapingWaypointIndexes->at(idx) != 1)
            {
                drawLine(planeBoard->getToReachWaypoints()->at(overlapingWaypointIndexes->at(idx)),
                         planeBoard->getToReachWaypoints()->at(overlapingWaypointIndexes->at(idx + 1)),
                         painter, PURE_RED);

                for (int idx2 = overlapingWaypointIndexes->at(idx); idx2 < overlapingWaypointIndexes->at(idx + 1); idx2++)
                {
                    drawLine(planeBoard->getToReachWaypoints()->at(idx2),
                             planeBoard->getToReachWaypoints()->at(idx2 + 1), painter, PURE_YELLOW);
                }
            }
            else
            {
                drawLine(planeBoard->getToReachWaypoints()->at(overlapingWaypointIndexes->at(idx)),
                         planeBoard->getToReachWaypoints()->at(overlapingWaypointIndexes->at(idx + 1)),
                         painter, ROUTE_IN_FRONT_COLOR);
            }
        }

        QColor planeConnectionColor = overlapingWaypointIndexes->at(0) == 0 ? ROUTE_IN_FRONT_COLOR : PURE_YELLOW;
        connectWithPlane(planeBoard->getToReachWaypoints()->at(overlapingWaypointIndexes->at(0)), painter, planeConnectionColor);
    }
}

void PlaneBoardRenderArea::renderWaypoints(QPainter &painter)
{
    renderWaypoints(painter, planeBoard->getToReachWaypoints());
    renderWaypoints(painter, planeBoard->getReachedWaypoints());
}

void PlaneBoardRenderArea::renderWaypoints(QPainter &painter, WaypointsDequePtr waypoints)
{
    pen = QPen();
    pen.setColor(PURE_BLACK);
    painter.setPen(pen);
    for (auto &waypoint : *waypoints)
    {
        QPointF drawPoint = WaypointConverter::toQPointF(waypoint, GRID_SIZE_MULTIPLIER);
        drawPoint = PlaneBoardRenderArea::getPointOnMathTypeAxis(drawPoint);
        painter.drawText(drawPoint.x() + LABEL_AXIS_TRANSLATION, drawPoint.y(),
                         LABEL_WIDTH, LABEL_HEIGHT, LABEL_FLAGS, QString(waypoint.getLabel().c_str()));
        painter.setBrush(Qt::black);
        painter.drawEllipse(drawPoint, WAYPOINT_CIRCLE_RADIUS, WAYPOINT_CIRCLE_RADIUS);
    }
}

void PlaneBoardRenderArea::drawPolyline(WaypointsDequePtr points, QPainter &painter, const QColor &color)
{
    std::vector<QPointF> pointsToDraw;
    pointsToDraw.reserve(points->size());
    for (auto &waypoint : *points)
    {
        QPointF drawPoint = WaypointConverter::toQPointF(waypoint, GRID_SIZE_MULTIPLIER);
        pointsToDraw.push_back(PlaneBoardRenderArea::getPointOnMathTypeAxis(drawPoint));
    }

    pen = QPen();
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawPolyline(pointsToDraw.data(), pointsToDraw.size());
}

void PlaneBoardRenderArea::drawLine(const Waypoint &waypointA, const Waypoint &waypointB, QPainter &painter, const QColor &color)
{
    pen = QPen();
    pen.setColor(color);
    painter.setPen(pen);
    QPointF pointA = WaypointConverter::toQPointF(waypointA, GRID_SIZE_MULTIPLIER);
    QPointF pointB = WaypointConverter::toQPointF(waypointB, GRID_SIZE_MULTIPLIER);
    painter.drawLine(PlaneBoardRenderArea::getPointOnMathTypeAxis(pointA),
                     PlaneBoardRenderArea::getPointOnMathTypeAxis(pointB));
}

void PlaneBoardRenderArea::connectWithPlane(const Waypoint &waypoint, QPainter &painter, const QColor &color)
{
    QPointF planePosition = WaypointConverter::toQPointF(planeBoard->getPlane()->getPosition(), GRID_SIZE_MULTIPLIER);
    QPointF connectionPoint = WaypointConverter::toQPointF(waypoint, GRID_SIZE_MULTIPLIER);
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawLine(PlaneBoardRenderArea::getPointOnMathTypeAxis(planePosition),
                     PlaneBoardRenderArea::getPointOnMathTypeAxis(connectionPoint));
}

void PlaneBoardRenderArea::drawPlaneBoardStatus(QPainter &painter)
{
    renderPlane(painter);
    renderRoadAhead(painter);
    renderRoadTraveled(painter);
    renderWaypoints(painter);
}

std::unique_ptr<std::vector<int> > PlaneBoardRenderArea::getOverlapingWaypointIndexesInRouteAhead() const
{
    std::unique_ptr<std::vector<int>> overlapingWaypointIndexes = std::make_unique<std::vector<int>>();
    overlapingWaypointIndexes->reserve(planeBoard->getAlternativeWaypointsToReach()->size());

    for (const Waypoint &waypoint : *planeBoard->getAlternativeWaypointsToReach())
    {
        auto positionIt = std::find(planeBoard->getToReachWaypoints()->begin() + overlapingWaypointIndexes->size(),
                                    planeBoard->getToReachWaypoints()->end(), waypoint);
        overlapingWaypointIndexes->push_back(std::distance(planeBoard->getToReachWaypoints()->begin(), positionIt));
    }
    return std::move(overlapingWaypointIndexes);
}
