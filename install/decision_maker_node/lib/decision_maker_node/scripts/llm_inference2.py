from huggingface_hub import InferenceClient

import json


repo_id = "TinyLlama/TinyLlama-1.1B-Chat-v1.0"


llm_client = InferenceClient(

    model=repo_id,

    timeout=120,

)


def call_llm(inference_client: InferenceClient, prompt: str):

    response = inference_client.post(

        json={

            "inputs": prompt,

            "parameters": {"max_new_tokens": 200},

            "task": "text-generation",

        },

    )

    return json.loads(response.decode())[0]["generated_text"]



# response=call_llm(llm_client, "write me a crazy joke")
response1=call_llm(llm_client, "write me a crazy joke")

print (response1)