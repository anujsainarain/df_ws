# ROS 2 Humble
FROM ros:humble

# Directory
WORKDIR /root/df_ws/src/whisper_asr_node
COPY . .

# Dependencies
RUN apt-get update && apt-get install -y \
    python3-pip \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Build the workspace
RUN /bin/bash -c "source /opt/ros/humble/setup.bash && colcon build"

# Sourcing and running the node
CMD ["/bin/bash", "-c", "source /opt/ros/humble/setup.bash && source install/setup.bash && ros2 run whisper_asr_node whisper_asr"]
