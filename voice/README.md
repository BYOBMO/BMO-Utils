# BMO Voice Translation

This is my build of:
https://github.com/GoogleCloudPlatform/cpp-samples/tree/master/speech/api/transcribe.cc

You can build our own version but it's incredibally complicated and tends to fail a lot. It's impossible to build on a Pi Zero since it's so resource intensive. It's the gRPC that's the main problem. There isn't a C++ package available for the Pi. There might be other versions like Python but for voice translation I need as much speed as I can get since there is already some lag in the cloud translation.

You aren't stuck using this particular program either (if you wnat to use Python, go for it!). Any program named transcribe that can take the raw audio and translate it will work. Just put it in the /home/pi/bmos/gv directory and BMO will find it.

To get this version to work:

```
cd BMO-Utils/voice
mkdir build
cd build
cmake ..
make install
```

For this to work you will need Google Speech-To-Text credentials. You will need your credentials in a file called credentials.json in the /home/pi/bmos directory.

You can find out how to create your credentials here:
https://github.com/GoogleCloudPlatform/cpp-samples/tree/master/speech/api#build-and-run

To test you first need an audio file and then process it with the google-voice.sh script.
```
export LD_LIBRARY_PATH="/home/pi/bmos/gv/opt"
export GOOGLE_APPLICATION_CREDENTIALS="/home/pi/bmos/credentials.json"

arecord -d 3 --format=S16_LE --rate=16000 --file-type=raw out.raw

/home/pi/bmos/scripts/google-voice.sh
```
