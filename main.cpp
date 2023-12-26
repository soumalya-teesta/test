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


#include "test.hpp"
int main()
{
    string coin = "bnbusdt";
    get_bin_partial_orderbook(coin);
    return 0;
}


