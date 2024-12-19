#!/bin/bash

# URL for the public Google Drive file
FILE_URL="https://drive.google.com/uc?id=1MI0UPvoyLGLO2JgPjnhIOe9PsJT7qTRu&export=download"

# Target destination for the downloaded file
OUTPUT_PATH="src/decision_maker_node/models/llama-2-7b-chat.Q5_K_M-002.gguf"

# Download the file
echo "Downloading the GGUF model file..."
wget --no-check-certificate "$FILE_URL" -O "$OUTPUT_PATH"

echo "Download complete. File saved to $OUTPUT_PATH."
