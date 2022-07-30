// Copyright 2017 Hakan Metin - LIP6

#include "cosy/Stats.h"

namespace cosy {

Stat::Stat(const std::string& name) : _name(name) {
}

Stat::Stat(const std::string& name, StatsGroup *group) : _name(name) {
    group->registerStat(this);
}

void StatsGroup::print(bool section /* = false */) const {
    if (section)
        Printer::printSection(_name.c_str());
    for (const Stat* stat : _stats)
        stat->print();
}




DistributionStat::DistributionStat(const std::string& name) :
    Stat(name),
    _sum(0.0),
    _average(0.0),
    _sum_squares_from_average(0.0),
    _min(0.0),
    _max(0.0),
    _num(0) {}


DistributionStat::DistributionStat(const std::string& name, StatsGroup *group) :
    Stat(name, group),
    _sum(0.0),
    _average(0.0),
    _sum_squares_from_average(0.0),
    _min(0.0),
    _max(0.0),
    _num(0) {}

void DistributionStat::addToDistribution(double value) {
    if (_num == 0) {
        _min = value;
        _max = value;
        _sum = value;
        _average = value;
        _num = 1;
        return;
    }
    _min = std::min(_min, value);
    _max = std::max(_max, value);
    _sum += value;
    ++_num;
    const double delta = value - _average;
    _average = _sum / _num;
    _sum_squares_from_average += delta * (value - _average);
}

double DistributionStat::average() const {
    return _average;
}
double DistributionStat::stdDeviation() const {
    return sqrt(_sum_squares_from_average / _num);
}

std::string TimeDistribution::valueString() const {
    std::stringstream stream;

    stream << std::fixed << std::setprecision(2)  << _num <<
        " [" << min() << ", " << max() << "] "
           << average() << " " << stdDeviation() << " "  << sum();
    return stream.str();
}


}  // namespace cosy
