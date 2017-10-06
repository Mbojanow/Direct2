#include "MainWindowLayout.h"
#include "plane/PlaneBoard.h"
#include <iostream>

constexpr QSize MainWindowLayout::COMMON_BUTTON_SIZE;

MainWindowLayout::MainWindowLayout()
{
    initWidgets();
    setAppLayout();
}

MainWindowLayout::~MainWindowLayout()
{
    delete mainWidget;
}

QWidget* MainWindowLayout::getMainWidget() const
{
    return mainWidget;
}

void MainWindowLayout::initWidgets()
{
    mainWidget = new QWidget();
    initRouteMap();
    initButtons();
    initSpeedCheckboxes();
    initLayout();
}

void MainWindowLayout::initRouteMap()
{
    planeRouteMap = new QLabel(mainWidget);
    planeRouteMap->setFixedSize(GRID_SIDE_SIZE, GRID_SIDE_SIZE);
}

void MainWindowLayout::initButtons()
{
    generateFlightPlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(generateFlightPlanButton, "Generate flight plan");

    runSimulationButton = new QPushButton(mainWidget);
    initCommonPushButtonData(runSimulationButton, "Run simulation");

    pauseSimulationButton = new QPushButton(mainWidget);
    initCommonPushButtonData(pauseSimulationButton, "Pause simulation");

    alternativeFlightPlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(alternativeFlightPlanButton, "Submit flight plan alternative");

    acceptAlternativePlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(acceptAlternativePlanButton, "Accept plan alternative");

    rejectAlternativePlanButton = new QPushButton(mainWidget);
    initCommonPushButtonData(rejectAlternativePlanButton, "Reject plan alternative");

    acceptAlternativePlanButton->setEnabled(false);
    rejectAlternativePlanButton->setEnabled(false);
}

void MainWindowLayout::initSpeedCheckboxes()
{
    doubleSpeedCheckBox = new QCheckBox(mainWidget);
    doubleSpeedCheckBox->setText("2x");
    halfSpeedCheckBox = new QCheckBox(mainWidget);
    halfSpeedCheckBox->setText("0.5x");
}

void MainWindowLayout::initLayout()
{
    mainLayout = new QHBoxLayout(mainWidget);
    leftWindowSideLayout = new QVBoxLayout();
    rightWindowSideLayout = new QVBoxLayout();
}

void MainWindowLayout::initCommonPushButtonData(QPushButton *button, const QString &buttonText)
{
    button->setFixedSize(COMMON_BUTTON_SIZE);
    button->setText(buttonText);
}

void MainWindowLayout::setAppLayout()
{
    planeRouteMap->setText("TEST TEXT");
    leftWindowSideLayout->addWidget(planeRouteMap);

    rightWindowSideLayout->addWidget(generateFlightPlanButton);
    rightWindowSideLayout->addWidget(runSimulationButton);
    rightWindowSideLayout->addWidget(pauseSimulationButton);
    rightWindowSideLayout->addWidget(alternativeFlightPlanButton);
    rightWindowSideLayout->addWidget(acceptAlternativePlanButton);
    rightWindowSideLayout->addWidget(rejectAlternativePlanButton);
    setRadioBoxesLayout();

    mainLayout->addLayout(leftWindowSideLayout);
    mainLayout->addLayout(rightWindowSideLayout);
    mainWidget->setLayout(mainLayout);
}

void MainWindowLayout::setRadioBoxesLayout()
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
