#!/bin/bash

export LD_LIBRARY_PATH="/home/pi/bmos/gv/opt"
export GOOGLE_APPLICATION_CREDENTIALS="/home/pi/bmos/credentials.json"

/home/pi/bmos/gv/transcribe out.raw
rm out.raw
