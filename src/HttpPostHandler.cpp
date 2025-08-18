#include "HttpPostHandler.h"
#include "HttpData.h"
#include "ClientSocket.h"

void HttpPostHandler::HandleRequest(
    const std::shared_ptr<ClientSocket>& client,
    const std::shared_ptr<HttpData>& req)
{
    if (req->GetPath() != "/api") { Send404Response(client); return; }

    const std::string body  = req->GetBody();               // suppose Content-Length parsé
    const std::string ctype = req->GetHeader("Content-Type");

    HttpData resp;
    resp.SetStatusCode(200);
    resp.SetContentType("application/json");
    resp.SetBody(std::string("{\"ok\":true,\"len\":") + std::to_string(body.size()) +
                 ",\"ctype\":\"" + ctype + "\"}");

    client->send_http_response(resp.to_string());
}
