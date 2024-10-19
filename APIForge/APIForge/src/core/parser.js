const fs = require('fs');

// Load the API configuration from api_config.json
function loadConfig(configPath) {
    return new Promise((resolve, reject) => {
        fs.readFile(configPath, 'utf8', (err, data) => {
            if (err) {
                return reject(`Error reading config file: ${err}`);
            }
            try {
                const config = JSON.parse(data);
                resolve(config);
            } catch (parseError) {
                reject(`Error parsing config file: ${parseError}`);
            }
        });
    });
}

// Validate the configuration
function validateConfig(config) {
    if (!config.api_info || !config.endpoints || !config.generation_settings) {
        throw new Error('Invalid configuration: Missing required fields.');
    }
}

// Extract relevant information for client generation
function extractInfo(config) {
    const { api_info, endpoints, generation_settings } = config;

    const apiDetails = {
        name: api_info.name,
        version: api_info.version,
        baseUrl: api_info.base_url,
        endpoints: endpoints.map(endpoint => ({
            name: endpoint.name,
            path: endpoint.path,
            method: endpoint.method,
            parameters: endpoint.parameters,
            responses: endpoint.responses,
        })),
        generationSettings: generation_settings,
    };

    return apiDetails;
}

// Main function to parse the API config
async function main() {
    try {
        const configPath = 'api_config.json';
        const config = await loadConfig(configPath);
        
        validateConfig(config);
        const apiDetails = extractInfo(config);

        console.log('API Details:', JSON.stringify(apiDetails, null, 2));
        // Further processing for code generation can be done here
    } catch (error) {
        console.error(error);
    }
}

main();
