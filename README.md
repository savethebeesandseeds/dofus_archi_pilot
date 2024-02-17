# Dofus Archi Pilot
This is a bot for the game Dofus, it helps in the guigantic task of capturing all the Archimonsters. 

Enjoy playing and don't be a Snitch. 

### How it works
Autopiloting Dragoturkeys are used to drive all over the map, in schedules defined in *./data/schedules/*. 

While the character moves the folder *./data/temp* is filled with images and image file *./data/archi.bmp* is used for *template matching recognition* with *OpenCV*. It plays a loud sounf if there are any archimonsters in the current map. 


### Requirements
In the game you must own an Autopiloting Dragoturkey. 

### Safety
Relax. This will not ask you for email, password or anything.

### Instructions
1. Start playing Dofus, mount an Autopiloting Dragoturkey. 

2. Take a look at configuration file *./build/.config* and change the config setting ```window``` to your character name. And change the config setting ```schedule``` to select the route you want to take and explore.

3. Execute the file *./build/archi_pilot.exe* 

You will see that the player starts moving, and whenever you hear a loud sound is becouse it has found and archimonster. The character will continue to move but you can go to folder *./data/archis/* and see the location. 

It's very simple, it's very stable, and it works in Windows.

### Debuging the .config file
The current *./build.config* file, works for me, but it might not work for you. 

Test changing these: 

1. If the resolution of your screen is lower, then ``box_change_x, box_change_y`` might be different. The idea here is to capture a portion of the interface minimap. Just take a screenshot of your Dofus game and open it in paint. Check where the minimap starts and replaces those coordinates there instead. 

2. If you feel the code is running very slow, lower the value ```down_size_coef```.

3. If the algorithm is incorrect in identifying new archis, you might want to change the configuration setting ```threshold``` or find an archi, take a screenshot, crop the part that has the logo of the archi as in *./data/archi.bmp* and cover the backgound with black #000000

### Build from source instructions
Follow the instructions in file *./deploy.md*
