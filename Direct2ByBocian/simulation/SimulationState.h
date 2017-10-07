#pragma once

#include <mutex>
#include "SimulationSpeed.h"

class SimulationState
{
private:
    int simulationStepSleepTime;
    bool paused;
    bool finished;
    std::mutex mtx;

public:
    SimulationState();

    int getSimulationStepSleepTime();
    bool isPaused();
    bool isFinished();

    void setSimulationStepSleepTime(const SimulationSpeed &simulationSpeed);
    void setPauseState(bool isPaused);
    void setFinishedState(bool isFinished);
};
