// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_COSYMANAGER_H_
#define INCLUDE_COSY_COSYMANAGER_H_

#include <memory>
#include <vector>

#include "cosy/Assignment.h"
#include "cosy/CosyStatus.h"
#include "cosy/Group.h"
#include "cosy/Logging.h"
#include "cosy/Order.h"

namespace cosy {

class CosyManager {
 public:
    CosyManager(const Group& group, const Assignment& assignment);
    ~CosyManager();

    void defineOrder(std::unique_ptr<Order>&& order);

    void generateUnits(ClauseInjector *injector);
    void updateNotify(const Literal& literal, ClauseInjector *injector);
    void updateCancel(const Literal& literal);

    void summarize() const;
    void printStats() const { _stats.print(); }

 private:
    const Group& _group;
    const Assignment& _assignment;
    std::unique_ptr<Order> _order;

    std::vector< std::unique_ptr<CosyStatus> > _statuses;

    struct Stats : public StatsGroup {
        Stats() : StatsGroup("Cosy Manager"),
                  total_time("Cosy total time", this),
                  notify_time(" |- notify time", this),
                  cancel_time(" |- cancel time", this)
        {}
        TimeDistribution total_time;
        TimeDistribution notify_time;
        TimeDistribution cancel_time;
    };
    Stats _stats;

    DISALLOW_COPY_AND_ASSIGN(CosyManager);
};

}  // namespace cosy

#endif  // INCLUDE_COSY_COSYMANAGER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
