#include "SimulationState.h"
#include "SimulationConstants.h"


SimulationState::SimulationState()
{
    paused = false;
    finished = false;
    setSimulationStepSleepTime(SimulationSpeed::NORMAL);
    simulationExecutionTime = 0;
}

int SimulationState::getSimulationExecutionTime()
{
    std::lock_guard<std::mutex> lock(mtx);
    return simulationExecutionTime;
}

int SimulationState::getSimulationStepSleepTime()
{
    std::lock_guard<std::mutex> lock(mtx);
    return simulationStepSleepTime;
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

void SimulationState::incrementSimulationExecutionTime()
{
    std::lock_guard<std::mutex> lock(mtx);
    ++simulationExecutionTime;
}

void SimulationState::resetSimulationTimer()
{
    std::lock_guard<std::mutex> lock(mtx);
    simulationExecutionTime = 0;
}

void SimulationState::setSimulationStepSleepTime(const SimulationSpeed &simulationSpeed)
{
    std::lock_guard<std::mutex> lock(mtx);
    switch(simulationSpeed)
    {
        case SimulationSpeed::HALF:
            simulationStepSleepTime = SIMULATION_NORMAL_SPEED_SLEEP_TIME_IN_MS * 2;
            break;
        case SimulationSpeed::NORMAL:
            simulationStepSleepTime = SIMULATION_NORMAL_SPEED_SLEEP_TIME_IN_MS;
            break;
        case SimulationSpeed::DOUBLE:
            simulationStepSleepTime = SIMULATION_NORMAL_SPEED_SLEEP_TIME_IN_MS / 2;
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
