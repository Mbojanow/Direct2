#pragma once

#include <mutex>

#include "SimulationSpeed.h"

/*
 * SimulationState class declaration. SimulationState stores data about simulation
 *  - execution time of simulation in simulation units
 *  - sleep time between simulation steps in milliseconds
 *  - simulation's pause state
 *  - simulation's finished state
 * Methods in this class except constructors are thread safe.
 */
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
