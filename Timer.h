#pragma once

#include <chrono>
#include <vector>

/***********************************************************
* A simple class to measure benchmark times
***********************************************************/
class Timer
{
public:
  using Clock = std::chrono::high_resolution_clock;
  using Second = std::chrono::duration<double, std::ratio<1> >;
  using Timepoint = std::chrono::time_point<Clock>;
  using Timevector = std::vector<Timepoint>;

  Timer() = default;

  // Getter
  const Timevector& times() const { return tv_; }

  // Measure time
  void count() { tv_.push_back( Clock::now() ); }

  // Get delta between timepoints in seconds
  double delta(int i) const
  {
    int sz = tv_.size();
    if ( i > (sz-2) || i < 0 )
      throw std::runtime_error("Invalid timer access");

    auto t1 = tv_[i+1];
    auto t0 = tv_[i];

    return std::chrono::duration_cast<Second>(t1-t0).count();
  }

private:

  Timevector tv_;


}; // Timer
