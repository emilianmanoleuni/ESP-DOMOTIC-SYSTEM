from google.cloud import speech_v1p1beta1 as speech
from flask import Flask, request, jsonify, make_response
import os


# Codice del server Flask qui...   

app = Flask(__name__)
file_name = "./resources/recording.wav"


def speech_to_text_api():    
    client = speech.SpeechClient.from_service_account_file('key.json')
    # Legge il file audio e lo converte in base64
    with open(file_name, 'rb') as audio_file:        audio_content = audio_file.read()
    # Configurazione per il riconoscimento del parlato
    config = {        
       "encoding": speech.RecognitionConfig.AudioEncoding.LINEAR16,
        "sample_rate_hertz": 16000,
        "audio_channel_count": 1,        
        "language_code": "en-US"
    }
    # Creazione dell'oggetto RecognitionAudio    
    audio = speech.RecognitionAudio(content=audio_content)
    # Creazione dell'oggetto RecognitionConfig
    recognition_config = speech.RecognitionConfig(**config)
    # Esegue il riconoscimento del parlato    
    response = client.recognize(config=recognition_config, audio=audio)
    # Estrae la trascrizione dal risultato
    transcription = "\n".join(result.alternatives[0].transcript for result in response.results)
    print(f'Trascrizione: {transcription}') 

    response_message = Handle_Message(transcription)
    response = make_response(jsonify({'message': response_message}), 200)
    response.headers['Content-Type'] = 'application/json'
    return response


def Handle_Message(transcription):
    message = 0

    if 'Bathroom' in transcription or'bathroom' in transcription:
        if ('lights'  in transcription or 'light'  in transcription or 'Light'  in transcription or 'Lughts'  in transcription):
            if 'on' in transcription:
                message = 9
            elif 'off' in transcription:
                message = 10 

    if 'Living' in transcription or 'living' in transcription:
        if ('lights'  in transcription or 'light'  in transcription or 'Light'  in transcription or 'Lughts'  in transcription):
            if 'on' in transcription:
                message = 1 
            elif 'off' in transcription:
                message = 2 

    if 'Kitchen' in transcription or 'kitchen' in transcription:
        if ('lights'  in transcription or 'light'  in transcription or 'Light'  in transcription or 'Lughts'  in transcription):
            if 'on' in transcription:
                message = 3 
            elif 'off' in transcription:
                message = 4 

    if 'Hallway' in transcription or 'hallway' in transcription:
        if ('lights'  in transcription or 'light'  in transcription or 'Light'  in transcription or 'Lughts'  in transcription):
            if 'on' in transcription:
                message = 5 
            elif 'off' in transcription:
                message = 6

    if 'Bedroom' in transcription or 'bedroom'in transcription:
        if ('lights'  in transcription or 'light'  in transcription or 'Light'  in transcription or 'Lughts'  in transcription):
            if 'on' in transcription:
                message = 7
            elif 'off' in transcription:
                message = 8 


    elif ('fans' in transcription or 'fan'  in transcription or 'Fan'  in transcription or 'Fans'  in transcription):
        if 'on' in transcription:
            message =  11
        elif 'off' in transcription:
            message = 12 

    print(message)
    return message

@app.route('/uploadAudio', methods=['POST'])
def upload_audio():   
    if request.method == 'POST':
        print('Data arrived')  # Salva il file audio
        with open(file_name, 'wb') as f:            
            f.write(request.data)
        # Trascrivi l'audio
        transcription = speech_to_text_api()
        return transcription, 200
    return 'Errore: Controllare la richiesta POST', 405

if __name__=="__main__":
 app.run(host='0.0.0.0', port=8888, debug=True)
