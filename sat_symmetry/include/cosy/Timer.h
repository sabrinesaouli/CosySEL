// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_TIMER_H_
#define INCLUDE_COSY_TIMER_H_

#include <chrono>

namespace cosy {

class Timer {
 public:
    Timer() {}
    ~Timer() {}

    void start() { _start = std::chrono::system_clock::now(); }
    void stop()  { _sum += std::chrono::system_clock::now() - _start; }
    void restart() {
        _sum = std::chrono::nanoseconds::zero();
        start();
    }

    double time() const { return _sum.count(); }

 private:
    std::chrono::time_point<std::chrono::system_clock> _start;
    std::chrono::duration<double> _sum;
};

}  // namespace cosy

#endif  // INCLUDE_COSY_TIMER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
