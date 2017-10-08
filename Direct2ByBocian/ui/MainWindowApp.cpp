#include "MainWindowApp.h"
#include "plane/PlaneBoard.h"
#include <iostream>

constexpr QSize MainWindowApp::COMMON_BUTTON_SIZE;

MainWindowApp::MainWindowApp() : QObject()
{
    initWidgets();
    setAppLayout();
    routeSimulation = std::make_shared<RouteSimulation>();
    connectWidgets();
}

void MainWindowApp::handleGenerateFlightPlan()
{
    routeSimulation->generateFlightPlan();
    runSimulationButton->setEnabled(true);
}

void MainWindowApp::handleRunSimulation()
{
    if (!simulationStarted())
    {
        routeSimulation->start();
        generateFlightPlanButton->setEnabled(false);
        alternativeFlightPlanButton->setEnabled(true);
    }
    else
    {
        routeSimulation->unpause();
    }
    pauseSimulationButton->setEnabled(true);
    runSimulationButton->setEnabled(false);
}

void MainWindowApp::handlePauseSimulation()
{
    routeSimulation->pause();
    runSimulationButton->setEnabled(true);
    pauseSimulationButton->setEnabled(false);
}

void MainWindowApp::handleSubmitFlightPlanAlternative()
{
    auto alternativeFlightPlanInfo = routeSimulation->generateFlightPlanAlternative();
    if (alternativeFlightPlanInfo.second)
    {
        // no waypoints removed
        alternativePlanInfoLabel->setText("Flight plan has not been modified");
    }
    else
    {
        alternativeRouteWaypoints = alternativeFlightPlanInfo.first;
        alternativeFlightPlanButton->setEnabled(false);
        acceptAlternativePlanButton->setEnabled(true);
        rejectAlternativePlanButton->setEnabled(true);
        alternativePlanInfoLabel->setText("");
        // TODO: draw alternative route here
    }
}

void MainWindowApp::handleAcceptFlightPlanAlternative()
{
    routeSimulation->acceptFlightPlanAlternative();
    alternativeFlightPlanButton->setEnabled(true);
    acceptAlternativePlanButton->setEnabled(false);
    rejectAlternativePlanButton->setEnabled(false);
}

void MainWindowApp::handleRejectFlightPlanAlternative()
{
    routeSimulation->rejectFlightPlanAlternative();
    alternativeFlightPlanButton->setEnabled(true);
    acceptAlternativePlanButton->setEnabled(false);
    rejectAlternativePlanButton->setEnabled(false);
}

void MainWindowApp::handleDoubleSpeedChange(bool toggled)
{
    if (toggled)
    {
        routeSimulation->changeSpeed(SimulationSpeed::DOUBLE);
        halfSpeedCheckBox->setEnabled(false);
    }
    else
    {
        routeSimulation->changeSpeed(SimulationSpeed::NORMAL);
        halfSpeedCheckBox->setEnabled(true);
    }
}

void MainWindowApp::handleHalfSpeedChange(bool toggled)
{
    if (toggled)
    {
        routeSimulation->changeSpeed(SimulationSpeed::HALF);
        doubleSpeedCheckBox->setEnabled(false);
    }
    else
    {
        routeSimulation->changeSpeed(SimulationSpeed::NORMAL);
        doubleSpeedCheckBox->setEnabled(true);
    }
}

MainWindowApp::~MainWindowApp()
{
    delete mainWidget;
}

QWidget* MainWindowApp::getMainWidget() const
{
    return mainWidget;
}

void MainWindowApp::initWidgets()
{
    mainWidget = new QWidget();
    initRouteMap();
    initButtons();
    initSpeedCheckboxes();
    initLayout();
}

void MainWindowApp::initRouteMap()
{
    planeRouteMap = new QLabel(mainWidget);
    planeRouteMap->setFixedSize(GRID_SIDE_SIZE, GRID_SIDE_SIZE);
}

