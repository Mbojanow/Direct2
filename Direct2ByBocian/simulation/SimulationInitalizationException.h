#pragma once

#include <stdexcept>
#include <string>

/*
 * SimulationInitializationException declaration and implementation.
 */
class SimulationInitializationException : public std::runtime_error
{
public:
    explicit SimulationInitializationException(const std::string &errorMsg) : std::runtime_error(errorMsg) {}
    explicit SimulationInitializationException(const char *errorMsg) : std::runtime_error(errorMsg) {}
};
