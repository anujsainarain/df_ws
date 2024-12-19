import os
import sys
from ctransformers import AutoModelForCausalLM

script_dir = os.path.dirname(os.path.realpath(__file__))
models_dir = os.path.join(script_dir, "../models")

# Load the pre-trained model
llm = AutoModelForCausalLM.from_pretrained(
    models_dir,
    model_file="llama-2-7b-chat.Q5_K_M.gguf"
)

# Mock LLMs with placeholder values
llm_metadata = {
    'llm_1': {'latency': 0.6, 'sentiment_score': 0.9, 'relevance_score': 0.8},
    'llm_2': {'latency': 0.7, 'sentiment_score': 0.9, 'relevance_score': 0.9},
    'llm_3': {'latency': 0.4, 'sentiment_score': 0.55, 'relevance_score': 0.7}
}

# Function to generate a response using the loaded model
def generate_response(prompt: str) -> str:
    return llm(prompt)

# Fn to select the best LLM for the task at hand (Rephrase this Anuj)
def select_best_llm(input_length: int):
    if input_length <= 15:                      # Inputs with 15 words or fewer
        # Select LLM with the lowest latency and a tie-breaker on highest relevance score (llm_3)
        selected_llm = min(
            llm_metadata,
            key=lambda x: (llm_metadata[x]['latency'], -llm_metadata[x]['relevance_score'])
        )
    elif 15 < input_length <= 25:               # Inputs of 16-25 words
        # Select LLM with the highest sentiment score and a tie-breaker on lowest latency (llm_1)
        selected_llm = max(
            llm_metadata,
            key=lambda x: (llm_metadata[x]['sentiment_score'], -llm_metadata[x]['latency'])
        )
    else:
        # Select LLM with the highest sentiment score and a tie-breaker on highest relevance score (llm_2)
        selected_llm = max(
            llm_metadata,
            key=lambda x: (llm_metadata[x]['sentiment_score'], llm_metadata[x]['relevance_score'])
        )
    return selected_llm


# Function to determine the system message based on the selected LLM
def get_system_message(selected_llm: str) -> str:
    system_messages = {
        'llm_1': "You are an AI assistant. You are fun to talk to and respond to the point. You respond in 1-2 sentences.",
        'llm_2': "You are an AI assistant and you are fun to talk to. You respond in 2-3 sentences.",
        'llm_3': "You are an AI assistant. You are fun to talk to but are quite insightful. You respond in 3-4 sentences."
    }
    return system_messages.get(selected_llm)

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 llm_inference.py <instruction>")
        sys.exit(1)
    
    instruction = sys.argv[1]
    input_length = len(instruction.split()) 
    selected_llm = select_best_llm(input_length)
    system = get_system_message(selected_llm)                       # Essentially selects the LLM by setting the persona and constraints for the LLM.

    prompt = f"<s>[INST] <<SYS>>\n{system}\n<</SYS>>\n\n{instruction} [/INST]"

    # Generate response using the selected LLM
    response = generate_response(prompt)

    # Print out LLM response
    # print(f"Selected LLM: {selected_llm}")                        # Uncomment to see the LLM Selected
    print(response)

if __name__ == "__main__":
    main()
