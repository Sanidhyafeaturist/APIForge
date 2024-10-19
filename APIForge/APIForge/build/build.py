import subprocess
import os
import sys

class BuildManager:
    def __init__(self, project_root):
        self.project_root = project_root

    def build_cpp(self):
        cpp_source_files = [
            "logger.cpp",
            "client_generator.cpp",  # Add other necessary .cpp files
        ]
        for source in cpp_source_files:
            self._compile_cpp(source)

    def _compile_cpp(self, source_file):
        output_file = os.path.splitext(source_file)[0]
        command = ["g++", source_file, "-o", output_file]
        try:
            subprocess.run(command, check=True, cwd=self.project_root)
            print(f"Compiled {source_file} to {output_file}.")
        except subprocess.CalledProcessError as e:
            print(f"Error compiling {source_file}: {e}")

    def build_python(self):
        python_files = [
            "config_reader.py",
            "file_manager.py",
            "logger.py",
            "validator.py",  # Add other necessary .py files
        ]
        for file in python_files:
            self._run_python(file)

    def _run_python(self, python_file):
        command = ["python", python_file]
        try:
            subprocess.run(command, check=True, cwd=self.project_root)
            print(f"Executed {python_file}.")
        except subprocess.CalledProcessError as e:
            print(f"Error executing {python_file}: {e}")

    def build_go(self):
        go_files = [
            "validator.go",  # Add other necessary .go files
        ]
        for file in go_files:
            self._build_go(file)

    def _build_go(self, go_file):
        command = ["go", "build", go_file]
        try:
            subprocess.run(command, check=True, cwd=self.project_root)
            print(f"Compiled {go_file}.")
        except subprocess.CalledProcessError as e:
            print(f"Error compiling {go_file}: {e}")

if __name__ == "__main__":
    project_root = os.path.dirname(os.path.abspath(__file__))
    build_manager = BuildManager(project_root)

    build_manager.build_cpp()
    build_manager.build_python()
    build_manager.build_go()

    print("Build process completed.")
