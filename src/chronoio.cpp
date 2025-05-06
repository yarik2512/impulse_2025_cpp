#include "chronoio.h"

std::istream &operator>>(std::istream &in, std::chrono::time_point<std::chrono::system_clock> &time_point) {
    std::tm tm{};
    tm.tm_year = 70;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    in >> std::get_time(&tm, TIME_FMT.c_str());
    if (in.fail() || tm.tm_hour >= 24 || tm.tm_min >= 60) {
        throw std::runtime_error("Couldn't parse time");
    }
    time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return in;
}

std::ostream &operator<<(std::ostream &out, const std::chrono::time_point<std::chrono::system_clock> &tp) {
    const std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    out << std::put_time(std::localtime(&tt), TIME_FMT.c_str());
    return out;
}

std::ostream &operator<<(std::ostream &out, const std::chrono::minutes &mins) {
    out << std::format("{:%H:%M}", mins);
    return out;
}
