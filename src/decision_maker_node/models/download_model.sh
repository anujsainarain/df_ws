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





# # URL to download
# # FILE_URL="https://drive.google.com/uc?id=1MI0UPvoyLGLO2JgPjnhIOe9PsJT7qTRu&export=download"
# # FILE_URL="https://drive.google.com/uc?id=1qG0xuBa_0iV3eUgBSfeAmsXWaQrUvI9L&export=download" #Paddle_ball
# FILE_ID = "1qG0xuBa_0iV3eUgBSfeAmsXWaQrUvI9L"

# # Target folder
# # OUTPUT_PATH="src/decision_maker_node/models/llama-2-7b-chat.Q5_K_M-002.gguf"
# OUTPUT_PATH="/root/df_ws/src/decision_maker_node/models/Paddle_ball.py"


# echo "Downloading the GGUF model file..." # Heads up
# wget --no-check-certificate "$FILE_URL" -O "$OUTPUT_PATH"

# echo "Download complete."
