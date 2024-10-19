import requests

class RestClient:
    def __init__(self, base_url):
        self.base_url = base_url

    def get_example(self, endpoint):
        url = f"{self.base_url}/{endpoint}"
        response = requests.get(url)
        
        if response.status_code != 200:
            raise Exception(f"GET request failed: {response.status_code} - {response.text}")

        return response.json()

    def post_example(self, endpoint, data):
        url = f"{self.base_url}/{endpoint}"
        response = requests.post(url, json=data)
        
        if response.status_code != 200:
            raise Exception(f"POST request failed: {response.status_code} - {response.text}")

        return response.json()

# Example usage
if __name__ == "__main__":
    base_url = "https://api.example.com"  # Replace with your actual API base URL
    client = RestClient(base_url)

    # GET request example
    try:
        get_response = client.get_example("endpoint")  # Replace with your actual endpoint
        print("GET Response:", get_response)
    except Exception as e:
        print("Error during GET request:", e)

    # POST request example
    try:
        post_data = {
            "field1": "value1",  # Replace with actual field and value
            "field2": "value2"   # Replace with actual field and value
        }
        post_response = client.post_example("endpoint", post_data)  # Replace with your actual endpoint
        print("POST Response:", post_response)
    except Exception as e:
        print("Error during POST request:", e)
