// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_STATS_H_
#define INCLUDE_COSY_STATS_H_

#include <cmath>
#include <utility>
#include <deque>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

#include "cosy/IntegralTypes.h"
#include "cosy/Logging.h"
#include "cosy/Macros.h"
#include "cosy/Printer.h"
#include "cosy/Timer.h"

namespace cosy {

// Forward Declaration
class StatsGroup;
class TimeDistribution;

class Stat {
 public:
    explicit Stat(const std::string& name);
    Stat(const std::string& name, StatsGroup *group);
    virtual ~Stat() {}

    std::string name() const { return _name;}
    virtual std::string valueString() const = 0;

    void print() const {
        Printer::printStat(_name, valueString());
    }
 private:
    std::string _name;
};


class StatsGroup {
 public:
    explicit StatsGroup(const std::string& name) : _name(name), _stats() {}
    ~StatsGroup() {}

    void registerStat(Stat *stat) { _stats.push_back(stat); }
    void reset();

    void print(bool section = false) const;
 private:
    std::string _name;
    std::vector<Stat*> _stats;

    DISALLOW_COPY_AND_ASSIGN(StatsGroup);
};

/*----------------------------------------------------------------------------*/
class DistributionStat : public Stat {
 public:
    explicit DistributionStat(const std::string& name);
    DistributionStat(const std::string& name, StatsGroup *group);
    ~DistributionStat() override {}

    std::string valueString() const override = 0;

    double sum() const { return _sum; }
    double max() const { return _max; }
    double min() const { return _min; }
    int64  num() const { return _num; }
    double average()      const;
    double stdDeviation() const;
 protected:
    void addToDistribution(double value);
    double _sum;
    double _average;
    double _sum_squares_from_average;
    double _min;
    double _max;
    int64 _num;
};

/*----------------------------------------------------------------------------*/
class TimeDistribution : public DistributionStat {
 public:
    explicit TimeDistribution(const std::string& name)
        : DistributionStat(name), _timer() {}
    TimeDistribution(const std::string& name, StatsGroup* group)
        : DistributionStat(name, group), _timer() {}

    void startTimer() { _timer.restart(); }
    double stopTimerAndAddElapsedTime() {
        _timer.stop();
        const double elapsed = _timer.time();
        addToDistribution(elapsed);
        return elapsed;
    }
    void addTimeInSeconds(double value) {
        addToDistribution(value);
    }
    std::string valueString() const override;
 private:
    Timer _timer;
};

class EnableScopedTimeDistributionUpdater {
 public:
    explicit EnableScopedTimeDistributionUpdater(TimeDistribution *stat) :
        _stat(stat),
        _also(nullptr) {
        _stat->startTimer();
    }
    virtual ~EnableScopedTimeDistributionUpdater() {
        const double elapsed = _stat->stopTimerAndAddElapsedTime();
        if (_also != nullptr)
            _also->addTimeInSeconds(elapsed);
    }

    void alsoUpdate(TimeDistribution *also) { _also = also; }
 private:
    TimeDistribution *_stat;
    TimeDistribution *_also;
    DISALLOW_COPY_AND_ASSIGN(EnableScopedTimeDistributionUpdater);
};

class DisableScopedTimeDistributionUpdater {
 public:
    explicit DisableScopedTimeDistributionUpdater(TimeDistribution *stat) {
        UNUSED_PARAMETER(stat);
    }
    ~DisableScopedTimeDistributionUpdater() {}
    void alsoUpdate(TimeDistribution *also) { UNUSED_PARAMETER(also); }
 private:
    DISALLOW_COPY_AND_ASSIGN(DisableScopedTimeDistributionUpdater);
};

/*----------------------------------------------------------------------------*/
class CounterStat : public Stat {
 public:
    explicit CounterStat(const std::string& name) : Stat(name), _value(0) {}
    CounterStat(const std::string& name, StatsGroup *group) :
        Stat(name, group), _value(0) {}
    ~CounterStat() {}

    void increment() { _value++; }
    virtual std::string valueString() const { return std::to_string(_value); }
 private:
    int64 _value;
};

#ifdef COSY_STATS
using  ScopedTimeDistributionUpdater = EnableScopedTimeDistributionUpdater;
#define IF_STATS_ENABLED(instructions) instructions
#else
using  ScopedTimeDistributionUpdater = DisableScopedTimeDistributionUpdater;
#define IF_STATS_ENABLED(instructions)
#endif

#define SCOPED_TIME_STAT(stat) \
    EnableScopedTimeDistributionUpdater scoped_time_stat(stat)

}  // namespace cosy

#endif  // INCLUDE_COSY_STATS_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
