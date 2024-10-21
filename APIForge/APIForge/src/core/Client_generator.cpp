#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <sys/stat.h>
#include <nlohmann/json.hpp> // Include the nlohmann JSON library

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir // For Windows compatibility
#endif

// Function to handle API responses
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

// Function to generate a basic API client
void generateApiClient(const std::string& baseUrl, const std::string& endpoint, const std::string& method, const std::string& outputDir) {
    // Ensure output directory exists
    struct stat info;
    if (stat(outputDir.c_str(), &info) != 0) {
        mkdir(outputDir.c_str(), 0777); // Use _mkdir for Windows
    }

    // Start creating the client code
    std::ofstream clientFile(outputDir + "/ApiClient.cpp");
    if (!clientFile.is_open()) {
        std::cerr << "Failed to open file: " << outputDir + "/ApiClient.cpp" << std::endl;
        return;
    }

    // Write the necessary includes and callback function for API response handling
    clientFile << "#include <iostream>\n";
    clientFile << "#include <curl/curl.h>\n\n";

    clientFile << "// Function to handle API responses\n";
    clientFile << "static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {\n";
    clientFile << "    size_t totalSize = size * nmemb;\n";
    clientFile << "    out->append((char*)contents, totalSize);\n";
    clientFile << "    return totalSize;\n";
    clientFile << "}\n\n";

    // Write the makeApiCall function
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

// Function to read configurations from JSON file
nlohmann::json readConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file: " << configFile << std::endl;
        exit(1);
    }

    nlohmann::json config;
    file >> config;
    return config;
}

int main() {
    // Read configuration from JSON file
    std::string configFile = "api_config.json"; // Path to your JSON config file
    nlohmann::json config = readConfig(configFile);

    // Extract configuration values
    std::string baseUrl = config["baseUrl"];
    std::string endpoint = config["endpoint"];
    std::string method = config["method"];
    std::string outputDir = "./output"; // The directory to store the generated client

    // Generate API client based on the configuration
    generateApiClient(baseUrl, endpoint, method, outputDir);

    return 0;
}
