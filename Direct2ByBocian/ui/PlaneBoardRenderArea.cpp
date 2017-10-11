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
    drawPolyline(planeBoard->getToReachWaypoints(), painter, ROUTE_IN_FRONT_COLOR);
    if (!planeBoard->getToReachWaypoints()->empty())
    {
        connectWithPlane(planeBoard->getToReachWaypoints()->at(0),
                        painter, ROUTE_IN_FRONT_COLOR);
    }
}

void PlaneBoardRenderArea::renderAlternativeRoute(QPainter &painter)
{
    if (planeBoard->getAlternativeWaypointsToReach()->size() == 0)
    {
        return;
    }

    pen = QPen();
    pen.setColor(PURE_RED);
    painter.setPen(pen);

    // TODO: this logic needs to be divided into separate methods
    std::vector<int> foundIndexes;
    foundIndexes.reserve(planeBoard->getAlternativeWaypointsToReach()->size());
    for (const Waypoint &waypoint : *planeBoard->getAlternativeWaypointsToReach())
    {
        auto pos = std::find(planeBoard->getToReachWaypoints()->begin() + foundIndexes.size(),
                             planeBoard->getToReachWaypoints()->end(), waypoint);
        foundIndexes.push_back(std::distance(planeBoard->getToReachWaypoints()->begin(), pos));
    }

    for (int waypointIndex = 0; waypointIndex < foundIndexes.size() - 1; waypointIndex++)
    {
        if (foundIndexes[0] != 0)
        {
            QPointF fromPoint = WaypointConverter::toQPointF(planeBoard->getPlane()->getPosition(),
                                                             GRID_SIZE_MULTIPLIER);
            QPointF toPoint = WaypointConverter::toQPointF(planeBoard->getToReachWaypoints()->at(foundIndexes[0]),
                    GRID_SIZE_MULTIPLIER);
            painter.drawLine(PlaneBoardRenderArea::getPointOnMathTypeAxis(fromPoint),
                             PlaneBoardRenderArea::getPointOnMathTypeAxis(toPoint));
        }

        if (foundIndexes[waypointIndex + 1] - foundIndexes[waypointIndex] != 1)
        {
            QPointF fromPoint = WaypointConverter::toQPointF(planeBoard->getToReachWaypoints()->at(foundIndexes[waypointIndex]),
                                                             GRID_SIZE_MULTIPLIER);
            QPointF toPoint = WaypointConverter::toQPointF(planeBoard->getToReachWaypoints()->at(foundIndexes[waypointIndex + 1]),
                    GRID_SIZE_MULTIPLIER);
            painter.drawLine(PlaneBoardRenderArea::getPointOnMathTypeAxis(fromPoint),
                             PlaneBoardRenderArea::getPointOnMathTypeAxis(toPoint));
        }
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
    renderAlternativeRoute(painter);
    renderWaypoints(painter);
}
