const fs = require('fs');

class ConfigReader {
    constructor(filePath) {
        this.filePath = filePath;
        this.config = this.loadConfig();
    }

    loadConfig() {
        try {
            const rawData = fs.readFileSync(this.filePath, 'utf8');
            return JSON.parse(rawData);
        } catch (error) {
            throw new Error(`Failed to load config: ${error.message}`);
        }
    }

    get(key) {
        if (!(key in this.config)) {
            throw new Error(`Config key "${key}" not found.`);
        }
        return this.config[key];
    }
}

// Example usage
if (require.main === module) {
    const configPath = '../../config/api_config.json';
    const configReader = new ConfigReader(configPath);

    try {
        const someSetting = configReader.get('someSetting'); // Replace with the actual key you want to retrieve
        console.log('Value of someSetting:', someSetting);
    } catch (error) {
        console.error(`Error: ${error.message}`);
    }
}
