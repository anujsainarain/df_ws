# ROS 2 Humble
FROM ros:humble

# Directory
WORKDIR /root/df_ws/src/google_tts_node

# Copy the workspace files
COPY . .

# Dependencies
RUN apt-get update && apt-get install -y \
    python3-pip \
    build-essential \
    ffmpeg \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install gtts pydub
# RUN sudo apt install ffmpeg
RUN /bin/bash -c "source /opt/ros/humble/setup.bash && colcon build"

# Sourcing and running the code
CMD ["/bin/bash", "-c", "source /opt/ros/humble/setup.bash && source install/setup.bash && ros2 run google_tts_node google_tts"]