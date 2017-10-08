#pragma once

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPainter>
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
    static const int GRID_SIZE_MULTIPLIER = 5;
    static const int GRID_SIDE_SIZE = GRID_SIZE_MULTIPLIER * PlaneBoard::BOARD_SIZE;

    std::shared_ptr<PlaneBoard> planeBoard;

    QPen pen;
    QBrush brush;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    PlaneBoardRenderArea(std::shared_ptr<PlaneBoard> planeBoard, QWidget *parent = nullptr);

private:
    void renderPlane(QPainter *painter);
    void renderRoadTraveled(QPainter *painter);
    void renderRoadAhead(QPainter *painter);
    void renderAlternativeRoute(QPainter *painter);
};