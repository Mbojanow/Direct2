#include "SimulationState.h"
#include "SimulationConstants.h"


SimulationState::SimulationState()
{
    paused = false;
    finished = false;
    setStepSleepTime(SimulationSpeed::NORMAL);
    executionTime = 0;
}

int SimulationState::getExecutionTime()
{
    std::lock_guard<std::mutex> lock(mtx);
    return executionTime;
}

int SimulationState::getStepSleepTime()
{
    std::lock_guard<std::mutex> lock(mtx);
    return stepSleepTime;
}

bool SimulationState::isPaused()
{
    std::lock_guard<std::mutex> lock(mtx);
    return paused;
}

bool SimulationState::isFinished()
{
    std::lock_guard<std::mutex> lock(mtx);
    return finished;
}

void SimulationState::incrementExecutionTime()
{
    std::lock_guard<std::mutex> lock(mtx);
    ++executionTime;
}

void SimulationState::resetTimer()
{
    std::lock_guard<std::mutex> lock(mtx);
    executionTime = 0;
}

void SimulationState::setStepSleepTime(const SimulationSpeed &simulationSpeed)
{
    std::lock_guard<std::mutex> lock(mtx);
    switch(simulationSpeed)
    {
        case SimulationSpeed::HALF:
            stepSleepTime = SIMULATION_NORMAL_SPEED_SLEEP_TIME_IN_MS * 2;
            break;
        case SimulationSpeed::NORMAL:
            stepSleepTime = SIMULATION_NORMAL_SPEED_SLEEP_TIME_IN_MS;
            break;
        case SimulationSpeed::DOUBLE:
            stepSleepTime = SIMULATION_NORMAL_SPEED_SLEEP_TIME_IN_MS / 2;
            break;
        default:
            throw std::runtime_error("Unsupported simulation speed");
    }
}
void SimulationState::setPauseState(bool isPaused)
{
    std::lock_guard<std::mutex> lock(mtx);
    paused = isPaused;
}

void SimulationState::setFinishedState(bool isFinished)
{
    std::lock_guard<std::mutex> lock(mtx);
    finished = isFinished;
}
