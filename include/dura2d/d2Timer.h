#ifndef D2TIMER_H
#define D2TIMER_H

#include <chrono>
#include <iostream>

class d2Timer
{
public:
    d2Timer()
    {
        start();
    }

    ~d2Timer()
    {
        stop();
        std::cout << "Duration: " << getDuration() << " ns" << std::endl;
    }

    void start()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    void stop()
    {
        auto end = std::chrono::high_resolution_clock::now();
        m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_start);
    }

    [[nodiscard]]
    long long getDuration() const
    {
        return m_duration.count();
    }

private:
    std::chrono::high_resolution_clock::time_point m_start { };
    std::chrono::nanoseconds m_duration { 0 };
};

#endif //D2TIMER_H