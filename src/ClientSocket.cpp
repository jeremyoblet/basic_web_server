//
// Created by huber on 24/05/2024.
//

#include "ClientSocket.h"

#include <string.h>
#include <stdio.h>
#include <chrono>
#include <Utils.h>

#include "Config.h"

ClientSocket::ClientSocket(int socket)
{
    this->socket = socket;
    memset(this->buffer, 0, BUFFER_SIZE);
    this->in_cursor_position = 0;
    this->out_cursor_position = 0;
    this->received_data_count = 0;
}

ClientSocket::~ClientSocket()
{
    if(this->socket)
    {
#ifdef _WIN32
        closesocket(this->socket);
#else
        close(this->socket);
#endif
    }
}

int ClientSocket::recv_buffer()
{
    if(!this->socket)
        return -1;

    if(this->out_cursor_position > 0)
    {
        int data_cursor_pos = this->in_cursor_position - this->out_cursor_position;
        if(data_cursor_pos > 0)
        {
            memmove(this->buffer, this->buffer + this->out_cursor_position, data_cursor_pos);
        }
        this->in_cursor_position -= this->out_cursor_position;
        this->out_cursor_position = 0;
    }

    int max_receive_size = BUFFER_SIZE - this->in_cursor_position;

    if(max_receive_size < 0)
        return 0;

    int data_count = recv(this->socket, this->buffer + this->in_cursor_position, max_receive_size, 0);

    if(data_count < 0)
    {
#ifdef _WIN32
        fprintf(stderr, "Error during receive : %d\n", WSAGetLastError());
#else
        fprintf(stderr, "Error during receive : %d\n", errno);
#endif
        return -1;
    }

    this->in_cursor_position += data_count;

    return data_count;
}

bool ClientSocket::full_request_received()
{
    char* end_of_headers = strstr(this->buffer, "\r\n\r\n");

    if(end_of_headers == nullptr)
        return false;

    int headers_size = end_of_headers - this->buffer;

    std::string headers(this->buffer, end_of_headers - this->buffer);

    auto pos = headers.find("Content-Length: ");

    if(pos == std::string::npos)
    {
        this->expected_body_length = 0;
        return true;
    }

    pos += strlen("Content-Length: ");
    auto end_pos = headers.find("\r\n", pos);
    std::string content_length_str = headers.substr(pos, end_pos - pos);
    this->expected_body_length = std::stoi(content_length_str);

    if(received_data_count >= expected_body_length + headers_size + 4 /* 4 is for \r\n\r\n */)
    {
        return true;
    }

    return false;
}

int ClientSocket::send_http_response(const std::string& response)
{
    return send_packet((void*)response.c_str(), response.size());
}

int ClientSocket::send_packet(void* packet, int packet_size) const
{
    if(!this->socket)
        return -1;

    int data_count = send(this->socket, static_cast<const char*>(packet), packet_size, 0);

    if(data_count < packet_size)
    {
#ifdef _WIN32
        fprintf(stderr, "Error during send_packet : %d\n", WSAGetLastError());
#else
        fprintf(stderr, "Error during send packet : %d\n", errno);
#endif
    }

    return data_count;
}

std::shared_ptr<HttpData> ClientSocket::read_request()
{
    if(!this->socket)
        return nullptr;

    Config& config = Config::GetInstance();

    auto start = TimestampNow();

    while(!full_request_received() || expected_body_length == -1)
    {
        int data_count = this->recv_buffer();
        received_data_count += data_count;

        if(data_count == 0 && TimestampNow() - start > config.GetTimeoutDelay())
        {
            return nullptr;
        }
    }
	
	fprintf(stderr, "%s", this->buffer);

    auto httpData = HttpData::fromString(this->buffer);

    return httpData;
}