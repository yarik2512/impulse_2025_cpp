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
    int free_pcs{}; // кол-во незанятых столов
    std::vector<time_point> pc_start; // время начала новой сессии стола
    std::vector<long long> income; // доход столов
    std::vector<mins> up_time; // время работы столов
    long long cost{};
    time_point open_time, close_time;
    std::map<std::string, int> clients; // словарь клиент - номер стола
    std::queue<std::string> clients_queue; // очередь ожидания

public:
    Club() = default;

    [[nodiscard]] time_point get_open_time() const;

    [[nodiscard]] time_point get_close_time() const;

    void process_action(const Action &action); // обработка события

    [[nodiscard]] std::vector<Action> process_remaining_clients(); // обработка оставшихся клиентов

    [[nodiscard]] std::vector<std::pair<long long, mins> > get_pc_info() const; // информация о столах

    [[nodiscard]] int get_n_pcs() const;

    friend std::istream &operator>>(std::istream &in, Club &club);
};

#endif //CLUB_H
