from gtts import gTTS
from pydub import AudioSegment
import os
import sys

def main():
    if len(sys.argv) < 3:
        # print("Usage: tts_inference.py '<text>' '<output_path>'")
        sys.exit(1)

    text = sys.argv[1]
    output_path = sys.argv[2]

    try:
        # gTTS
        tts = gTTS(text=text, lang='en')
        temp_mp3 = "/tmp/temp_output.mp3" 
        tts.save(temp_mp3)

        # Convert MP3 to WAV
        audio = AudioSegment.from_mp3(temp_mp3)
        audio.export(output_path, format="wav")

        # Audio testing
        # os.system(f"aplay {output_path}")

        print(f"Done and saved")
    except Exception as e:
        print(f"Didn't work; TTS")

if __name__ == "__main__":
    main()
