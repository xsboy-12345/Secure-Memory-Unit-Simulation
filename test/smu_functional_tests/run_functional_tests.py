import os
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEST_EXECUTABLES = [
    "component_parameter_sanity_tests",
    "fault_injection_tests",
    "read_write_tests",
]

def build_project():
    print("Building with make...")
    result = subprocess.run(["make"], cwd=SCRIPT_DIR, capture_output=True, text=True)
    if result.returncode != 0:
        print("Build failed:\n", result.stderr)
        return False
    print("Build succeeded")
    return True

def run_tests():
    for exe in TEST_EXECUTABLES:
        exe_path = os.path.join(SCRIPT_DIR, exe)
        if not os.path.isfile(exe_path):
            print(f"Skipping {exe} — not found.")
            continue

        print(f"\nRunning test: {exe}")
        result = subprocess.run([exe_path], capture_output=True, text=True)
        print("Output:")
        print(result.stdout.strip())
        if result.stderr:
            print("Error:\n", result.stderr.strip())

def main():
    if build_project():
        run_tests()

if __name__ == "__main__":
    main()