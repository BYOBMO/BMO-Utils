import speech_recognition as sr
r = sr.Recognizer()
try:
   af = sr.AudioFile('out.wav')
   with af as source:
      audio = r.record(source)

   try:
      x = r.recognize_google(audio)
      print('"' + x + '"')
   except sr.UnknownValueError:
       print("ERR: Google Speech Recognition could not understand audio")
   except sr.RequestError as e:
       print("ERR: Could not request results from Google Speech Recognition service; {0}".format(e))


except sr.RequestError as e:
   print("ERR: audio file; {0}".format(e))

