# ROS 2 Humble
FROM ros:humble

# Directory
WORKDIR /root/df_ws/src/decision_maker_node

# Copy the workspace files
COPY . .

# Dependencies
RUN apt-get update && apt-get install -y \
    python3-pip \
    build-essential \
    wget \
    && rm -rf /var/lib/apt/lists/*


RUN pip3 install \
    gdown \
    ctransformers

# Download model
RUN /bin/bash models/download_model.sh

# Build the workspace
RUN /bin/bash -c "source /opt/ros/humble/setup.bash && colcon build"

# Sourcing and running the node
CMD ["/bin/bash", "-c", "source /opt/ros/humble/setup.bash && source install/setup.bash && ros2 run decision_maker_node decision_maker"]
