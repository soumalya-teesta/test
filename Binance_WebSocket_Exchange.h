
#ifndef binance_websocket_class_websocket
#define binance_websocket_class_websocket

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <thread>
#include <mutex>
#include <queue>

namespace net       = boost::asio;
namespace ssl       = net::ssl;
namespace beast     = boost::beast;
namespace http      = beast::http;
namespace websocket = beast::websocket;

using tcp      = net::ip::tcp;
using Request  = http::request<http::string_body>;
using Stream   = beast::ssl_stream<beast::tcp_stream>;
using Response = http::response<http::dynamic_body>;
using namespace std;

class Binance_Websocket_Exchange {
public:
    Binance_Websocket_Exchange(std::string name, const std::string& http_host)
        : m_name(std::move(name))
    {
        init_http(http_host);
    }

    void init_http(std::string const& host)
    {
        const auto results{resolver.resolve(host, "443")};
        get_lowest_layer(stream).connect(results);
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            boost::system::error_code ec{
                static_cast<int>(::ERR_get_error()),
                boost::asio::error::get_ssl_category()};
            throw boost::system::system_error{ec};
        }
        stream.handshake(ssl::stream_base::client);
    }

    void init_webSocket(std::string const& host, std::string const& port,const char* p = "")
    {
        std::lock_guard<std::mutex> lock(binance_ws_mutex);
        ssl::context ctx_webSocket{ssl::context::tlsv12_client};
        auto const results = resolver_webSocket.resolve(host, port);
        net::connect(ws.next_layer().next_layer(), results.begin(),results.end());

        // Set SNI hostname
        if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(),host.c_str())) {
            boost::system::error_code ec{
                static_cast<int>(::ERR_get_error()),
                boost::asio::error::get_ssl_category()
            };
            throw boost::system::system_error{ec};
        }

        ws.next_layer().handshake(ssl::stream_base::client);
        ws.handshake(host, p);
    }

    void read_Socket() {
        
        std::lock_guard<std::mutex> lock(binance_ws_mutex);
        ws.read(buffer);

        // Push the received data into the queue
        std::string receivedData = beast::buffers_to_string(buffer.data());
        dataQueue.push(receivedData);

        // Clear the buffer for the next read
        buffer.clear();
    }

    inline bool has_data() {
        std::lock_guard<std::mutex> lock(binance_ws_mutex);
        return !dataQueue.empty();
    }

    inline std::string get_socket_data() {
        std::lock_guard<std::mutex> lock(binance_ws_mutex);
        if (!dataQueue.empty()) {
            std::string data = dataQueue.front();
            dataQueue.pop();
            return data;
        }
        return "";
    }

    inline bool is_socket_open() {
        std::lock_guard<std::mutex> lock(binance_ws_mutex);
        return ws.is_open();
    }

    inline void write_Socket(const std::string& text) {
        std::lock_guard<std::mutex> lock(binance_ws_mutex);
        ws.write(net::buffer(text));
    }

    inline void buffer_clear() {
        return;
    }

    inline void webSocket_close() {
        std::lock_guard<std::mutex> lock(binance_ws_mutex);
        ws.close(websocket::close_code::none);
    }

private:
    // HTTP REQUEST SET //
    std::string     m_name;
    net::io_context ioc;
    ssl::context    ctx{ssl::context::tlsv12_client};
    tcp::resolver   resolver{ioc};
    Stream          stream{ioc, ctx};

    // WEB SOCKET SET //
    std::string        m_web_socket_host;
    std::string        m_web_socket_port;
    beast::flat_buffer buffer;
    net::io_context    ioc_webSocket;
    ssl::context       ctx_webSocket{ssl::context::tlsv12_client};
    tcp::resolver      resolver_webSocket{ioc_webSocket};
    websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc_webSocket,ctx_webSocket};

    // Queue to store received JSON messages
    std::queue<std::string> dataQueue;

    // Mutex for synchronization
    std::mutex binance_ws_mutex;
};

#endif
