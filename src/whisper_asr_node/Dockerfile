# ROS 2 Humble
FROM ros:humble

# Directory
WORKDIR /root/df_ws/src/whisper_asr_node
COPY . .

# Cloning the whisper_cpp repository
RUN git clone https://github.com/ggerganov/whisper.cpp src/whisper_asr_node/whisper_cpp && \
rm -rf src/whisper_asr_node/whisper_cpp/.git



# Dependencies
RUN apt-get update && apt-get install -y \
    python3-pip \
    build-essential \
    wget \
    && rm -rf /var/lib/apt/lists/*

RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc

# Building the GGML file
RUN mkdir -p whisper_cpp/models && \
    wget -P whisper_cpp/models https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin
    
# Build the workspace
RUN /bin/bash -c "source /opt/ros/humble/setup.bash && colcon build"

# Sourcing and running the node
CMD ["/bin/bash", "-c", "source /opt/ros/humble/setup.bash && source install/setup.bash && ros2 run whisper_asr_node whisper_asr"]
