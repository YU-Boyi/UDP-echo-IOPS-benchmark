/*  Author: YU Boyi (yuboyi@moborobotics.com)
 *  Date Created:  2023/12/12, 18:23 PM
 *  Date Modified: 2023/12/13, 15:06 PM
 */
# include <asio.hpp>
//
# include <cstddef>
# include <thread>
# include <chrono>
# include <iostream>
# include <fstream>
# include <string>

//
//  Configuration
//
bool server_or_client = true;
std::string server_ip = "127.0.0.1";
auto server_port = 10024;
auto client_port = 10025;
//
constexpr auto msg_length = 8;
auto test_time_sec = 10;

//
//  Implementation
//
using udp = asio::ip::udp;

void as_server(void) {
    asio::io_context io_context;
    udp::socket server_sock(io_context, udp::endpoint(udp::v4(), server_port));
    char server_msg_buf[msg_length];
    udp::endpoint endpoint;
    //
    while (true) {
        size_t length = server_sock.receive_from(
            asio::buffer(server_msg_buf, msg_length), endpoint);
        server_sock.send_to(asio::buffer(server_msg_buf, length), endpoint);
    }
}

void as_client(void) {
    asio::io_context io_context;
    udp::socket client_sock(io_context, udp::endpoint(udp::v4(), client_port));
    udp::endpoint endpoint(asio::ip::address_v4::from_string(server_ip), server_port);
    char client_msg_buf[msg_length];
    //
    unsigned io_count = 0;
    auto time0 = std::chrono::steady_clock::now();
    //
    while (true) {
        client_sock.send_to(asio::buffer(client_msg_buf, msg_length), endpoint);
        size_t length = client_sock.receive_from(
            asio::buffer(client_msg_buf, msg_length), endpoint);
        //
        ++io_count;
        auto time1 = std::chrono::steady_clock::now();
        auto total_time = std::chrono::duration_cast<std::chrono::seconds>(time1 - time0).count();;
        if (total_time > test_time_sec) {
            std::cout << 1. * io_count / total_time << std::endl;
            break;
        }
    }
}

void try_read_config_file(void) {
    std::ifstream file;
	std::string file_path = server_or_client ?
        "./server_config.txt" : "./client_config.txt";
    file.open(file_path);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        server_ip = line;
        std::getline(file, line);
        if (server_or_client)
            server_port = std::stoul(line);
        else
            client_port = std::stoul(line);
    }
    file.close();
}

int main(void) {
    try_read_config_file();
    if (server_or_client)
        as_server();
    else
        as_client();

    return 0;
}
