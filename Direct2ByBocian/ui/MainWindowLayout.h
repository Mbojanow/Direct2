#pragma once
#include <QtWidgets>
#include <QString>

#include "plane/PlaneBoard.h"

class MainWindowLayout
{
private:
    static const int GRID_SIZE_MULTIPLIER = 5;
    static const int GRID_SIDE_SIZE = GRID_SIZE_MULTIPLIER * PlaneBoard::BOARD_SIZE;
    static constexpr QSize COMMON_BUTTON_SIZE = QSize(200, 50);

    QWidget *mainWidget;
    QLabel *planeRouteMap;
    QPushButton *generateFlightPlanButton;
    QPushButton *runSimulationButton;
    QPushButton *pauseSimulationButton;
    QPushButton *alternativeFlightPlanButton;
    QPushButton *acceptAlternativePlanButton;
    QPushButton *rejectAlternativePlanButton;
    QCheckBox *doubleSpeedCheckBox;
    QCheckBox *halfSpeedCheckBox;

    QHBoxLayout *mainLayout;
    QVBoxLayout *leftWindowSideLayout;
    QVBoxLayout *rightWindowSideLayout;

public:


    MainWindowLayout();
    ~MainWindowLayout();

    QWidget *getMainWidget() const;

private:
    void initWidgets();
    void initRouteMap();
    void initButtons();
    void initSpeedCheckboxes();
    void initLayout();

    void setRadioBoxesLayout();
    void setAppLayout();

    void initCommonPushButtonData(QPushButton *button, const QString &buttonText);
};
