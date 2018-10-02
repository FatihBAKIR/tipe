//
// Created by Mehmet Fatih BAKIR on 02/10/2018.
//

#pragma once

#include <uvw/loop.hpp>
#include <uvw.hpp>
#include <tos/span.hpp>

namespace tip::nodes
{
    class tcp_server
    {
    public:
        tcp_server(const std::shared_ptr<uvw::Loop>& l, const std::string& host, uint16_t port)
            : m_handle(l->resource<uvw::TcpHandle>()) {
            m_handle->bind(host, port);
        }

        template <class GraphT, class NextT>
        void tip_start(GraphT& g, NextT&& next)
        {
            m_handle->on<uvw::ListenEvent>([next](const uvw::ListenEvent &, uvw::TcpHandle &srv) {
                std::shared_ptr<uvw::TcpHandle> client = srv.loop().resource<uvw::TcpHandle>();
                client->once<uvw::EndEvent>([](const uvw::EndEvent &, uvw::TcpHandle &client) {
                    client.close();
                });
                client->on<uvw::DataEvent>([next](const uvw::DataEvent & ev, uvw::TcpHandle &) {
                    next(tos::span<const char>(ev.data.get(), ev.length));
                });
                srv.accept(*client);
                client->read();
            });
            m_handle->listen();
        }

        void tip_destroy()
        {
            m_handle.reset();
        }

    private:
        std::shared_ptr<uvw::TcpHandle> m_handle;
    };
}
