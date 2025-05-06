#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <string>
#include <chrono>
#include "chronoio.h"

class Action {
public:
    using time_point = std::chrono::time_point<std::chrono::system_clock>;

private:
    time_point time{};
    int id{};
    std::string client{};
    int pc{};

public:
    Action() = default;

    Action(const time_point &time_, int id_);

    Action(const time_point &time_, int id_, std::string client_);

    Action(const time_point &time_, int id_, std::string client_, int pc_);

    friend std::istream &operator>>(std::istream &in, Action &action);

    friend std::ostream &operator<<(std::ostream &out, const Action &action);

    friend class Club;
};

#endif
