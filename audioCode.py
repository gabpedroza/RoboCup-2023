#import cv2
from vosk import Model, KaldiRecognizer
import pyaudio
import pyttsx3
import numpy
import importlib

#import serial
import time

#ser = serial.Serial('COM5', 115200)
model = Model('vosk-model-small-en-us')
recognizer = KaldiRecognizer(model,16000)
sintetizador = pyttsx3.init()
sintetizador.setProperty('rate', 125)
sintetizador.setProperty('volume', 1.0)
sintetizador.setProperty('voice', 'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_EN-US_ZIRA_11.0')
numeros = {
    'zero': 0,
    'one': 1,
    'two': 2,
    'three': 3,
    'four': 4,
    'five': 5,
    'six': 6,
    'seven': 7,
    'eight': 8,
    'nine': 9,
    'ten': 10,
    'for':4,
    'to':2,
    'sei':6,
    'es':6,
    'free':3,
    'tree':3,
}
mic = pyaudio.PyAudio()
stream = mic.open(format=pyaudio.paInt16,
                  channels=1,
                  rate=16000,
                  input=True,
                  frames_per_buffer=8192)
stream.start_stream()



def fazer_conta(text):
    conta = text.split()
    if(len(conta) < 4): return 2 #if the phrase that it grabs is small, then it doesn't stand a chance(doesn't work)
    j = conta.index('calculate') #grabs the index of the word "calcular"
    #this index is essencial in order to start grabbing the words at the right position
    if conta[1] == 'e' or conta[1] == 'é':
        conta.pop(1)
        print('removido um E aleatório')

    print('-----------------------------')
    print('conta: ', conta)
    print('operação: ', conta[2])
    print(len(conta))
    if len(conta) - j == 4:
        if conta[2+j] == 'plus' or conta[2+j] == 'pais' or conta[2+j] == 'mas': #if its an sum operation
            if conta[1+j] in numeros.keys() and conta[3+j] in numeros.keys(): #sums both numbers
                resposta = (numeros[conta[1+j]] + numeros[conta[3+j]])
                print(resposta)
                resposta = 'The answer is ' + str(resposta)
                sintetizador.say(resposta)#speaks the result
                sintetizador.runAndWait()
                return 1
            else:
                return 2
        elif conta[2+j] == 'minus': #if its an subtraction
            if conta[1+j] in numeros.keys() and conta[3+j] in numeros.keys():
                resposta = (numeros[conta[1+j]] - numeros[conta[3+j]])
                print(resposta)
                resposta = 'The answer is ' + str(resposta)
                sintetizador.say(resposta)
                sintetizador.runAndWait()
                return 1
            else:
                return 2
        elif conta[2+j] == 'times' or conta[2+j] == 'time': #if its an multiplication
            if conta[1+j] in numeros.keys() and conta[3+j] in numeros.keys():
                resposta = (numeros[conta[1+j]] * numeros[conta[3+j]])
                print(resposta)
                resposta = 'the answer is ' + str(resposta)
                sintetizador.say(resposta)
                sintetizador.runAndWait()
                return 1
            else:
                return 2
        else:
            return 2
    elif len(conta)-j == 5:
        if conta[2+j] in ['divided', 'dirigido','dividida']: #sometimes the robot hears the wrong word
            if conta[1+j] in numeros.keys() and conta[4+j] in numeros.keys():
                resposta = (numeros[conta[1+j]] / numeros[conta[4+j]])
                print(resposta)
                resposta = 'the answer is ' + str(round(resposta, 2)) #attempts to aproximate the answer in case the number is irrational
                sintetizador.say(resposta)
                sintetizador.runAndWait()
                return 1
            else:
                return 2
        else:
            return 2
    else:
        return 2
