#include "graphql_template.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>

// Function to handle API responses
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function to check HTTP response code
void checkHttpResponse(CURLcode res, CURL* curl) {
    if (res != CURLE_OK) {
        throw std::runtime_error(std::string("CURL error: ") + curl_easy_strerror(res));
    }
}

// Function to send a GraphQL request
nlohmann::json sendGraphQLRequest(const std::string& url, const std::string& query, const nlohmann::json& variables) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }

    std::string responseString;
    nlohmann::json jsonResponse;

    try {
        // Construct the request payload
        nlohmann::json requestPayload = {
            {"query", query},
            {"variables", variables}
        };

        std::string jsonString = requestPayload.dump();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr); // Set headers if needed
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(nullptr, "Content-Type: application/json")); // Set the content type header

        CURLcode res = curl_easy_perform(curl);
        checkHttpResponse(res, curl);

        // Parse the JSON response
        jsonResponse = nlohmann::json::parse(responseString);
    } catch (const std::exception& e) {
        curl_easy_cleanup(curl);
        throw std::runtime_error(std::string("Error during GraphQL request: ") + e.what());
    }

    curl_easy_cleanup(curl);
    return jsonResponse;
}

// Example usage (Remove or modify as needed)
int main() {
    const std::string url = "https://your-graphql-endpoint.com/graphql";
    const std::string query = "query { yourQuery { field1 field2 } }"; // Replace with your actual query
    nlohmann::json variables = {}; // Add any variables needed for the query

    try {
        nlohmann::json response = sendGraphQLRequest(url, query, variables);
        std::cout << "Response: " << response.dump(4) << std::endl; // Pretty print the JSON response
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
