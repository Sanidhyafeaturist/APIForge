package main

import (
	"context"
	"fmt"
	"log"
	"time"

	"google.golang.org/grpc"
	"google.golang.org/grpc/metadata"
	"github.com/Sanidhyafeaturist/APIForge/api"
)

type Client struct {
	conn   *grpc.ClientConn
	client NewClient
}

// NewClient initializes a new gRPC client
func NewClient(address string) (*Client, error) {
	conn, err := grpc.Dial(address, grpc.WithInsecure()) // Use WithInsecure for testing; replace with secure options for production
	if err != nil {
		return nil, fmt.Errorf("did not connect: %v", err)
	}

	client := UserService(conn) // Replace with your gRPC service client interface
	return &Client{conn: conn, client: client}, nil
}

// Close closes the gRPC connection
func (c *Client) Close() {
	c.conn.Close()
}

// CallExample makes a gRPC call to the Example method
func (c *Client) CallExample(ctx context.Context, userID string) (*ExampleResponse, error) {
	// Set up context with timeout
	ctx, cancel := context.WithTimeout(ctx, time.Second)
	defer cancel()

	// Add any necessary metadata
	md := metadata.New(map[string]string{"Authorization": "Bearer YOUR_TOKEN"})
	ctx = metadata.NewIncomingContext(ctx, md)

	// Create request
	req := &CreateUserRequest{
		Name:  userName, // Use the actual user name
		Email: userEmail, // Use the actual user email
	if err != nil {
		return nil, fmt.Errorf("could not call Example: %v", err)
	}
	return res, nil
}

// main function for testing the gRPC client
func main() {
	address := "localhost:50051" // Replace with your gRPC server address

	client, err := NewClient(address)
	if err != nil {
		log.Fatalf("Failed to create client: %v", err)
	}
	defer client.Close()

	// Make a call to the Example method
	ctx := context.Background()
	response, err := client.CallExample(ctx, "12345") // Replace with an actual user ID
	if err != nil {
		log.Fatalf("Error calling Example: %v", err)
	}

	// Process the response
	log.Printf("Response: %v", response)
}
