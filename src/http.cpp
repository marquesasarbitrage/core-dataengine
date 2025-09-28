#include <iostream>
#include "../include/cpp-dataengine/core.hpp"

HTTPRequest::HTTPRequest(const std::string& url):url_(url), exceptionPtr_(nullptr) {};
YahooFinanceRequest::YahooFinanceRequest(const std::string& url): HTTPRequest(url){}; 

nlohmann::json HTTPRequest::getData() const {return data_;}
std::exception_ptr HTTPRequest::getException() const {return exceptionPtr_;}
std::string HTTPRequest::getUrl() const{return url_;}
void HTTPRequest::setException(const std::exception_ptr& exceptionPtr){exceptionPtr_ = exceptionPtr;}
void HTTPRequest::setData(const nlohmann::json& data){data_=data;}
void HTTPRequest::setUrl(const std::string& url){url_ = url;}

void HTTPRequest::run() 
{
    try {
        std::string response;

        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize CURL");
        }

        curl_easy_setopt(curl, CURLOPT_URL, getUrl().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error(curl_easy_strerror(res));
        }
        setData(nlohmann::json::parse(response));
    }
    catch (const std::exception& e) {
        setData(nlohmann::json::object()); // empty JSON
        setException(std::current_exception());
    }
}

size_t HTTPRequest::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) 
{
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string YahooFinanceRequest::getPythonVirtualEnvPath()
{
    std::string libraryName = "cpp-dataengine";
    std::filesystem::path cwd = std::filesystem::current_path();
    std::vector<std::string> result;

    bool libraryFound = false;
    for (const auto& part : cwd) {
        std::string segment = part.string();
        result.push_back(segment);
        if (segment == libraryName) {
            libraryFound = true;
            break;
        }
    }

    if (libraryFound)
    {
        std::string path; 
        for (std::string& s : result)
        {
            path += s; 
            path += "/";
        }

        return path + "venv/lib/python3.12/site-packages";
    }
    else
    {
        return (cwd / "_deps" / (libraryName + "-src") / "venv/lib/python3.12/site-packages").string();
    }
}

void YahooFinanceRequest::run()
{
     // start the Python interpreter

    try {
        py::scoped_interpreter guard{};
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").attr("insert")(0, getPythonVirtualEnvPath());
        py::module_ yFinance = py::module_::import("yfinance");
        py::object yfData = yFinance.attr("YfData");
        py::object yfDataInstance = yfData();
        std::string data = yfDataInstance.attr("get")(getUrl()).attr("text").cast<std::string>();
        setData(nlohmann::json::parse(data));
        setException(nullptr);
        
    } catch (const std::exception &e) {
        setData(nlohmann::json::object()); // empty JSON
        setException(std::current_exception());
    }
};