void MainWindowApp::initButtons()
{
    generateFlightPlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(generateFlightPlanButton, "Generate flight plan");
    generateFlightPlanButton->setEnabled(true);

    runSimulationButton = new QPushButton(mainWidget);
    initCommonPushButtonData(runSimulationButton, "Run simulation");

    pauseSimulationButton = new QPushButton(mainWidget);
    initCommonPushButtonData(pauseSimulationButton, "Pause simulation");

    alternativeFlightPlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(alternativeFlightPlanButton, "Submit flight plan alternative");

    alternativePlanInfoLabel = new QLabel(mainWidget);

    acceptAlternativePlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(acceptAlternativePlanButton, "Accept plan alternative");

    rejectAlternativePlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(rejectAlternativePlanButton, "Reject plan alternative");

    acceptAlternativePlanButton->setEnabled(false);
    rejectAlternativePlanButton->setEnabled(false);
}

void MainWindowApp::initSpeedCheckboxes()
{
    doubleSpeedCheckBox = new QCheckBox(mainWidget);
    doubleSpeedCheckBox->setText("2x");
    halfSpeedCheckBox = new QCheckBox(mainWidget);
    halfSpeedCheckBox->setText("0.5x");
}

void MainWindowApp::initLayout()
{
    mainLayout = new QHBoxLayout(mainWidget);
    leftWindowSideLayout = new QVBoxLayout();
    rightWindowSideLayout = new QVBoxLayout();
}

void MainWindowApp::initCommonPushButtonData(QPushButton *button, const QString &buttonText)
{
    button->setFixedSize(COMMON_BUTTON_SIZE);
    button->setText(buttonText);
    button->setEnabled(false);
}

void MainWindowApp::setAppLayout()
{
    planeRouteMap->setText("ROUTE WILL BE DRAWN HERE");
    leftWindowSideLayout->addWidget(planeRouteMap);

    rightWindowSideLayout->addWidget(generateFlightPlanButton);
    rightWindowSideLayout->addWidget(runSimulationButton);
    rightWindowSideLayout->addWidget(pauseSimulationButton);
    rightWindowSideLayout->addWidget(alternativeFlightPlanButton);
    rightWindowSideLayout->addWidget(alternativePlanInfoLabel);
    rightWindowSideLayout->addWidget(acceptAlternativePlanButton);
    rightWindowSideLayout->addWidget(rejectAlternativePlanButton);
    setRadioBoxesLayout();

    mainLayout->addLayout(leftWindowSideLayout);
    mainLayout->addLayout(rightWindowSideLayout);
    mainWidget->setLayout(mainLayout);
}

void MainWindowApp::setRadioBoxesLayout()
{
    QWidget *speedBoxWidget = new QWidget(mainWidget);
    QHBoxLayout *speedBoxLayout = new QHBoxLayout(speedBoxWidget);
    QLabel *speedLabel = new QLabel(speedBoxWidget);
    speedLabel->setText("Simulation speed: ");
    speedBoxLayout->addWidget(speedLabel);
    speedBoxLayout->addWidget(doubleSpeedCheckBox);
    speedBoxLayout->addWidget(halfSpeedCheckBox);
    speedBoxWidget->setLayout(speedBoxLayout);
    rightWindowSideLayout->addWidget(speedBoxWidget);
}

void MainWindowApp::connectWidgets()
{
    // TODO: change connect to QT5 style connect
    connect(generateFlightPlanButton, SIGNAL (released()), this, SLOT (handleGenerateFlightPlan()));
    connect(runSimulationButton, SIGNAL (released()), this, SLOT (handleRunSimulation()));
    connect(pauseSimulationButton, SIGNAL (released()), this, SLOT (handlePauseSimulation()));
    connect(alternativeFlightPlanButton, SIGNAL (released()), this, SLOT (handleSubmitFlightPlanAlternative()));
    connect(acceptAlternativePlanButton, SIGNAL (released()), this, SLOT (handleAcceptFlightPlanAlternative()));
    connect(rejectAlternativePlanButton, SIGNAL (released()), this, SLOT (handleRejectFlightPlanAlternative()));
    connect(doubleSpeedCheckBox, SIGNAL(toggled(bool)), this, SLOT(handleDoubleSpeedChange(bool)));
    connect(halfSpeedCheckBox, SIGNAL(toggled(bool)), this, SLOT(handleHalfSpeedChange(bool)));
}

bool MainWindowApp::simulationStarted() const
{
    return routeSimulation->getElapsedTime() > 0;
}
