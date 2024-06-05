import subprocess
import os

# Define the paths to the subdirectory and scripts
base_path = os.path.dirname(os.path.abspath(__file__))
subdirectory_path = os.path.join(base_path, "Website")
server_js_path = os.path.join(base_path, "server.js")
testserver_js_path = os.path.join(base_path, "testserver.js")
python_script_path = os.path.join(base_path, "3d.py")


# Function to run a process
def run_process(command):
    try:
        result = subprocess.Popen(command, shell=True)
        print(f"Started process: {command}")
        return result
    except Exception as e:
        print(f"Failed to start process {command}: {e}")


# Change the current working directory to the subdirectory
os.chdir(subdirectory_path)

# Print the current working directory to verify the change
print("Current working directory:", os.getcwd())

# Run the npm run dev command
npm_command = ["npm", "run", "dev"]
try:
    npm_process = subprocess.Popen(npm_command, shell=True)
    print("Started npm run dev")
except subprocess.CalledProcessError as e:
    print("An error occurred while running npm run dev:", e)

# Change back to the base directory
os.chdir(base_path)

# Run server.js using Node.js
server_process = run_process(f"node {server_js_path}")

# Run testserver.js using Node.js
testserver_process = run_process(f"node {testserver_js_path}")

# Run 3d.py using Flask (assuming it is a Flask app)
flask_process = run_process(f"python {python_script_path}")

# Wait for processes to complete (if needed)
server_process.wait()
testserver_process.wait()
flask_process.wait()
npm_process.wait()
