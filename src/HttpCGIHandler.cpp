#include "HttpCGIHandler.h"

#include <Config.h>

#include "ProcessUtils.h"

HttpCGIHandler::HttpCGIHandler()
{
}

HttpCGIHandler::~HttpCGIHandler()
{
}

void HttpCGIHandler::HandleRequest(
    const std::shared_ptr<ClientSocket>& client_socket,
    const std::shared_ptr<HttpData>& http_data
    )
{
    // Get the path of the script
    std::string script_path = http_data->GetPath();

    // Handle the python CGI
    std::string response = HandlePythonCGI(script_path, http_data);

    // Send the response
    client_socket->send_http_response(response);
}

std::string HttpCGIHandler::HandlePythonCGI(const std::string& script_path, const std::shared_ptr<HttpData>& http_data)
{
    Config& config = Config::GetInstance();

    std::string script_dir = config.GetRootDir();

    // Get the file extension from the path
    std::string extension = script_path.substr(script_path.find_last_of('.') + 1);

    if(extension == "py")
    {
        // Execute the python script
        std::vector<std::string> arguments = {
            "python.exe",
            script_dir + script_path,
            http_data->GetQueryParamsString()
        };

        std::string output = exec(arguments, http_data->GetBody());

        return output;
    }

    return "";
}
