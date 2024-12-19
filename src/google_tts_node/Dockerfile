# Use the ROS 2 Humble base image
FROM ros:humble

# Set working directory
WORKDIR /root/df_ws/src/google_tts_node

# Copy the workspace files
COPY . .

# Build the workspace
RUN apt-get update && apt-get install -y \
    python3-pip \
    build-essential \
    ffmpeg \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install gtts pydub
RUN sudo apt install ffmpeg
RUN /bin/bash -c "source /opt/ros/humble/setup.bash && colcon build"

# Source the workspace when starting the container
CMD ["/bin/bash", "-c", "source /opt/ros/humble/setup.bash && source install/setup.bash && ros2 run google_tts_node google_tts"]