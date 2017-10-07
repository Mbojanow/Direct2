#pragma once

#include <mutex>
#include "SimulationSpeed.h"

class SimulationState
{
private:
    int simulationExecutionTime;
    int simulationStepSleepTime;
    bool paused;
    bool finished;
    std::mutex mtx;

public:
    SimulationState();

    int getSimulationExecutionTime();
    int getSimulationStepSleepTime();
    bool isPaused();
    bool isFinished();

    void incrementSimulationExecutionTime();
    void resetSimulationTimer();
    void setSimulationStepSleepTime(const SimulationSpeed &simulationSpeed);
    void setPauseState(bool isPaused);
    void setFinishedState(bool isFinished);
};
