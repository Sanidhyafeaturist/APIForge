#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

// Function to handle API responses
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

// Function to generate a basic API client
void generateApiClient(const std::string& baseUrl, const std::string& outputDir) {
    // Example endpoint details
    std::string endpoint = "/users";
    std::string method = "GET"; // Can be POST, PUT, etc.
    
    // Start creating the client code
    std::ofstream clientFile(outputDir + "/ApiClient.cpp");

    clientFile << "#include <iostream>\n";
    clientFile << "#include <curl/curl.h>\n\n";

    clientFile << "// Function to handle the API call\n";
    clientFile << "std::string makeApiCall(const std::string& url) {\n";
    clientFile << "    CURL* curl;\n";
    clientFile << "    CURLcode res;\n";
    clientFile << "    std::string responseString;\n\n";
    
    clientFile << "    curl_global_init(CURL_GLOBAL_DEFAULT);\n";
    clientFile << "    curl = curl_easy_init();\n";
    clientFile << "    if(curl) {\n";
    clientFile << "        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());\n";
    clientFile << "        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);\n";
    clientFile << "        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);\n";
    clientFile << "        res = curl_easy_perform(curl);\n";
    clientFile << "        if(res != CURLE_OK) {\n";
    clientFile << "            std::cerr << \"curl_easy_perform() failed: \" << curl_easy_strerror(res) << std::endl;\n";
    clientFile << "        }\n";
    clientFile << "        curl_easy_cleanup(curl);\n";
    clientFile << "    }\n";
    clientFile << "    curl_global_cleanup();\n";
    clientFile << "    return responseString;\n";
    clientFile << "}\n\n";

    // Create the main function
    clientFile << "int main() {\n";
    clientFile << "    std::string apiUrl = \"" << baseUrl << endpoint << "\";\n";
    clientFile << "    std::string response = makeApiCall(apiUrl);\n";
    clientFile << "    std::cout << \"Response: \" << response << std::endl;\n";
    clientFile << "    return 0;\n";
    clientFile << "}\n";

    clientFile.close();
    std::cout << "API client generated in " << outputDir << "/ApiClient.cpp\n";
}

int main() {
    // Example usage: generate a client for an API with a base URL
    std::string baseUrl = "https://api.sample.com";
    std::string outputDir = "./generated_clients";

    generateApiClient(baseUrl, outputDir);

    return 0;
}
