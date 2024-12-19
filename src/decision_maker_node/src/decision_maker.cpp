#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <array>
#include <memory>

class DecisionMakerNode : public rclcpp::Node {
public:
    DecisionMakerNode() : Node("decision_maker_node") {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "recognized_speech", 10,
            std::bind(&DecisionMakerNode::processSpeech, this, std::placeholders::_1));

        publisher_ = this->create_publisher<std_msgs::msg::String>("final_response", 10);
    }

private:
    void processSpeech(const std_msgs::msg::String::SharedPtr msg) {
        // RCLCPP_INFO(this->get_logger(), "Received speech: '%s'", msg->data.c_str()); // Uncomment if you dont want to see the recieved speech

        
        std::string llm_response = callLLMInference(msg->data);             // Calling the LLM inference script

        // Publishing the response
        auto message = std_msgs::msg::String();
        message.data = llm_response;            // Response from LLM
        RCLCPP_INFO(this->get_logger(), "DF Models says: '%s'", llm_response.c_str());
        publisher_->publish(message);
    }

    std::string callLLMInference(const std::string &input) {
        // Construct the command to execute the Python script
        std::string command = "python3 ~/df_ws/src/decision_maker_node/scripts/llm_inference.py \"" + input + "\"";
        std::string output;

        // Execute the command and capture output
        std::array<char, 128> buffer;
        std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            RCLCPP_ERROR(this->get_logger(), "Failed to execute LLM inference script.");
            return "Error";
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            output += buffer.data();
        }

        // Trim output (optional)
        output.erase(output.find_last_not_of(" \n\r\t") + 1);
        return output;
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DecisionMakerNode>());
    rclcpp::shutdown();
    return 0;
}
