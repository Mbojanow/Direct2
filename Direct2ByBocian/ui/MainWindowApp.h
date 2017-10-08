#pragma once

#include <QtWidgets>
#include <QString>
#include <QGraphicsItem>

#include <memory>

#include "plane/PlaneBoard.h"
#include "simulation/RouteSimulation.h"

/*
 * MainWindowApp Q_OBJECT class declaration.
 * Contains:
 *  - single widget with mainLayout, that is parent for every button, checkbox etc. used in MainWindow
 *  - all the widget handlers
 *  - painter logic (if I can make it;))
 */
class MainWindowApp : public QObject
{
    Q_OBJECT

private:
    static constexpr QSize COMMON_BUTTON_SIZE = QSize(200, 50);

    // widgets
    QWidget *mainWidget;
    //TODO: change this QLabel to PlanBoardRenderArea
    QLabel *planeRouteMap;
    QPushButton *generateFlightPlanButton;
    QPushButton *runSimulationButton;
    QPushButton *pauseSimulationButton;
    QPushButton *alternativeFlightPlanButton;
    QLabel *alternativePlanInfoLabel;
    QPushButton *acceptAlternativePlanButton;
    QPushButton *rejectAlternativePlanButton;
    QCheckBox *doubleSpeedCheckBox;
    QCheckBox *halfSpeedCheckBox;

    // layouts
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftWindowSideLayout;
    QVBoxLayout *rightWindowSideLayout;

    std::shared_ptr<RouteSimulation> routeSimulation;
    WaypointsDequePtr alternativeRouteWaypoints;

public:
    MainWindowApp(QWidget *parent = nullptr);
    ~MainWindowApp();

    QWidget *getMainWidget() const;

private slots:
    void handleGenerateFlightPlan();
    void handleRunSimulation();
    void handlePauseSimulation();
    void handleSubmitFlightPlanAlternative();
    void handleAcceptFlightPlanAlternative();
    void handleRejectFlightPlanAlternative();
    void handleDoubleSpeedChange(bool toggled);
    void handleHalfSpeedChange(bool toggled);

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
