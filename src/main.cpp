#include <iostream>
#include "chronoio.h"
#include "action.h"
#include "club.h"

int main(int argc, char *argv[]) {
    if (argc != 2 || argv[1] == nullptr) {
        std::cout << "Wrong args! Usage: task.exe <input_file>" << std::endl;
        return 0;
    }
    if (std::freopen(argv[1], "r", stdin) == nullptr) {
        std::cout << "Failed to open input file: " << argv[1] << std::endl;
        return 0;
    }
    std::ostringstream sout;
    Club club;
    try {
        std::cin >> club;
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    sout << club.get_open_time() << std::endl;
    Club::time_point prev_time = club.get_open_time();
    try {
        Action a;
        while (std::cin >> a) {
            const bool time_valid = a.time >= prev_time;
            const bool id_valid = 1 <= a.id && a.id <= 4;
            const bool client_valid = !a.client.empty() && std::all_of(a.client.begin(), a.client.end(),
                                                                       [](char c) {
                                                                           return std::islower(c) || std::isdigit(c);
                                                                       });
            const bool pc_valid = 1 <= a.pc && a.pc <= club.get_n_pcs();
            prev_time = a.time;
            if (!time_valid || !id_valid || !client_valid || !pc_valid) {
                std::stringstream ss;
                ss << a;
                throw std::runtime_error(ss.str());
            }
            sout << a << std::endl;
            try {
                club.process_action(a);
            } catch (const Action &e) {
                sout << e << std::endl;
            }
        }
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    const auto remaining_clients = club.process_remaining_clients();
    for (const auto &a: remaining_clients) {
        sout << a << std::endl;
    }
    sout << club.get_close_time() << std::endl;
    const auto pc_info = club.get_pc_info();
    for (int i = 0; i < pc_info.size(); ++i) {
        sout << i + 1 << " " << pc_info[i].first << " " << pc_info[i].second << std::endl;
    }
    std::cout << sout.str();
    return 0;
}
