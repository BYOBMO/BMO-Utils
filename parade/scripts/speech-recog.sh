#!/bin/bash

export LD_LIBRARY_PATH="/home/pi/bmos/gv/opt"
export GOOGLE_APPLICATION_CREDENTIALS="/home/pi/bmos/credentials.json"


arecord -d 3 --format=S16_LE --rate=16000 --file-type=raw out.raw

/home/pi/bmos/scripts/google-voice.sh
