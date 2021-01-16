# RetroGame Configuration

Let's get those front panel buttons working.

First you need to install the AdaFruit retrogame project.
```
curl https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/retrogame.sh >retrogame.sh
sudo bash retrogame.sh
```
This will start asking you questions. Mainly, what type of system are you configuring. There is no BMO option so I just choose item 6 (Six buttons + joystick) since that seems closest to what we want. In reality it doesn't matter since we are going to replace all of that in a second.

This will create a file called /boot/retrogame.cfg which has all the button mappings in it. Replace that with the new one.
```
sudo cp /home/pi/git/BMO-Utils/retrogame-config/retrogame.cfg /boot/retrogame.cfg
sudo reboot
```
You should get the following mappings for BMO:

* DPad: Up Arrow/Down Arrow/Left Arrow/Right Arrow
* Blue Button: Shift
* Green Button: Alt
* Red Button: Ctrl

For more info see: https://learn.adafruit.com/retro-gaming-with-raspberry-pi/adding-controls-software
