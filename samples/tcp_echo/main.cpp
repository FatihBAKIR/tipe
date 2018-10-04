#include <iostream>
#include <tipe/tipe.hpp>
#include <tipe/nodes/tcp.hpp>
#include <tipe/nodes/msgpack_to_json.hpp>
#include <sys/utsname.h>

int main()
{
    auto loop = uvw::Loop::getDefault();

    auto nodes = tip::make_nodes(
        tip::nodes::tcp_server(loop, "localhost", 4242),
        tip::nodes::tcp_server(loop, "localhost", 8080),
        [](tos::span<const char> req) -> std::string {
            std::string res = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/plain; charset=utf-8\r\n\r\n"
                              "hello there\r\n\r\n";

            utsname unameData;

            uname(&unameData);
            res += std::string(unameData.sysname) + "\r\n";
            res += std::string(unameData.nodename) + "\r\n";
            res += std::string(unameData.release) + "\r\n";
            res += std::string(unameData.version) + "\r\n";
            res += std::string(unameData.machine) + "\r\n\r\n";

            res += std::string(req.begin(), req.end()) + "\r\n";

            return res;
        },
        tip::nodes::tcp_response {}
    );

    auto ids = tip::get_ids(nodes);
    auto& [tcp, http_server, process_http, echo] = ids;

    auto edges = tip::make_edges(ids)
            .connect(tcp | echo)
            .connect(http_server | process_http | echo);

    auto g = tip::make_graph(std::move(nodes), edges);

    tip::start_roots(g);
    loop->run<uvw::Loop::Mode::DEFAULT>();
}
