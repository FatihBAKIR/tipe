//
// Created by Mehmet Fatih BAKIR on 02/10/2018.
//

#pragma once

#include <uvw.hpp>
#include <chrono>

namespace tip::nodes
{
    class timer
    {
    public:
        timer(const std::shared_ptr<uvw::Loop>& loop, std::chrono::milliseconds interval)
            : m_tmr(loop->resource<uvw::TimerHandle>()),
              m_interval(interval)
        {
        }

        template <class GraphT, class NextT>
        void tip_start(GraphT& g, NextT&& next)
        {
            m_tmr->on<uvw::TimerEvent>([next](const uvw::TimerEvent& e, uvw::TimerHandle&) {
                next();
            });
            m_tmr->start(m_interval, m_interval);
        }

    private:
        std::chrono::milliseconds m_interval;
        std::shared_ptr<uvw::TimerHandle> m_tmr;
    };
}
