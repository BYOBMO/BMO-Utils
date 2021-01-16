# BMO Voice Translation

BMO uses the Google Voice API portion of the Python SpeechRecognition library.

First install the package. I had to increase the swap file size on my Pi Zero because it was running into memory issues during installation.
```
sudo nano /etc/dphys-swapfile
```
Change
```
CONF_SWAPSIZE=100
```
To
```
CONF_SWAPSIZE=2048
```
Then restart the service
```
sudo /etc/init.d/dphys-swapfile stop
sudo /etc/init.d/dphys-swapfile start
```

You should be able to install the package now.
```
pip install SpeechRecognition
```
Once everything is installed it's probably a good idea to set the swap file back to CONF_SWAPSIZE=100

Now you can install the BMO scripts.
```
cd BMO-Utils/voice
mkdir build
cd build
cmake ..
make install
```

To test it out, first record a file. Say something intelligent then hit ctrl-C to stop the recording.
```
arecord -D pcm.micboost -c2 -r 48000 -f S32_LE -t wav -V mono -v out.wav
```

This will create a WAV file called out.wav that the translator will read.
```
/home/pi/bmos/scripts/google-voice.sh
```
You should see a translation of your file printed to the console.

