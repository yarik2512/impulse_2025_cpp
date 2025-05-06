#include <iostream>
#include "chronoio.h"
#include "action.h"
#include "club.h"

int main(int argc, char *argv[]) {
    if (argc != 2 || argv[1] == nullptr) {
        std::cout << "Wrong args! Usage: task <input_file>" << std::endl;
        return 0;
    }
    if (std::freopen(argv[1], "r", stdin) == nullptr) {
        std::cout << "Failed to open input file: " << argv[1] << std::endl;
        return 0;
    }
    std::ostringstream sout; // вспомогательный буфер
    Club club;
    try {
        std::cin >> club;
    } catch (const std::runtime_error &e) { // ошибка формата ввода
        std::cout << e.what() << std::endl;
        return 0;
    }
    sout << club.get_open_time() << std::endl;
    Club::time_point prev_time{}; // для проверки временного порядка событий
    try {
        Action a;
        while (std::cin >> a) {
            const bool time_valid = a.time >= prev_time; // события идут по возрастанию времени
            const bool id_valid = 1 <= a.id && a.id <= 4; // входящие события имеют id = 1-4
            const bool client_valid = !a.client.empty() && std::all_of(a.client.begin(), a.client.end(),
                                                                       [](char c) {
                                                                           return std::islower(c) || std::isdigit(c);
                                                                       }); // проверка корректности имени
            const bool pc_valid = a.id != 2 || (0 <= a.pc && a.pc < club.get_n_pcs()); // проверка корректности номера стола
            prev_time = a.time;
            if (!time_valid || !id_valid || !client_valid || !pc_valid) { // ошибка формата ввода
                std::stringstream ss;
                ss << a;
                throw std::runtime_error(ss.str());
            }
            sout << a << std::endl;
            try {
                club.process_action(a); // обработка события
            } catch (const Action &e) { // сгенерировано исходящее событие
                sout << e << std::endl;
            }
        }
    } catch (const std::runtime_error &e) { // ошибка формата ввода
        std::cout << e.what() << std::endl;
        return 0;
    }
    const auto remaining_clients = club.process_remaining_clients(); // обработка оставшихся пользователей
    for (const auto &a: remaining_clients) {
        sout << a << std::endl;
    }
    sout << club.get_close_time() << std::endl;
    const auto pc_info = club.get_pc_info(); // информация по заработку и времени работы столов
    for (int i = 0; i < pc_info.size(); ++i) {
        sout << i + 1 << " " << pc_info[i].first << " " << pc_info[i].second << std::endl;
    }
    std::cout << sout.str();
    return 0;
}
