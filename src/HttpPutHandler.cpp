#include "HttpPutHandler.h"
#include "Config.h"

#include <fstream>


void HttpPutHandler::HandleRequest(const std::shared_ptr<ClientSocket>& client_socket,
    const std::shared_ptr<HttpData>& http_data)
{
    Config& Config = Config::GetInstance();
    std::string file_path_with_root = Config.GetRootDir() + http_data->GetPath();
    std::ofstream file(file_path_with_root, std::ios::binary | std::ios::trunc);

    if (file.is_open()) {
        file.write(http_data->GetBody().c_str(), http_data->GetBody().size());
        file.close();
    }
    else {
        Send500Response(client_socket);
    }

    HttpData response;
    response.SetStatusCode(200);

    client_socket->send_http_response(response.to_string());
}