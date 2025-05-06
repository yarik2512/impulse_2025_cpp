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
    std::stringstream ss;
    Club club;
    try {
        std::cin >> club;
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    ss << club.get_open_time() << std::endl;
    try {
        Action a;
        while (std::cin >> a) {
            ss << a << std::endl;
            try {
                club.process_action(a);
            } catch (const Action &e) {
                ss << e << std::endl;
            }
        }
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    const auto remaining_clients = club.process_remaining_clients();
    for (const auto &a: remaining_clients) {
        ss << a << std::endl;
    }
    ss << club.get_close_time() << std::endl;
    const auto pc_info = club.get_pc_info();
    for (int i = 0; i < pc_info.size(); ++i) {
        ss << i + 1 << " " << pc_info[i].first << " " << pc_info[i].second << std::endl;
    }
    std::cout << ss.str();
    return 0;
}
