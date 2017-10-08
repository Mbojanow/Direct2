#include "PlaneBoardRenderArea.h"
#include "plane/Waypoint.h"


void PlaneBoardRenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);
}

PlaneBoardRenderArea::PlaneBoardRenderArea(std::shared_ptr<PlaneBoard> planeBoard, QWidget *parent) : QWidget(parent)
{
    this->planeBoard = planeBoard;
    setFixedSize(GRID_SIDE_SIZE, GRID_SIDE_SIZE);
}

void PlaneBoardRenderArea::renderRoadTraveled(QPainter *painter)
{
    WaypointsDequePtr reachedWaypoints = planeBoard->getReachedWaypoints();
    for (auto waypoint : *reachedWaypoints)
    {
        // TODO: convert waypoints to QPoints -> create converter for that...
        //painter->drawPolyline();
    }
}
