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
        std::cout << "Failed to open input file" << std::endl;
        return 0;
    }
    Club club;
    try {
        std::cin >> club;
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    std::cout << club.get_open_time() << std::endl;
    try {
        Action a;
        while (std::cin >> a) {
            std::cout << a << std::endl;
            try {
                club.process_action(a);
            } catch (const Action &e) {
                std::cout << e << std::endl;
            }
        }
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    const auto remaining_clients = club.process_remaining_clients();
    for (const auto &a: remaining_clients) {
        std::cout << a << std::endl;
    }
    std::cout << club.get_close_time() << std::endl;
    const auto pc_info = club.get_pc_info();
    for (int i = 0; i < pc_info.size(); ++i) {
        std::cout << i + 1 << " " << pc_info[i].first << " " << pc_info[i].second << std::endl;
    }
    return 0;
}
