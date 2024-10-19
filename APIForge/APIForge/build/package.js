const fs = require('fs');
const path = require('path');

class PackageManager {
    constructor(packageName, version) {
        this.packageName = packageName;
        this.version = version;
        this.dependencies = {};
    }

    addDependency(dependencyName, version) {
        this.dependencies[dependencyName] = version;
    }

    createPackageJson() {
        const packageJson = {
            name: this.packageName,
            version: this.version,
            main: 'index.js',
            scripts: {
                start: 'node index.js',
                test: 'echo "Error: no test specified" && exit 1'
            },
            dependencies: this.dependencies
        };

        fs.writeFileSync(path.join(process.cwd(), 'package.json'), JSON.stringify(packageJson, null, 2));
        console.log('package.json created successfully.');
    }
}

// Example usage
const packageName = 'apiforge';  // Replace with your package name
const version = '1.0.0';          // Replace with your package version

const packageManager = new PackageManager(packageName, version);

// Add dependencies
packageManager.addDependency('express', '^4.17.1');  // Example dependency
packageManager.addDependency('axios', '^0.21.1');    // Example dependency

packageManager.createPackageJson();
