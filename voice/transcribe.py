import speech_recognition as sr
r = sr.Recognizer()
af = sr.AudioFile('out.wav')
with af as source:
   audio = r.record(source)

x = r.recognize_google(audio)
print('"' + x + '"')