'''
def enviar_arduino(c):
    contador_falha, contador_sucesso = 0, 0  # attempts to send it multple times. If it fails too many times, its an signal to reset and try again
    while contador_falha <= 00 and contador_sucesso < 1:  # that's why I have used counters (that are set for a single "launch"[sends an letter to the arduino],because its reliable)
        try:
            ser.write(c)
            time.sleep(0.1)  # attempts to send the desired character
            contador_sucesso += 1
        except:
            contador_falha += 1
    if (contador_falha > 10):
        return False  # returns "error" if it fails too much
    else:
        return True  # returns "true" if it doesn't fail
    # time.sleep(1)
def rastreador_faces():
    tracker = cv2.TrackerKCF_create()
    # tracker = cv2.TrackerCSRT_create()
    detectorFace = cv2.CascadeClassifier(
        cv2.data.haarcascades + "haarcascade_frontalface_default.xml")  # initializes independent face detector
    video = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    ok, frame = video.read()  # receives the image
    detectedFaces = ()
    while len(detectedFaces) == 0:
        status, image = video.read()  # captures an image
        imageGrey = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)  # transforms the image to greyscale
        detectedFaces = detectorFace.detectMultiScale(imageGrey, scaleFactor=1.5,
                                                      minSize=(150, 150))  # detects faces in the image

    bbox = (detectedFaces[0][0], detectedFaces[0][1], detectedFaces[0][2],
            detectedFaces[0][3])  # we then set the object to be tracked as the face detected
    ok = tracker.init(frame, bbox)  # initialize the tracker
    totalSize = len(frame[0])  # this allows us to see the x-size of the recording
    while True:
        ok, frame = video.read()
        if not ok:  # if video input is over
            break
        ok, bbox = tracker.update(frame)  # updates the frame analyzed
        # print(bbox)
        if ok:
            qttErrors = 0
            x, y, w, h = [int(v) for v in
                          bbox]  # records the tracked object's (x,y) position, its width w and its heigh h
            ser.write(bytes(str(cvt((2 * x + w) / 2 - totalSize / 2, totalSize)),
                            'ascii'))  # here we send the angle the head motor should rotate, based on object position
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)  # draws a rectangle around the object
            # print(ser.readline())
            print(bytes(str(cvt((2 * x + w) / 2 - totalSize / 2, totalSize)), 'ascii'))
        else:  # if it cannot see the object tracked, it shows an error message
            cv2.putText(frame, "Error", (100, 80), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

            qttErrors += 1  # this variable stores how many consecutive frames resulted in a error while tracking
            if (
                    qttErrors >= 50):  # if greater than a certain amount, it restarts the process of selecting something to track
                detectedFaces = ()
                while len(detectedFaces) == 0:
                    status, image = video.read()  # captures an image
                    imageGrey = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)  # transforms the image to greyscale
                    detectedFaces = detectorFace.detectMultiScale(imageGrey, scaleFactor=1.5,
                                                                  minSize=(150, 150))  # detects faces in the image
                # print(detectedFaces)
                bbox = (detectedFaces[0][0], detectedFaces[0][1], detectedFaces[0][2],
                        detectedFaces[0][3])  # we then set the object to be tracked as the face detected
                tracker = cv2.TrackerKCF_create()
                tracker.init(frame, bbox)
                qttErrors = 0
        cv2.imshow('Tracking', frame)  # shows the frame
        if cv2.waitKey(1) & 0XFF == 27:  # pressing ESC stops code
            break
'''
def falar_palavra(palavra):
    try:
        sintetizador.say(palavra) #attempts to speak the word
        sintetizador.runAndWait()
        return 1
    except:
        return 3 #if it fails, it returns "grave failure"

def reconhecer_frase():
    if (1): #this "if"  is here only to have the correct identification
        print("Fale...")
        try:
            data = stream.read(4096)

            if recognizer.AcceptWaveform(data): #if the voice recognition is acceptable
                text = recognizer.Result()
                text = text[14:-3] #this string cut is here to remove  a bunch of random characters that come along
                return text #returns the text that was read

            else:
                return 'problemaComum' #if it doesn't recognize what was said it's an " comum error"
        except:
            return 'problemaReset' #if its fails on the "try", its an "grave error"




