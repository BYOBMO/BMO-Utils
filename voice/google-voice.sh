#!/bin/bash

#export LD_LIBRARY_PATH="/home/pi/bmos/gv/opt"
#export GOOGLE_APPLICATION_CREDENTIALS="/home/pi/bmos/credentials.json"

python3 /home/pi/bmos/gv/transcribe.py
rm out.wav
