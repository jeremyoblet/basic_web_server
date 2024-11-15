#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#endif


#define BUFFER_SIZE (1024 * 128)
#include "HttpData.h"

class ClientSocket
{
public:
    ClientSocket(int socket);
    ~ClientSocket();

private:
#ifdef _WIN32
    SOCKET socket;
#else
    int socket;
#endif

    char buffer[BUFFER_SIZE];
    int in_cursor_position;
    int out_cursor_position;

    int expected_body_length;
    int received_data_count;

    int send_packet(void* packet, int packet_size) const;
    int recv_buffer();
    bool full_request_received();



public:
    std::shared_ptr<HttpData> read_request();
    int send_http_response(const std::string& response);
};
