#include <iostream>
#include <fstream>
#include <string>
#include "curl/include/curl/curl.h"
#include <sys/stat.h>
#include <sstream>

#ifdef _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir) // For Windows, _mkdir takes only one argument
#else
#include <sys/types.h>
#include <unistd.h>
#endif

// Function to handle API responses
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

// Function to manually parse JSON configuration (without using external libraries)
void parseJsonConfig(const std::string& jsonContent, std::string& baseUrl, std::string& endpoint, std::string& method, std::string& headers, std::string& body) {
    std::istringstream ss(jsonContent);
    std::string line;
    while (std::getline(ss, line)) {
        // Parse baseUrl
        if (line.find("\"baseUrl\"") != std::string::npos) {
            size_t start = line.find(":") + 2;
            size_t end = line.find_last_of("\"");
            baseUrl = line.substr(start, end - start);
        }
        // Parse endpoint
        if (line.find("\"endpoint\"") != std::string::npos) {
            size_t start = line.find(":") + 2;
            size_t end = line.find_last_of("\"");
            endpoint = line.substr(start, end - start);
        }
        // Parse method
        if (line.find("\"method\"") != std::string::npos) {
            size_t start = line.find(":") + 2;
            size_t end = line.find_last_of("\"");
            method = line.substr(start, end - start);
        }
        // Parse headers (if present)
        if (line.find("\"headers\"") != std::string::npos) {
            size_t start = line.find(":") + 2;
            size_t end = line.find_last_of("\"");
            headers = line.substr(start, end - start);
        }
        // Parse body (for POST/PUT methods)
        if (line.find("\"body\"") != std::string::npos) {
            size_t start = line.find(":") + 2;
            size_t end = line.find_last_of("\"");
            body = line.substr(start, end - start);
        }
    }
}

// Function to generate the API client based on the JSON configuration
void generateApiClient(const std::string& baseUrl, const std::string& endpoint, const std::string& method, const std::string& headers, const std::string& body, const std::string& outputDir) {
    // Ensure the output directory exists
#ifdef _WIN32
    _mkdir(outputDir.c_str()); // Use _mkdir for Windows
#else
    mkdir(outputDir.c_str(), 0777); // Use POSIX mkdir for Unix-like systems
#endif

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

    // Add dynamic method handling
    if (method == "POST" || method == "PUT") {
        clientFile << "        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, \"" << method << "\");\n";
        if (!body.empty()) {
            clientFile << "        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, \"" << body << "\");\n";
        }
    } else {
        clientFile << "        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);\n";
    }

    // Handle headers if any
    if (!headers.empty()) {
        clientFile << "        struct curl_slist *headers = NULL;\n";
        clientFile << "        headers = curl_slist_append(headers, \"" << headers << "\");\n";
        clientFile << "        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);\n";
    }

    clientFile << "        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);\n";
    clientFile << "        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);\n";
    clientFile << "        res = curl_easy_perform(curl);\n";
    clientFile << "        if(res != CURLE_OK) {\n";
    clientFile << "            std::cerr << \"curl_easy_perform() failed: \" << curl_easy_strerror(res) << std::endl;\n";
    clientFile << "        }\n";
    clientFile << "        curl_easy_cleanup(curl);\n";
    if (!headers.empty()) {
        clientFile << "        curl_slist_free_all(headers);\n";
    }
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
    // Read the JSON configuration file
    std::ifstream configFile("json.json");
    if (!configFile) {
        std::cerr << "Error: Could not open configuration file.\n";
        return 1;
    }

    // Read the contents of the JSON file into a string
    std::stringstream buffer;
    buffer << configFile.rdbuf();
    std::string jsonContent = buffer.str();

    // Variables to hold configuration values
    std::string baseUrl, endpoint, method, headers, body;

    // Parse the JSON file to extract configuration values
    parseJsonConfig(jsonContent, baseUrl, endpoint, method, headers, body);

    // Directory to generate the API client code
    std::string outputDir = "./generated_clients";

    // Generate the API client code based on the configuration
    generateApiClient(baseUrl, endpoint, method, headers, body, outputDir);

    return 0;
}
