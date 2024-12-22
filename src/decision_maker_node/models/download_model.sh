#!/bin/bash

FILE_ID="1MI0UPvoyLGLO2JgPjnhIOe9PsJT7qTRu"
OUTPUT_PATH="/root/df_ws/src/decision_maker_node/models/llama-2-7b-chat.Q5_K_M.gguf"

echo "Downloading the file..."
gdown --id "$FILE_ID" -O "$OUTPUT_PATH"

if [ $? -eq 0 ]; then
    echo "Download complete."
else
    echo "Failed."
    exit 1
fi