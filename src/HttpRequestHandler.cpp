#include "ClientSocket.h"
#include "Config.h"
#include "HttpRequestHandler.h"
#include "HttpGetHandler.h"
#include "HttpHeadHandler.h"
#include "HttpPostHandler.h"

#include <memory>


void HttpRequestHandler::HandleRequest(int client_socket) {
    auto client = std::make_shared<ClientSocket>(client_socket);
    auto httpData = client->read_request();

    Config& config = Config::GetInstance();

    if(httpData == nullptr) {
        Send408Response(client);
        return;
    }

    if(httpData->GetPath().find("/../") != std::string::npos) {
        Send409Response(client);
        return;
    }


    switch(httpData->GetMethod()) {
    case HttpMethod::HTTP_METHOD_GET:
        handle_get(client, httpData);
        break;
    case HttpMethod::HTTP_METHOD_POST:
        handle_post(client, httpData);
        break;
    case HttpMethod::HTTP_METHOD_PUT:
        handle_put(client, httpData);
        break;
    case HttpMethod::HTTP_METHOD_DELETE:
        handle_delete(client, httpData);
        break;
    case HttpMethod::HTTP_METHOD_HEAD:
        handle_head(client, httpData);
        break;
    case HttpMethod::HTTP_METHOD_UNKNOWN:
        Send404Response(client);
        break;
    }
}

void HttpRequestHandler::handle_get(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data) {
    HttpGetHandler handler;
    handler.HandleRequest(client_socket, http_data);
}

void HttpRequestHandler::handle_post(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data) {
    HttpPostHandler handler;
    handler.HandleRequest(client_socket, http_data);
}

void HttpRequestHandler::handle_put(std::shared_ptr<ClientSocket> c, const std::shared_ptr<HttpData>&) {
    Send405Response(c);
}
void HttpRequestHandler::handle_delete(std::shared_ptr<ClientSocket> c, const std::shared_ptr<HttpData>&) {
    Send405Response(c);
}

void HttpRequestHandler::handle_head(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data) {
    HttpHeadHandler handler;
    handler.HandleRequest(client_socket, http_data);
}

void HttpRequestHandler::Send400Response(std::shared_ptr<ClientSocket> client_socket) {
    const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\n\r\nBad Request";
    client_socket->send_http_response(response);
}

void HttpRequestHandler::Send404Response(std::shared_ptr<ClientSocket> client_socket) {
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
    client_socket->send_http_response(response);
}

void HttpRequestHandler::Send405Response(std::shared_ptr<ClientSocket> c) {
    const char* r =
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Allow: GET, HEAD, POST\r\n"
        "Content-Length: 18\r\n"
        "\r\nMethod Not Allowed";
    c->send_http_response(r);
}

void HttpRequestHandler::Send408Response(std::shared_ptr<ClientSocket> client_socket) {
    const char* response = "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\nContent-Length: 14\r\n\r\nRequest Timeout";
    client_socket->send_http_response(response);
}

void HttpRequestHandler::Send409Response(std::shared_ptr<ClientSocket> client_socket) {
    const char* response = "HTTP/1.1 409 Conflict\r\nContent-Length: 8\r\n\r\nConflict";
    client_socket->send_http_response(response);
}

void HttpRequestHandler::Send500Response(std::shared_ptr<ClientSocket> client_socket) {
    const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 21\r\n\r\nInternal Server Error";
    client_socket->send_http_response(response);
}
