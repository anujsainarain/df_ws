#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp> 
#include <vector>
#include <fstream>
#include <string>
#include "whisper.h"
#include <unordered_set>
#include <filesystem> 
namespace fs = std::filesystem;


class WhisperASRNode : public rclcpp::Node {
public:
    WhisperASRNode() : Node("whisper_asr_node") {
        publisher_ = this->create_publisher<std_msgs::msg::String>("recognized_speech", 10);

        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "/audio_file", 10,
            std::bind(&WhisperASRNode::setAudioFile, this, std::placeholders::_1));

        timer_ = this->create_wall_timer(
            std::chrono::seconds(10),
            std::bind(&WhisperASRNode::processAudio, this));
    }

private:
    std::unordered_set<std::string> processed_files;
    std::string model_path = "/root/df_ws/src/whisper_asr_node/whisper_cpp/models/ggml-base.bin";
    std::string audio_dir = "/root/df_ws/src/whisper_asr_node/audio";
    std::string audio_path;

    void setAudioFile(const std_msgs::msg::String::SharedPtr msg) {
        // std::string file_path = msg->data;
        std::string file_path = audio_dir + "/" + msg->data + ".wav";
        if (fs::exists(file_path) && fs::path(file_path).extension() == ".wav") {
            audio_path = file_path;
            RCLCPP_INFO(this->get_logger(), "Audio file set to: %s", audio_path.c_str());
        } else {
            RCLCPP_ERROR(this->get_logger(), "Invalid file path: %s", file_path.c_str());
        }
    }

    void processAudio() {
        // std::string model_path = "/root/df_ws/src/whisper_asr_node/whisper_cpp/models/ggml-base.bin";
        // std::string audio_path = "/root/df_ws/src/whisper_asr_node/audio/Food.wav";
        // std::string audio_dir = "/root/df_ws/src/whisper_asr_node/audio";
        std::vector<std::string> wav_files;
        
        for (const auto &entry : fs::directory_iterator(audio_dir)) {
            if (entry.path().extension() == ".wav") {
                wav_files.push_back(entry.path().string());
            }
        }

        // No .wav files
        if (wav_files.empty()) {
            RCLCPP_INFO(this->get_logger(), "No .wav files found.");
            return;
        }

        // One .wav file is present
        if (wav_files.size() == 1) {
            audio_path = wav_files[0];
        } 

         // Check if an audio file is selected
        if (audio_path.empty()) {
            RCLCPP_INFO(this->get_logger(), "waiting at /audio_file topic.");
            return;
        }
        


        // One .wav file is present
        // if (wav_files.size() == 1) {
        //     audio_path = wav_files[0];
        // } 
        // else { // Show all .wav preesent
        //     RCLCPP_INFO(this->get_logger(), "Present .wav files:");
        //     for (size_t i = 0; i < wav_files.size(); ++i) {
        //         RCLCPP_INFO(this->get_logger(), "[%zu] %s", i + 1, wav_files[i].c_str());
        //     }

        //     // Display audio files
        //     std::cout << "Audio files:" << std::endl;
        //     for (size_t i = 0; i < wav_files.size(); ++i) {
        //         std::cout << (i + 1) << ". " << wav_files[i] << std::endl;
        //     }
        //     // To choose one
        //     int choice = 0;
        //     do {
        //         std::cout << "Which Audio file? Please pick from 1-" << wav_files.size() << ". Your choice is: ";
        //         std::cin >> choice;
        //         if (choice < 1 || choice > wav_files.size()) {
        //             RCLCPP_ERROR(this->get_logger(), "Invalid choice. Try again.");
        //         }
        //     } while (choice < 1 || choice > wav_files.size());

        //     audio_path = wav_files[choice - 1];
        // }

        if (processed_files.count(audio_path)) {
            // RCLCPP_INFO(this->get_logger(), "Audio file already processed: %s", audio_path.c_str());
            rclcpp::shutdown();
        }

        processed_files.insert(audio_path);

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

        // Transcribing the audio
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
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WhisperASRNode>());
    rclcpp::shutdown();
    return 0;
}