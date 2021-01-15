# RetroPie Extensions

## Splashscreen
Copy the BMOS splash screen to the RetroPie splash screen directory:
```
sudo cp -r BMO-Utils/RetroPie/splashscreens/moco.png /home/pi/RetroPie/splashscreens
```
You can activate it in the RetroPie setup script.

## Theme
Copy the theme files to the theme directory for Emulation Station.
```
sudo cp -r BMO-Utils/RetroPie/bmos-theme/ /etc/emulationstation/themes/
```

## Emulator
For custom SDL games set up an emulator entry to launch the scripts. Load your launch scripts into the roms directory for RetroPie and they should show up in the game menu under BMOS.
```
sudo nano /etc/emulationstation/es_systems.cfg
```
Then add the following block to the emulator config:
```
  <system>
    <name>BMOS</name>
    <fullname>BMO Games</fullname>
    <path>/home/pi/RetroPie/roms/bmos</path>
    <extension>.sh .SH</extension>
    <command>%ROM%</command>
    <theme>bmos</theme>
  </system>
```
