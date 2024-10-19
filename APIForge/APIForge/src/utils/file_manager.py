import os

class FileManager:
    def __init__(self, file_path):
        self.file_path = file_path

    def read_file(self):
        try:
            with open(self.file_path, 'r') as file:
                return file.read()
        except FileNotFoundError:
            raise Exception(f"File not found: {self.file_path}")
        except Exception as e:
            raise Exception(f"Error reading file: {e}")

    def write_file(self, content):
        try:
            with open(self.file_path, 'w') as file:
                file.write(content)
        except Exception as e:
            raise Exception(f"Error writing to file: {e}")

    def delete_file(self):
        try:
            os.remove(self.file_path)
        except FileNotFoundError:
            raise Exception(f"File not found: {self.file_path}")
        except Exception as e:
            raise Exception(f"Error deleting file: {e}")

# Example usage
if __name__ == "__main__":
    file_path = 'example.txt'  # Replace with your actual file path
    file_manager = FileManager(file_path)

    # Write to the file
    try:
        file_manager.write_file("This is some content.")  # Replace with actual content
        print("File written successfully.")
    except Exception as e:
        print("Error during write operation:", e)

    # Read the file
    try:
        content = file_manager.read_file()
        print("File content:", content)
    except Exception as e:
        print("Error during read operation:", e)

    # Delete the file
    try:
        file_manager.delete_file()
        print("File deleted successfully.")
    except Exception as e:
        print("Error during delete operation:", e)
