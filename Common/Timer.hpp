#include <iostream>
#include <chrono>

class Timer {
  public:
    Timer() noexcept { m_start = std::chrono::steady_clock::now(); }
    ~Timer() noexcept {
        const auto end = std::chrono::steady_clock::now();
        const auto diff = std::chrono::duration<double, std::milli>(end - m_start).count();
        std::clog << "\nTime taken: " << diff << " ms.\n";
    }

  private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point m_start;
};
