import os
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MODULE_DIR = os.path.join(SCRIPT_DIR, "eingabe")
MODULE_EXEC = os.path.join(MODULE_DIR, "module")
CSV_DIR = MODULE_DIR

def run_all_tests():
    print("\nRunning tests using CSV input files")

    # Build the module
    print("Building module...")
    result = subprocess.run(["make"], cwd=MODULE_DIR, capture_output=True, text=True)
    if result.returncode != 0:
        print("Build failed:\n", result.stderr)
        return
    print("Build succeeded")

    for file in os.listdir(CSV_DIR):
        if file.endswith(".csv"):
            path = os.path.join(CSV_DIR, file)
            print(f"\nTesting with file: {file}")

            result = subprocess.run([MODULE_EXEC, path], capture_output=True, text=True)

            print("Output:")
            print(result.stdout.strip())

            if result.stderr:
                print("Error:\n", result.stderr.strip())

if __name__ == "__main__":
    run_all_tests()