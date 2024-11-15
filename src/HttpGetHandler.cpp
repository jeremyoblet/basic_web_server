#include "HttpGetHandler.h"
#include <fstream>
#include <iostream>
#include "CacheManager.h"
#include "Utils.h"
#include "Config.h"

HttpGetHandler::HttpGetHandler()
{
}

HttpGetHandler::~HttpGetHandler()
{
}

void HttpGetHandler::HandleRequest(const std::shared_ptr<ClientSocket>& client_socket, const std::shared_ptr<HttpData>& http_data)
{
    std::string path = http_data->GetPath();
    if(path == "/")
    {
        path = "/index.html";
    }

    SendFileResponse(client_socket, path);
}

void HttpGetHandler::SendFileResponse(std::shared_ptr<ClientSocket> client_socket, const std::string& path)
{
    Config& config = Config::GetInstance();
    std::string file_path_with_root = config.GetRootDir() + path;

    std::string content_type = GetContentTypeFromRequest(path);

    CacheManager& cache_manager = CacheManager::GetInstance();
    std::string file_data = cache_manager.GetFile(file_path_with_root);
	
	std::cout << file_path_with_root.c_str() << std::endl;

    if(file_data.empty())
    {
        Send404Response(client_socket);
        return;
    }

    HttpData response_data;
    response_data.SetBody(file_data);
    response_data.SetContentType(content_type);
    response_data.SetStatusCode(200);

    client_socket->send_http_response(response_data.to_string());
}