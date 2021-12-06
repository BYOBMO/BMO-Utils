# Audio Setup

To activate the I2S speakers and microphone run the pizeroaudio.sh script
```
cd BMO-Utils/audio
sudo ./pizeroaudio.sh
```
Now reboot. Let the script do it or do it from the command line.
```
sudo reboot
```
To get the audio settings finalized you have to play some audio. It's a quirk of the Pi audio system.

```
cd BMO-Utils/audio
./record.sh
```
Say something and then hit ctrl-C to end the recording. You should now have a file.wav file. You can play it back with aplay.
```
aplay out.wav
```

It might sound good. It might sound bad. Adjusting the settings with alsamixer should clean things up.
```
alsamixer
```

In alsamixer you can adjust the speaker volume and the microphone gain. The speakers should be set to around 80-90% for best results. The microphone must be set to less than 55%. Anything higher than that will cause distortion or clipping and you probably won't hear anything. 
