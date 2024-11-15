#pragma once
#include <string>
#include <vector>
#include <memory>

struct HttpNameValueCouple{
    std::string name;
    std::string value;
};

using HttpQueryParam = HttpNameValueCouple;
using HttpHeader = HttpNameValueCouple;

enum HttpVersion {
    HTTP_VERSION_10,
    HTTP_VERSION_11,
    HTTP_VERSION_20,

    HTTP_VERSION_UNKNOWN
};

enum HttpMethod {
    HTTP_METHOD_GET,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,

    HTTP_METHOD_UNKNOWN
};

class HttpData {
public:
    HttpData() : Version(HTTP_VERSION_11), Method(HTTP_METHOD_UNKNOWN){};
    HttpData(const std::vector<HttpHeader>& headers, const HttpVersion& version, const HttpMethod& method,
             const std::string& path, const std::vector<HttpQueryParam>& params, const std::string& body = "");


    static std::shared_ptr<HttpData> fromString(const std::string& RawData);

    const std::string& GetHeader(const std::string& name) const;
    const std::string& GetQueryParam(const std::string& name) const;
    const std::string GetQueryParamsString() const;
    const std::string& GetPath() const;
    const std::string& GetBody() const;
    const HttpVersion& GetVersion() const;
    const HttpMethod& GetMethod() const;

    void AddHeader(const std::string& name, const std::string& value);
    void SetBody(const std::string& string);
    void SetStatusCode(int i);
    void SetContentType(const std::string& string);
    void SetMethod(const HttpMethod& method);

    std::string to_string();

private:
    static std::vector<HttpHeader> ParseRequestData();

private:
    std::vector<HttpHeader> Headers;
    HttpVersion Version;
    HttpMethod Method;
    std::string Path;
    std::string Body;
    std::vector<HttpQueryParam> QueryParams;

    int StatusCode;
};
