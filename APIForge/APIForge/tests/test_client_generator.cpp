#include "gtest/gtest.h"
#include "client_generator.h"  // Include your client generator header

class ClientGeneratorTest : public ::testing::Test {
protected:
    ClientGenerator generator;

    void SetUp() override {
        // Initialize any necessary state before each test
    }

    void TearDown() override {
        // Clean up any state after each test
    }
};

// Test for valid input
TEST_F(ClientGeneratorTest, GenerateClient_ValidInput) {
    std::string apiSpecification = R"(
        {
            "name": "TestAPI",
            "version": "1.0.0",
            "endpoints": [
                {
                    "method": "GET",
                    "path": "/test",
                    "description": "Test endpoint"
                }
            ]
        }
    )";

    EXPECT_NO_THROW(generator.generateClient(apiSpecification));
}

// Test for invalid input
TEST_F(ClientGeneratorTest, GenerateClient_InvalidInput) {
    std::string invalidApiSpecification = R"(
        {
            "name": "TestAPI",
            "version": "1.0.0",
            "endpoints": [
                {
                    "method": "INVALID_METHOD",
                    "path": "/test",
                    "description": "Test endpoint"
                }
            ]
        }
    )";

    EXPECT_THROW(generator.generateClient(invalidApiSpecification), std::invalid_argument);
}

// Test for empty input
TEST_F(ClientGeneratorTest, GenerateClient_EmptyInput) {
    std::string emptyApiSpecification = "";

    EXPECT_THROW(generator.generateClient(emptyApiSpecification), std::invalid_argument);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
