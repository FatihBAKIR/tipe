//
// Created by Mehmet Fatih BAKIR on 02/10/2018.
//

#pragma once

#include <chrono>
#include <tipe/flow_ctx.hpp>
#include <boost/asio.hpp>

namespace tip::nodes
{
    class timer
    {
    public:
        timer(boost::asio::io_service& io, std::chrono::milliseconds interval)
            : m_tmr(std::make_shared<boost::asio::high_resolution_timer>(io)), m_interval(interval) {
        }

        template <class GraphT, class NextT>
        void tip_start(GraphT& g, NextT&& start)
        {
            auto handler = [this, start = std::forward<NextT>(start)](const boost::system::error_code& ec) {
                this->handle(ec, std::move(start));
            };
            m_tmr->expires_from_now(m_interval);
            m_tmr->async_wait(handler);
        }

    private:
        template <class NextT>
        void handle(const boost::system::error_code& ec, NextT&& start)
        {
            if (ec)
            {
                std::cerr << ec.message() << '\n';
                return;
            }

            auto handler = [this, start = std::forward<NextT>(start)](const boost::system::error_code& ec) {
                this->handle(ec, std::move(start));
            };
            m_tmr->expires_from_now(m_interval);
            m_tmr->async_wait(handler);
            start(empty_ctx{});
        };

        std::shared_ptr<boost::asio::high_resolution_timer> m_tmr;
        std::chrono::milliseconds m_interval;
    };
}
