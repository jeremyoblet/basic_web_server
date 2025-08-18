// #include "HttpHeadHandler.h"
// #include "Config.h"
// #include "Utils.h"

// #include <fstream>


// void HttpHeadHandler::HandleRequest(
//     const std::shared_ptr<ClientSocket>& client_socket,
//     const std::shared_ptr<HttpData>& http_data)
// {
//     Config& Config = Config::GetInstance();
//     std::string file_path_with_root = Config.GetRootDir() + http_data->GetPath();

//     std::string path = http_data->GetPath();
//     if(path == "/")
//     {
//         path = "/index.html";
//     }

//     std::ifstream file(file_path_with_root, std::ios::binary | std::ios::ate);
//     if(!file.is_open())
//     {
//         Send404Response(client_socket);
//         return;
//     }

//     int size = file.tellg();
//     std::string content_type = GetContentTypeFromRequest(path);

//     HttpData response_data;
//     response_data.SetStatusCode(200);
//     response_data.AddHeader("Content-Length", std::to_string(size));
//     response_data.SetContentType(content_type);

//     client_socket->send_http_response(response_data.to_string());
// }


#include "HttpHeadHandler.h"
#include "Config.h"
#include "CacheManager.h"
#include "Utils.h"

void HttpHeadHandler::HandleRequest(
    const std::shared_ptr<ClientSocket>& client,
    const std::shared_ptr<HttpData>& req)
{
    std::string path = req->GetPath();
    if (path == "/") path = "/index.html";

    auto& cfg   = Config::GetInstance();
    const std::string abs = cfg.GetRootDir() + path;

    auto& cache = CacheManager::GetInstance();
    const std::string data = cache.GetFile(abs);
    if (data.empty()) { Send404Response(client); return; }

    const std::string ctype = GetContentTypeFromRequest(path);
    // Réponse HEAD: en-têtes uniquement, pas de corps
    std::string hdr =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " + ctype + "\r\n"
        "Content-Length: " + std::to_string(data.size()) + "\r\n"
        "\r\n";
    client->send_http_response(hdr);
}
