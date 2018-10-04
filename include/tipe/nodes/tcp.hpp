//
// Created by Mehmet Fatih BAKIR on 02/10/2018.
//

#pragma once

#include <uvw/loop.hpp>
#include <uvw.hpp>
#include <tos/span.hpp>
#include <tipe/flow_ctx.hpp>

namespace tip::nodes
{
    struct tcp_ctx : ctx<tcp_ctx>
    {
        std::shared_ptr<uvw::TcpHandle> m_client;
    };

    class tcp_server
    {
    public:
        tcp_server(const std::shared_ptr<uvw::Loop>& l, const std::string& host, uint16_t port)
            : m_handle(l->resource<uvw::TcpHandle>()) {
            m_handle->bind(host, port);
        }

        template <class GraphT, class NextT>
        void tip_start(GraphT& g, NextT&& start)
        {
            m_handle->on<uvw::ListenEvent>([start](const uvw::ListenEvent &, uvw::TcpHandle &srv) {
                std::shared_ptr<uvw::TcpHandle> client = srv.loop().resource<uvw::TcpHandle>();
                client->once<uvw::EndEvent>([](const uvw::EndEvent &, uvw::TcpHandle &client) {
                    client.close();
                });
                client->on<uvw::DataEvent>([start](const uvw::DataEvent & ev, uvw::TcpHandle & client) {
                    start(tcp_ctx{ {}, client.shared_from_this() }, tos::span<const char>(ev.data.get(), ev.length));
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

    class tcp_response
    {
    public:
        template <class T>
        void operator()(const tcp_ctx& ctx, const T& t) const
        {
            auto data = std::make_unique<char[]>(t.size());
            std::copy(t.begin(), t.end(), data.get());
            ctx.m_client->write(std::move(data), t.size());
            ctx.m_client->close();
        }
    };
}
