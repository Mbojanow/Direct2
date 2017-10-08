#pragma once

#include <mutex>
#include "SimulationSpeed.h"

class SimulationState
{
private:
    int executionTime;
    int stepSleepTime;
    bool paused;
    bool finished;
    std::mutex mtx;

public:
    SimulationState();

    int getExecutionTime();
    int getStepSleepTime();
    bool isPaused();
    bool isFinished();

    void incrementExecutionTime();
    void resetTimer();
    void setStepSleepTime(const SimulationSpeed &simulationSpeed);
    void setPauseState(bool isPaused);
    void setFinishedState(bool isFinished);
};
