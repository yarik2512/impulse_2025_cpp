#include "club.h"

Club::time_point Club::get_open_time() const {
    return open_time;
}

Club::time_point Club::get_close_time() const {
    return close_time;
}

void Club::process_action(const Action &action) {
    switch (action.id) {
        case 1:
            if (clients.contains(action.client)) {
                throw Action(action.time, 13, "YouShallNotPass");
            }
            if (action.time < open_time || action.time > close_time) {
                throw Action(action.time, 13, "NotOpenYet");
            }
            clients[action.client] = -1;
            break;
        case 2: {
            if (pc_start[action.pc] != time_point()) {
                throw Action(action.time, 13, "PlaceIsBusy");
            }
            const auto client_pc = clients.find(action.client);
            if (client_pc == clients.end()) {
                throw Action(action.time, 13, "ClientUnknown");
            }
            const int prev_pc = client_pc->second;
            if (prev_pc != -1) {
                income[prev_pc] += std::chrono::ceil<hours>(action.time - pc_start[prev_pc]).count() * cost;
                up_time[prev_pc] += std::chrono::ceil<mins>(action.time - pc_start[prev_pc]);
                pc_start[client_pc->second] = time_point();
                ++free_pcs;
            }
            client_pc->second = action.pc;
            pc_start[action.pc] = action.time;
            --free_pcs;
            break;
        }
        case 3:
            if (free_pcs > 0) {
                throw Action(action.time, 13, "ICanWaitNoLonger");
            }
            if (clients_queue.size() > n_pcs) {
                throw Action(action.time, 11, action.client);
            }
            clients_queue.push(action.client);
            break;
        case 4: {
            const auto client_pc = clients.find(action.client);
            if (client_pc == clients.end()) {
                throw Action(action.time, 13, "ClientUnknown");
            }
            const int pc = client_pc->second;
            clients.erase(client_pc);
            income[pc] += std::chrono::ceil<hours>(action.time - pc_start[pc]).count() * cost;
            up_time[pc] += std::chrono::ceil<mins>(action.time - pc_start[pc]);
            if (clients_queue.empty()) {
                pc_start[pc] = time_point();
                break;
            }
            pc_start[pc] = action.time;
            const std::string client = clients_queue.front();
            clients_queue.pop();
            clients[client] = pc;
            throw Action(action.time, 12, client, pc);
        }
        default:
            break;
    }
}

std::vector<Action> Club::process_remaining_clients() {
    std::vector<Action> remaining_clients;
    for (const auto &[client, pc]: clients) {
        remaining_clients.emplace_back(close_time, 11, client);
        if (pc == -1) continue;
        income[pc] += std::chrono::ceil<hours>(close_time - pc_start[pc]).count() * cost;
        up_time[pc] += std::chrono::ceil<mins>(close_time - pc_start[pc]);
        pc_start[pc] = time_point();
    }
    return remaining_clients;
}

std::vector<std::pair<unsigned long long, Club::mins> > Club::get_pc_info() const {
    std::vector<std::pair<unsigned long long, mins> > pc_info(n_pcs);
    for (int i = 0; i < n_pcs; ++i) {
        pc_info[i] = {income[i], up_time[i]};
    }
    return pc_info;
}

int Club::get_n_pcs() const {
    return n_pcs;
}


std::istream &operator>>(std::istream &in, Club &club) {
    std::string input;
    std::getline(in, input);
    std::stringstream ss(input);
    ss >> club.n_pcs;
    if (ss.fail()) {
        throw std::runtime_error(input);
    }
    std::getline(in, input);
    ss = std::stringstream(input);
    try {
        ss >> club.open_time >> club.close_time;
    } catch (const std::runtime_error &) {
        throw std::runtime_error(input);
    }
    if (ss.fail()) {
        throw std::runtime_error(input);
    }
    std::getline(in, input);
    ss = std::stringstream(input);
    ss >> club.cost;
    club.free_pcs = club.n_pcs;
    club.pc_start.resize(club.n_pcs);
    club.income.resize(club.n_pcs);
    club.up_time.resize(club.n_pcs);
    return in;
}
