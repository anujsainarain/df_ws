#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp> 
#include <vector>
#include <fstream>
#include <string>
#include "whisper.h"


class WhisperASRNode : public rclcpp::Node {
public:
    WhisperASRNode() : Node("whisper_asr_node") {
        publisher_ = this->create_publisher<std_msgs::msg::String>("recognized_speech", 10);
        timer_ = this->create_wall_timer(
            std::chrono::seconds(10),
            std::bind(&WhisperASRNode::processAudio, this));
    }

private:
    void processAudio() {
        std::string model_path = "/home/amenephous/df_ws/src/whisper_asr_node/whisper_cpp/models/ggml-base.bin";
        std::string audio_path = "/home/amenephous/df_ws/src/whisper_asr_node/audio/Food.wav";

        // Initializing
        struct whisper_context *ctx = whisper_init_from_file(model_path.c_str());
        if (!ctx) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize the whisper model.");
            return;
        }

        // Load the audio file. Note, save the audio file at ~/df_ws/src/whisper_asr_node/audio
        std::vector<float> audio_data;
        if (!loadAudio(audio_path, audio_data)) {
            RCLCPP_ERROR(this->get_logger(), "No audio file at: %s", audio_path.c_str());
            whisper_free(ctx);
            return;
        }

        // Transcribingthe audio
        std::string transcription = runTranscription(ctx, audio_data);

        // Publishing the transcribed audio
        auto message = std_msgs::msg::String();
        message.data = transcription;
        RCLCPP_INFO(this->get_logger(), "Transcribed Audio: '%s'", transcription.c_str());
        publisher_->publish(message);

        // Free whisper context
        whisper_free(ctx);
    }

    bool loadAudio(const std::string &path, std::vector<float> &audio_data) {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "Get permission to load the audio: %s", path.c_str());
            return false;
        }

        int16_t sample;
        while (file.read(reinterpret_cast<char *>(&sample), sizeof(sample))) {
            audio_data.push_back(sample / 32768.0f); // Normalize 
        }

        file.close();
        return true;
    }

    std::string runTranscription(whisper_context *ctx, const std::vector<float> &audio_data) {
        // Set transcription parameters
        whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);

        params.language = "en"; // Language
        params.n_threads = 4;   // Thread counts

        // Transcription
        if (whisper_full(ctx, params, audio_data.data(), audio_data.size()) != 0) {
            return "Error during transcription.";
        }

        // Retrieve the transcribed data
        int n_segments = whisper_full_n_segments(ctx);
        std::string transcription;
        for (int i = 0; i < n_segments; ++i) {
            transcription += whisper_full_get_segment_text(ctx, i);
        }

        return transcription;
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WhisperASRNode>());
    rclcpp::shutdown();
    return 0;
}