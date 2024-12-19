#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <cstdlib>

class GoogleTTSNode : public rclcpp::Node {
public:
    GoogleTTSNode() : Node("google_tts_node") {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "text_to_speak", 10,
            std::bind(&GoogleTTSNode::synthesizeSpeech, this, std::placeholders::_1));
    }

private:
    void synthesizeSpeech(const std_msgs::msg::String::SharedPtr msg) {
        // TTS script
        std::string command = "python3 ~/df_ws/src/google_tts_node/scripts/tts_inference.py \"" + msg->data + "\" ~/df_ws/src/google_tts_node/output/output.wav";
        int ret_code = system(command.c_str());

        if (ret_code != 0) {
            RCLCPP_ERROR(this->get_logger(), "Failed to synthesize:'%s'", msg->data.c_str());
        } else {
            RCLCPP_INFO(this->get_logger(), "Successfully Synthesized: '%s'", msg->data.c_str());
        }
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GoogleTTSNode>());
    rclcpp::shutdown();
    return 0;
}
