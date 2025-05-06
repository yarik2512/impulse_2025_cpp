#ifndef CLUB_H
#define CLUB_H

#include <iostream>
#include <chrono>
#include "chronoio.h"
#include <map>
#include <queue>
#include "action.h"

class Club {
public:
    using time_point = std::chrono::time_point<std::chrono::system_clock>;
    using mins = std::chrono::minutes;
    using hours = std::chrono::hours;

private:
    int n_pcs{};
    int free_pcs{};
    std::vector<time_point> pc_start;
    std::vector<unsigned long long> income;
    std::vector<mins> up_time;
    unsigned long long cost{};
    time_point open_time, close_time;
    std::map<std::string, int> clients;
    std::queue<std::string> clients_queue;

public:
    Club() = default;

    [[nodiscard]] time_point get_open_time() const;

    [[nodiscard]] time_point get_close_time() const;

    void process_action(const Action &action);

    [[nodiscard]] std::vector<Action> process_remaining_clients();

    [[nodiscard]] std::vector<std::pair<unsigned long long, mins> > get_pc_info() const;

    friend std::istream &operator>>(std::istream &in, Club &club);
};

#endif //CLUB_H
