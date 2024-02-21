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
1. Open CodeComposerStudio
2. Go to **File** > **Open Projects from File Systems**
3. In **Import Projects from File Systems** search and select the project.

To add the DriverLib:
1. Go to **Build** > **Arm Linker** > **File Search Path** and add the paths where you installed the library

Once you have done that you can right-click on the main project folder and click on **Build Project**, once ended you can click **Flash** on CCS to upload the code on the MSP432.


For the main ESP: 
1. Open Arduino IDE 2.x
2. Select the board "ESP32 dev module"
3. Go to **File** > **Open** and select the .ino file

To import the libraries: 
///

## User Guide 
 // how to connect to the web app

 // pre-imposted voice command

## Contributors

<table>
    <tr>
        <td>House model and cabling</td>
        <td>Emilian Manole</td>
    </tr>
    <tr>
        <td>Web app and Async server</td>
        <td>Emilian Manole</td>
    </tr>
    <tr>
        <td>MSP432</td>
        <td>Leonardo Rigotti</td>
    </tr>
    <tr>
        <td>UART communication</td>
        <td>Leonardo Rigotti</td>
    </tr>
    <tr>
        <td>First ESP</td>
        <td>Emilian Manole, Leonardo Rigotti</td>
    </tr>
    <tr>
        <td>Second ESP</td>
        <td>Elia Avanzolini, Riccardo Randon</td>
    </tr>
    <tr>
        <td>Voice Module</td>
        <td>Elia Avanzolini, Riccardo Randon</td>
    </tr>
</table>

