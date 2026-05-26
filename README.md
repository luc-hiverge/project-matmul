# matmul

Example project demonstrating how to use a pre-built custom Docker base image with the Hive. The task itself is optimizing double-precision matrix multiplication (C = A*B, row-major).

See https://docs.hiverge.ai/gettingstarted/custom-base-image for details on custom base images.

## Structure

    *.cpp, *.h    -- evolved source files
    evaluator.py  -- builds, runs, parses output into Hive fitness JSON
    Dockerfile    -- custom base image (pre-compiles everything into /app)
    hive.yaml     -- experiment configuration

## Build

    make        # produces ./matmul
    ./matmul 2048 1536 2560

## Docker image

    docker build -t <your-registry>/matmul:latest .
    docker push <your-registry>/matmul:latest

