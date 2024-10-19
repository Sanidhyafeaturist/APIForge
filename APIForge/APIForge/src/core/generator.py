import json
import os
from jinja2 import Environment, FileSystemLoader

# Load the configuration from api_config.json
def load_config(config_path):
    with open(config_path, 'r') as file:
        return json.load(file)

# Ensure the output directory exists
def ensure_output_directory(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

# Function to generate client files using Jinja2 templates
def generate_client(api_info, endpoints, generation_settings):
    # Set up Jinja2 environment for template rendering
    env = Environment(loader=FileSystemLoader('templates'))

    # Iterate through the languages defined in the generation settings
    for language, settings in generation_settings['languages'].items():
        # Load the appropriate template for the language
        template = env.get_template(f"{language}_client.j2")

        # Prepare data for the template
        template_data = {
            'api_info': api_info,
            'endpoints': endpoints,
            'settings': settings
        }

        # Generate the client code by rendering the template with data
        output_code = template.render(template_data)

        # Ensure the output directory exists
        ensure_output_directory(settings['output_directory'])

        # Write the generated code to the appropriate file
        output_file_path = os.path.join(settings['output_directory'], f"api_client.{language}")
        with open(output_file_path, 'w') as output_file:
            output_file.write(output_code)

        print(f"API client generated for {language} at {output_file_path}")

# Main function to load the config and start the generation process
def main():
    config = load_config('api_config.json')
    
    api_info = config.get('api_info', {})
    endpoints = config.get('endpoints', [])
    generation_settings = config.get('generation_settings', {})

    generate_client(api_info, endpoints, generation_settings)

if __name__ == "__main__":
    main()
