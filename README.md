# ESP DOMOTIC SYSTEM
ESP Domotic System is a simple domotic system to control sensors and domotic items of a small house. It is associated to a web app that allows to check real-time graphs of power consumption and sensor readings. The web app also allows to turn on and off domotic items such as lights or fans. This project exploits the concept of embedded systems to simplify control of a small house.
### Schematic

 // schematic of circuit

 ## Requirements 

 ### Hardware requirements
 ESP Domotyc System requires:
 * Texas Instruments MSP432P401R with BoosterPack
 * 2 x ESP32
 * Relay module
 * Matrix Board and voltage divider
 * INMP441 microphone

### Software requirements
You will need [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) installed with [DriverLib](https://www.ti.com/tool/MSPDRIVERLIB) to compile and run the code on the MSP. You will also need [Arduino IDE 2.x](https://www.arduino.cc/en/software) to run the ESPs. 

## Project Layout

```

├── esp_main                
│   ├── esp_main_code        # main code to be run on the main esp
│   ├── esp_main_data        # data of the web server held on the esp
├── esp_audio
│   ├── ...
│   ├── ...
├── msp432                   # project to be imported in CodeComposerStudio
├── house_model              # 3D model to print house model anb brackets for components
```

## Build and Run the project

After connecting the devices as indicated in the [schematic](#schematic), you have to upload programs on each board.
Starting from the MSP432:
* Open CodeComposerStudio
* Go to **File** > **Open Projects from File Systems**
* In **Import Projects from File Systems** search and select the project.
  
> You will also need DriverLib that can be installed from the link above

For the main ESP: 
* Open Arduino IDE 2.x
* Go to **File** > **Open** and select the .ino file

