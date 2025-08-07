#include <memory>
#include "HttpData.h"
#include "Utils.h"

HttpData::HttpData(const std::vector<HttpHeader>& headers, const HttpVersion& version, const HttpMethod& method,
                   const std::string& path, const std::vector<HttpQueryParam>& params, const std::string& body) :
Headers(headers), Version(version), Method(method), Path(path), QueryParams(params), Body(body)
{

}

HttpVersion VersionStrToEnum(const std::string& version)
{
    if(version == "HTTP/1.1")
        return HttpVersion::HTTP_VERSION_11;
    if(version == "HTTP/2.0")
        return HttpVersion::HTTP_VERSION_20;

    return HTTP_VERSION_UNKNOWN;
}

HttpMethod MethodStrToEnum(const std::string& method)
{
    if(method == "GET")
        return HttpMethod::HTTP_METHOD_GET;
    if(method == "PUT")
        return HttpMethod::HTTP_METHOD_PUT;
    if(method == "POST")
        return HttpMethod::HTTP_METHOD_POST;
    if(method == "HEAD")
        return HttpMethod::HTTP_METHOD_HEAD;
    if(method == "DELETE")
        return HttpMethod::HTTP_METHOD_DELETE;

    return HTTP_METHOD_UNKNOWN;
}

std::string GetHttpCodeStr(int return_code)
{
    switch (return_code) {
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 202:
        return "Accepted";
    case 204:
        return "No Content";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 304:
        return "Not Modified";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 409:
        return "Conflict";
    case 410:
        return "Gone";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    case 503:
        return "Service Unavailable";
    case 504:
        return "Gateway Timeout";
    default:
        return "Unknown Status Code";
    }
}

std::string HttpVersionToString(const HttpVersion version)
{
    switch(version)
    {
    case HTTP_VERSION_10:
        return "HTTP/1.0";
    case HTTP_VERSION_11:
        return "HTTP/1.1";
    case HTTP_VERSION_20:
        return "HTTP/2.0";

    case HTTP_VERSION_UNKNOWN:
        return "UNKNOWN";
    }

    return "HTTP/1.1";
}

std::vector<HttpHeader> ParseHeaders(const std::vector<std::string>& headersList)
{
    std::vector<HttpHeader> headers;
    for(const auto& header : headersList)
    {
        auto pos = header.find(": ");
        auto name = header.substr(0, pos);
        auto value = header.substr(pos + 2);
        headers.push_back({name, value});
    }

    return headers;
}

std::vector<HttpQueryParam> ParseParams(const std::vector<std::string>& pathParameters)
{
    std::vector<HttpQueryParam> Params;
    for(const auto& param: pathParameters)
    {
        auto pos = param.find("=");
        auto name = param.substr(0, pos);
        auto value = param.substr(pos + 1);
        Params.push_back({name, value});
    }

    return Params;
}

std::shared_ptr<HttpData> HttpData::fromString(const std::string& RawData) {
    size_t pos = RawData.find("\r\n\r\n");

    auto headersData = RawData.substr(0, pos);
    auto body = RawData.substr(pos + 4);

    std::string requestData = headersData.substr(0, headersData.find('\r'));
    std::string strMethod = requestData.substr(0, requestData.find(' '));
    std::string strPath = requestData.substr(strMethod.size() + 1, requestData.find(' ', strMethod.size() + 1) - (strMethod.size() + 1)) ;
    auto queryParamsStr = strPath.substr(strPath.find('?') + 1);
    strPath = strPath.substr(0, strPath.find('?'));
    std::string strVersion = requestData.substr(requestData.rfind(' ') + 1);

    auto headersStr = headersData.substr(headersData.find("\r\n") + 2);
    auto headersList = split(headersStr, "\r\n");

    auto queryParamsList = split(queryParamsStr, "&");

    auto headers = ParseHeaders(headersList);
    auto params = ParseParams(queryParamsList);
    auto method = MethodStrToEnum(strMethod);
    auto version = VersionStrToEnum(strVersion);

    return std::make_shared<HttpData>(headers, version, method, strPath, params, body);
}

const std::string HttpData::GetQueryParamsString() const
{
    std::string result;
    for(const auto& param : QueryParams)
    {
        result += param.name + "=" + param.value + "&";
    }

    return result;
}

const std::string& HttpData::GetPath() const
{
    return Path;
}

const std::string& HttpData::GetBody() const
{
    return Body;
}

const HttpMethod& HttpData::GetMethod() const
{
    return Method;
}

void HttpData::AddHeader(const std::string& name, const std::string& value)
{
    for(auto& header : Headers)
    {
        if(header.name == name)
        {
            header.value = value;
            return;
        }
    }

    Headers.push_back({name, value});
}

void HttpData::SetBody(const std::string& string)
{
    Body = string;
    AddHeader("Content-Length", std::to_string(string.size()));
}

void HttpData::SetStatusCode(int i)
{
    StatusCode = i;
}

void HttpData::SetContentType(const std::string& string)
{
    AddHeader("Content-Type", string);
}

std::string HttpData::to_string()
{
    std::string result;
    result += HttpVersionToString(Version);
    result += " ";
    result += std::to_string(StatusCode);
    result += " ";
    result += GetHttpCodeStr(StatusCode);
    result += "\r\n";

    for(const auto& header : Headers)
    {
        result += header.name + ": " + header.value + "\r\n";
    }
    result += "\r\n";
    result += Body;

    return result;
}
