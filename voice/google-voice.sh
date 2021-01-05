#!/bin/bash

export LD_LIBRARY_PATH="/home/pi/bmos/gv/opt"
export GOOGLE_APPLICATION_CREDENTIALS="/home/pi/Beemo-78546133792b.json"

/home/pi/bmos/gv/transcribe out.raw
rm out.raw