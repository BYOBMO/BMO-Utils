# Servo Setup

To get the servos working you need to enable I2C support in raspi-config.
```
sudo raspi-config
```

* Choose "Interfacing Options".
* Select I2C
* Select yes when asked to enable it.
* Reboot.


## PCA9685
First, make sure your installation has the required files to support compiling for the PCA9685 I2C chip.
```
cd ~
sudo apt-get install wiringpi
git clone https://github.com/Reinbert/pca9685
cd pca9685/src
sudo make install
```

Go back to where you cloned https://github.com/BYOBMO/BMO-Utils.git
In my case it's /home/pi/git so:
```
cd /home/pi/git
```

## Servos
```
cd ./BMO-Utils/servos
mkdir build
cd build
cmake ..
make
make install
```

This should install the arms program into the /home/pi/bmos/servos directory.

Usage:
```
/home/pi/bmos/servos/arms [arm] [action] [delay]

arm: l|r|b (Specifies left, right, or both arms for the action)
action: u|d|w|p (up, down, wave, point)
delay: n (Integer number. Default: 7. 0=full speed, increasing the number slows it down)
```
Example: Wave both arms very fast.
```
~/bmos/servos/arms b w 2
```

Example: Point left arm at default speed.
```
~/bmos/servos/arms l p
```

Example: Lower both arms then raise right arm very slowly.
```
~/bmos/servos/arms b d
~/bmos/servos/arms r u 25
```

## limits.txt
Since BMO's range of motion is less than 180 degrees, the servo limits are stored in /home/pi/bmos/servos/limits.txt. This file is read everytime the arm command is made and prevents BMO from over-swinging. You could probably make BMO crawl however if you overswing in the downward position. 

The format is pretty simple:
```
left: [down] [up]
right: [up] [down]
```
For example:
```
left: 20 155
right: 25 150
```
The values are in degrees.

# position.txt
Servos like to move violently to whatever position that is requested. That can make BMO a little bit agressive. To fix this, the last requested position is stored in /home/pi/bmos/servos/position.txt. This allows the program to start its calculations from the last known position. You can read the last position directly from the PCA9685 chip but this only works if the chip hasn't been reset which happens every time you reboot. 

There are come cool servos with a fourth wire that allow you to read the position of the servo in realtime but they are slightly too big for BMO-Boy and would require the addition of an ADC. 

https://www.adafruit.com/product/1450
