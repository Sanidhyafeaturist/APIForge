#ifndef COMMON_TEMPLATE_H
#define COMMON_TEMPLATE_H

#include <string>
#include <stdexcept>
#include <curl/curl.h>
#include <nlohmann/json.hpp> // Include the nlohmann JSON library

// Common HTTP status codes
enum class HttpStatusCode {
    OK = 200,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500,
};

// Function to handle API responses
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function to check HTTP response code
inline void checkHttpResponse(CURLcode res, CURL* curl) {
    if (res != CURLE_OK) {
        throw std::runtime_error(std::string("CURL error: ") + curl_easy_strerror(res));
    }
}

// Function to parse the JSON response
inline nlohmann::json parseJsonResponse(const std::string& jsonResponse) {
    return nlohmann::json::parse(jsonResponse); // Parse the JSON response
}

// Common utility function to make an API request
template<typename RequestType, typename ResponseType>
ResponseType makeApiCall(CURL* curl, const std::string& url, RequestType& request) {
    std::string responseString;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    CURLcode res = curl_easy_perform(curl);
    checkHttpResponse(res, curl);
    
    nlohmann::json jsonResponse = parseJsonResponse(responseString);
    ResponseType response; // Create response object based on the actual API response
    // Populate response object from jsonResponse as necessary
    return response;
}

#endif // COMMON_TEMPLATE_H
