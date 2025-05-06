#include "action.h"

Action::Action(const time_point &time_, const int id_) : time(time_), id(id_) {
}

Action::Action(const time_point &time_, const int id_, std::string client_) : time(time_), id(id_),
                                                                              client(std::move(client_)) {
}

Action::Action(const time_point &time_, const int id_, std::string client_, const int pc_) : time(time_), id(id_),
    client(std::move(client_)), pc(pc_) {
}

std::istream &operator>>(std::istream &in, Action &action) {
    std::string input;
    std::getline(in, input);
    if (input.empty()) {
        return in;
    }
    std::stringstream ss(input);
    try {
        ss >> action.time >> action.id >> action.client;
    } catch (const std::runtime_error &) {
        throw std::runtime_error(input);
    }
    if (action.id == 2) {
        ss >> action.pc;
        --action.pc;
    }
    if (ss.fail()) {
        throw std::runtime_error(input);
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Action &action) {
    out << action.time << " " << action.id << " " << action.client;
    if (action.id == 2 || action.id == 12) {
        out << " " << action.pc + 1;
    }
    return out;
}
