#ifndef SRC_HUBBARD_APPLICATION_TIMER_HPP_
#define SRC_HUBBARD_APPLICATION_TIMER_HPP_

#include <chrono>
#include <ostream>

class ApplicationTimer
{
    friend std::ostream& operator<<(std::ostream& /*strm*/, const ApplicationTimer& /*timer*/);

private:
    const std::chrono::steady_clock::time_point _created;

public:
    ApplicationTimer() : _created(std::chrono::steady_clock::now()){};
    ApplicationTimer(const ApplicationTimer&) = delete;
    ApplicationTimer& operator=(const ApplicationTimer&) = delete;
};

std::ostream& operator<<(std::ostream& strm, const ApplicationTimer& timer)
{
    const auto now = std::chrono::steady_clock::now();
    strm << std::chrono::duration<double>(now - timer._created).count() << "s";
    return strm;
}

#endif
