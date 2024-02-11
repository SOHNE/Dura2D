#ifndef D2TIMER_H
#define D2TIMER_H

#include <chrono>
#include <iostream>

class d2Timer
{
public:
    d2Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~d2Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
    }

private:
    std::chrono::high_resolution_clock::time_point start;
};

#endif //D2TIMER_H
