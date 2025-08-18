#include "Utils.h"
#include "HttpData.h"

#include <chrono>


std::vector<std::string> split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> result;
    size_t left = 0;
    size_t nextPos = str.find(delimiter);
    while(nextPos != std::string::npos)
    {
        result.push_back(str.substr(left, nextPos - left));
        left = nextPos + delimiter.size();
        nextPos = str.find(delimiter, left);
    }

    result.push_back(str.substr(left));

    return result;
}

long long TimestampNow() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
        ).count();
}

std::string GetContentTypeFromRequest(const std::string& path)
{
    std::string extension = path.substr(path.find_last_of('.') + 1);
    if(extension == "html")
    {
        return "text/html";
    }
    else if(extension == "css")
    {
        return "text/css";
    }
    else if(extension == "js")
    {
        return "text/javascript";
    }
    else if(extension == "png")
    {
        return "image/png";
    }
    else if(extension == "jpg")
    {
        return "image/jpg";
    }
    else if(extension == "jpeg")
    {
        return "image/jpeg";
    }
    else if(extension == "gif")
    {
        return "image/gif";
    }
    else if(extension == "svg")
    {
        return "image/svg+xml";
    }
    else if(extension == "ico")
    {
        return "image/x-icon";
    }
    else if(extension == "json")
    {
        return "application/json";
    }
    else if(extension == "xml")
    {
        return "application/xml";
    }
    else if(extension == "pdf")
    {
        return "application/pdf";
    }
    else if(extension == "zip")
    {
        return "application/zip";
    }
    else if(extension == "gzip")
    {
        return "application/gzip";
    }
    else if(extension == "mp3")
    {
        return "audio/mpeg";
    }
    else if(extension == "mp4")
    {
        return "video/mp4";
    }
    else if(extension == "mpeg")
    {
        return "video/mpeg";
    }
    else if(extension == "webm")
    {
        return "video/webm";
    }
    else if(extension == "ogg")
    {
        return "video/ogg";
    }
    else if(extension == "flac")
    {
        return "audio/flac";
    }
    else if(extension == "wav")
    {
        return "audio/wav";
    }
    else if(extension == "webp")
    {
        return "image/webp";
    }
    else if(extension == "woff")
    {
        return "font/woff";
    }
    else if(extension == "woff2")
    {
        return "font/woff2";
    }
    else if(extension == "ttf")
    {
        return "font/ttf";
    }
    else if(extension == "otf")
    {
        return "font/otf";
    }
    else return "text/plain";
}

