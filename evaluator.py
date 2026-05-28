import json
import subprocess
import sys

N, M, K = 2048, 1536, 2560  # all multiples of 128

if __name__ == "__main__":
    # Build
    build = subprocess.run(
        ["make", "clean"],
        capture_output=True, text=True
    )
    build = subprocess.run(
        ["make"],
        capture_output=True, text=True
    )
    if build.returncode != 0:
        result = {
            "metainfo": f"build failed: {build.stderr.strip()}",
        }
        print(json.dumps(result), flush=True)
        sys.exit(0)

    # Run
    run = subprocess.run(
        ["./matmul", str(N), str(M), str(K)],
        capture_output=True, text=True, timeout=120
    )
    if run.returncode != 0:
        result = {
            "metainfo": "RuntimeError",
        }
        print(json.dumps(result), flush=True)
        sys.exit(0)

    # Parse output from matmul binary
    output = json.loads(run.stdout.strip())

    if output["status"] == "ERROR":
        result = {
            "metainfo": output["reason"],
        }
    else:
        speedup = -1000 + output["speedup"]
        mem_gb = (M*K + K*N + M*N) * 8 / 1e9
        result = {
            "output": {
                "fitness": speedup,
                "signature": [output["runtime"]],
                "summary": f"speedup={speedup:.4f} runtime={output['runtime']:.4e}s mem={mem_gb:.3f}GB",
            },
            "metainfo": "Success",
        }

    # Must be on the very last line on stdout and can't span multiple lines.
    print(json.dumps(result), flush=True)
