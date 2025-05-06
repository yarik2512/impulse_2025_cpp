#ifndef CHRONOIO_H
#define CHRONOIO_H

#include <iostream>
#include <string>
#include <chrono>

constexpr std::string TIME_FMT = "%H:%M";

std::istream &operator>>(std::istream &in, std::chrono::time_point<std::chrono::system_clock> &time_point);

std::ostream &operator<<(std::ostream &out, const std::chrono::time_point<std::chrono::system_clock> &tp);

std::ostream &operator<<(std::ostream &out, const std::chrono::minutes &mins);

#endif
