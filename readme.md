# ROS 2 Dockerized Conversational AI System

This repository contains a ROS 2 workspace (`df_ws`) designed to handle conversational AI tasks using Docker containers. The system comprises three main nodes:
- **Whisper ASR Node**: Handles audio transcription.
- **Decision Maker Node**: Processes the transcription and generates a response using an LLM.
- **Google TTS Node**: Converts the response text to speech.

---

## **How to Run**

1. **Git clone the repository**
    ``bash
    git clone https://github.com/anujsainarain/df_ws.git
    ```

1. **Build the Docker Compose Configuration**:
   ```bash
   cd ~/df_ws/
   docker-compose build
   ```

2. **Start the docker containers**:
   ```bash
   docker-compose build
   ```

Doing the above would spin the three nodes : `whisper_asr_node`, `decision_maker_node`, and `google_tts_node`

 - Whisper ASR Node: Transcribes the audio from the Food.wav file. 
        Note: Replace the `Food.wav` with another `.wav` file to transcribe a different audio. Use `https://www.narakeet.com/create/text-to-wav.html#trynow` to generate your own `.wav` file. Once you do that, rebuild and run the `docker-compose.yaml`.

 - Decision Maker Node: Processes the transcribed audio. It uses LLaMa 7B variant to implement a chatbot and has placeholders to use prompts for different LLM models. In placeholders, it uses different criterias `latency`,`sentiment_score`, and `relevance_score` to set a different persona and contraints to the LLaMa being used.
 - Google TTS Node: Generates a synthesized audio response. 
        Note: It can be retrieved using `docker cp`.


## Retrieve the Output
After the nodes have finished running copy the generated audio from the `google_tts_node" container to your system:
    ```bash
    docker cp <container_id>:/root/df_ws/src/google_tts_node/output/docker_output.wav ./src/google_tts_node/output/
    ```

    Where, <container_id> is the Container ID of the google_tts_node container.