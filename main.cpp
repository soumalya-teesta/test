/*#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <random>
#include <iomanip>
#include <sstream>

const char* server_address = "fstream.binance.com";
const int server_port = 443;
std::string generateWebSocketKey() {
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<unsigned int> distribution(0, 255);

    // Generate a random 16-byte value
    std::string randomBytes;
    randomBytes.reserve(16);
    for (int i = 0; i < 16; ++i) {
        randomBytes.push_back(static_cast<char>(distribution(generator)));
    }

    // Base64 encode the random value
    std::ostringstream base64Key;
    base64Key.fill('0');
    base64Key << std::hex << std::setw(2) << std::setfill('0');
    for (unsigned char byte : randomBytes) {
        base64Key << '%' << std::setw(2) << static_cast<unsigned>(byte);
    }

    return base64Key.str();
}
int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, "52.197.149.233", &server_addr.sin_addr) <= 0) {
        std::cerr << "Error converting address" << std::endl;
        close(client_socket);
        return 1;
    }

    if (connect(client_socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        std::cerr << "Error connecting to the server" << std::endl;
        close(client_socket);
        return 1;
    }
    std::string key = generateWebSocketKey();
    std::cout<<key<<" = key\n";
    
    std::string request =
        "GET / HTTP/1.1\r\n"
        "Host: fstream.binance.com\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: "+ key + "\r\n"
        "Sec-WebSocket-Version: 13\r\n\r\n";

    send(client_socket, request.c_str(), request.size(), 0);

    
    char response[4096];
    ssize_t bytes_received = recv(client_socket, response, sizeof(response), 0);
    if (bytes_received == -1) {
        std::cerr << "Error receiving data" << std::endl;
    } else {
        std::cout << "Server response:\n" << std::string(response, bytes_received) << std::endl;
    }

    // Close the socket
    close(client_socket);

    return 0;
}*/





// #include <arpa/inet.h>
// #include <iostream>
// #include <stdio.h>
// #include <string.h>
// #include <string>
// #include <sys/socket.h>
// #include <unistd.h>
// #define PORT 443
// const char* stream_path = "btcusdt@depth";
// int main()
// {
// 	int status, valread, client_fd;
// 	struct sockaddr_in serv_addr;
	
// 	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
// 		printf("\n Socket creation error \n");
// 		return -1;
// 	}

// 	serv_addr.sin_family = AF_INET;
// 	serv_addr.sin_port = htons(PORT);

	
// 	if (inet_pton(AF_INET, "52.69.53.245", &serv_addr.sin_addr)
// 		<= 0) {
// 		printf("\nInvalid address/ Address not supported \n");
// 		return -1;
// 	}

// 	if ((status
// 		= connect(client_fd, (struct sockaddr*)&serv_addr,
// 				sizeof(serv_addr)))< 0) {
// 		printf("\nConnection Failed \n");
// 		return -1;
// 	}

//     std::string subscription_msg = "{\"method\":\"SUBSCRIBE\",\"params\":[\"" + std::string(stream_path) + "\"],\"id\":1}";
// 	if(send(client_fd, subscription_msg.c_str(), subscription_msg.size(), 0) == -1)
//     {
//         perror("Error sending subscription message");
//         close(client_fd);
//         return -1;
//     }

//     char buffer[4096];
//     memset(buffer, 0, sizeof(buffer));
//     while (true) {
//         if (recv(client_fd, buffer, sizeof(buffer), 0) == -1) {
//             perror("Error receiving data from server");
//             close(client_fd);
//             return -1;
//         }
//         std::cout << "Received data:\n" << buffer << std::endl;

//         memset(buffer, 0, sizeof(buffer));
//     }

	
// 	close(client_fd);
// 	return 0;
// }


/*#include "test.hpp"
int main()
{
    string coin = "bnbusdt";
    get_bin_partial_orderbook(coin);
    return 0;
}*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

int main() {
    // Set up the Asio I/O context
    asio::io_context io_context;

    // Set up SSL context (required for Binance WebSocket)
    ssl::context ctx(ssl::context::sslv23_client);

    // Set up resolver and connect to Binance WebSocket endpoint
    tcp::resolver resolver(io_context);
    auto const results = resolver.resolve("fstream.binance.com", "443");
    auto const endpoint = results.begin()->endpoint();

    // Set up SSL stream
    ssl::stream<tcp::socket> ws(io_context, ctx);
    ws.set_verify_mode(ssl::verify_none);  // Binance WebSocket doesn't need certificate verification

    // Connect to Binance WebSocket
    asio::connect(ws.next_layer(), results.begin(), results.end());
    ws.handshake(ssl::stream_base::client);

    // Construct and send WebSocket upgrade request
    ws.handshake("fstream.binance.com", "/ws/btcusdt@kline_1m");

    // Read WebSocket messages
    beast::flat_buffer buffer;
    beast::websocket::stream<ssl::stream<tcp::socket>> wss(io_context, ctx, ws.next_layer());
    wss.text(true);  // Assume text messages

    // Start asynchronous read
    wss.async_read(buffer, [](beast::error_code ec, std::size_t) {
        if (ec)
            std::cerr << "Error reading WebSocket: " << ec.message() << std::endl;
        else
            std::cout << "Received message: " << beast::make_printable(buffer.data()) << std::endl;
    });

    // Run the Asio event loop
    io_context.run();

    return 0;
}

