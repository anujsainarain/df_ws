#!/bin/bash

# URL to download
FILE_URL="https://drive.google.com/uc?id=1MI0UPvoyLGLO2JgPjnhIOe9PsJT7qTRu&export=download"

# Target folder
OUTPUT_PATH="src/decision_maker_node/models/llama-2-7b-chat.Q5_K_M-002.gguf"

echo "Downloading the GGUF model file..." # Heads up
wget --no-check-certificate "$FILE_URL" -O "$OUTPUT_PATH"

echo "Download complete."
