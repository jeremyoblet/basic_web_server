#include "HttpDeleteHandler.h"
#include <fstream>
#include <cstdio>
#include "Config.h"

HttpDeleteHandler::HttpDeleteHandler()
{
}

HttpDeleteHandler::~HttpDeleteHandler()
{
}

void HttpDeleteHandler::HandleRequest(const std::shared_ptr<ClientSocket>& client_socket,
    const std::shared_ptr<HttpData>& http_data)
{
    Config& Config = Config::GetInstance();
    std::string file_path_with_root = Config.GetRootDir() + http_data->GetPath();
    std::ifstream file(file_path_with_root);
    if (file.good())
    {
        file.close();
        if (remove(file_path_with_root.c_str()) != 0)
        {
            Send500Response(client_socket);
        }
    }
    else
    {
        Send404Response(client_socket);
    }

    HttpData response;
    response.SetStatusCode(200);

    client_socket->send_http_response(response.to_string());
}