const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');

// Load the gRPC service definition
const PROTO_PATH = 'path/to/your_service.proto'; // Replace with your actual proto file path
const packageDefinition = protoLoader.loadSync(PROTO_PATH, {});
const yourProto = grpc.loadPackageDefinition(packageDefinition).yourPackageName; // Replace with your actual package name

// gRPC client class
class GrpcClient {
    constructor(serverAddress) {
        this.client = new yourProto.YourService(serverAddress, grpc.credentials.createInsecure()); // Replace with your actual service
    }

    // RPC method implementation
    callExample(request) {
        return new Promise((resolve, reject) => {
            this.client.exampleMethod(request, (error, response) => { // Replace with your actual RPC method
                if (error) {
                    return reject(new Error(`gRPC call failed: ${error.message}`));
                }
                resolve(response);
            });
        });
    }
}

// Example usage
const serverAddress = 'localhost:50051'; // Replace with your gRPC server address
const client = new GrpcClient(serverAddress);

// Construct your request object with actual fields
const request = {
    userId: 12345,      // Example field representing user ID
    query: "someQuery"  // Example field representing the query
};

client.callExample(request)
    .then(response => {
        console.log('Response:', response); // Print the response
    })
    .catch(error => {
        console.error(`Error: ${error.message}`);
    });
