#pragma once
#include <QtWidgets>
#include <QString>
#include <QGraphicsItem>

#include <memory>

#include "plane/PlaneBoard.h"
#include "simulation/RouteSimulation.h"

class MainWindowApp : public QObject
{
    Q_OBJECT

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

    std::shared_ptr<RouteSimulation> routeSimulation;
    WaypointsDequePtr alternativeRouteWaypoints;

public:
    MainWindowApp();
    ~MainWindowApp();

    QWidget *getMainWidget() const;

private slots:
    void handleGenerateFlightPlan();
    void handleRunSimulation();
    void handlePauseSimulation();
    void handleSubmitFlightPlanAlternative();
    void handleAcceptFlightPlanAlternative();
    void handleRejectFlightPlanAlternative();

private:
    void initWidgets();
    void initRouteMap();
    void initButtons();
    void initSpeedCheckboxes();
    void initLayout();

    void setRadioBoxesLayout();
    void setAppLayout();

    void initCommonPushButtonData(QPushButton *button, const QString &buttonText);
    void connectWidgets();

    bool simulationStarted() const;
};
