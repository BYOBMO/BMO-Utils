# RetroPie Extensions

##Splashscreen

##Theme
sudo cp -r BMO-Utils/RetroPie/bmos-theme/ /etc/emulationstation/themes/

sudo nano /etc/emulationstation/es_systems.cfg
  <system>
    <name>BMOS</name>
    <fullname>BMO Games</fullname>
    <path>/home/pi/RetroPie/roms/bmos</path>
    <extension>.sh .SH</extension>
    <command>%ROM%</command>
    <theme>bmos</theme>
  </system>
