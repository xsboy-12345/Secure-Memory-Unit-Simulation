import os
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MODULE_DIR = os.path.join(SCRIPT_DIR, "eingabe")
MODULE_EXEC = os.path.join(MODULE_DIR, "module")
CSV_DIR = MODULE_DIR
TRACEFILE = os.path.join(SCRIPT_DIR, "..", "tracefile", "a.vcd")

def run_all_tests():
    print("\n Testing CSV parsing and CLI interface (module)")
    for file in os.listdir(CSV_DIR):
        if file.endswith(".csv"):
            path = os.path.join(CSV_DIR, file)
            print(f"\n File: {file}")
            result = subprocess.run([
                MODULE_EXEC,
                "-c", "50",
                "-t", TRACEFILE,
                "-e", "0",
                "-s", "0",
                "-n", "0",
                "-m", "0",
                "-d", "42",
                path
            ], capture_output=True, text=True)

            print(result.stdout.strip())
            if result.stderr:
                print(" Error:\n", result.stderr.strip())

if __name__ == "__main__":
    run_all_tests()
