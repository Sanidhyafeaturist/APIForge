import unittest
from generator import generate_client  # Import your generate_client function

class TestGenerator(unittest.TestCase):

    def test_generate_client_valid_input(self):
        api_specification = {
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

        try:
            result = generate_client(api_specification)
            self.assertIsNotNone(result)  # Ensure a result is generated
        except Exception as e:
            self.fail(f"generate_client raised an exception: {str(e)}")

    def test_generate_client_invalid_method(self):
        api_specification = {
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

        with self.assertRaises(ValueError) as context:
            generate_client(api_specification)
        self.assertEqual(str(context.exception), "Invalid HTTP method: INVALID_METHOD")

    def test_generate_client_empty_input(self):
        api_specification = {}

        with self.assertRaises(ValueError) as context:
            generate_client(api_specification)
        self.assertEqual(str(context.exception), "API specification cannot be empty")

# Run the tests
if __name__ == "__main__":
    unittest.main()
