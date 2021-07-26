# SMART SAFETY JACKET FOR INFANTS
![alt text](https://github.com/MohamedOsamaAhmed/SMART-SAFETY-JACKET-FOR-INFANTS/blob/main/1.JPG?raw=true)

## Summary
system for ensuring safety for small baby within the home when the mother/guardian is busy with their stuff. Guarantee different detections like fire, gas leakage, temperature and the movement of the baby. proposed system is to make some improvements Global system for mobile communication (GSM) and receiver include parent’s mobile phone which is assigned for monitoring the surrounding conditions of the baby.

## Main Functionality 
This system can measure sound to detect if baby cries or not, temperature of the baby, detect if there are fire, smoke, or object nearby the baby. It is consisting of two microcontrollers master microcontrollers (Arduino Nano) and slave microcontroller (STM32) they are communicating through Bluetooth.
![alt text](https://github.com/MohamedOsamaAhmed/SMART-SAFETY-JACKET-FOR-INFANTS/blob/main/main%20function.JPG?raw=true)

## Hardware components 
•	Arduino Nano (Microcontroller)
•	STM32 (Microcontroller)
•	DHT11 (Temperature Sensor)
•	MQ2 (Gas Sensor)
•	Flame Detection Sensor Module (Fire Sensor)
•	DAOKI 5PCS High Sensitivity Sound Microphone Sensor
•	TCRT5000 infrared reflectance sensor
•	Rain Sensor 
•	Micro SD TF Memory Card Shield Module SPI Micro SD
•	SIM800C (GSM Module) 
•	2- Bluetooth Module HC05

## Software
•	Arduino IDE
•	Eclipse
•	STM32 ST-LINK Utility

## Programming Languages 
•	C programming Language
•	Arduino 

## Connections 
There is no need to show all connections to make a copy of them they are very easy so I’ll show only some of them like 
    **Micro SD Card Module:** 
    ![alt text](https://github.com/MohamedOsamaAhmed/SMART-SAFETY-JACKET-FOR-INFANTS/blob/main/SDCard%20connections.png?raw=true)
    **Speaker** 
    I used transistor to get high audio from the Arduino and connected it in switch mode. To learn how to run speaker with Micro SD Card reader you can read this topic:
    https://www.instructables.com/Audio-Player-Using-Arduino-With-Micro-SD-Card/
    **Bluetooth module**
        In STM:
            it is connected directly to any UART Peripheral as STM has enough UARTS
	    In Arduino
			I used software serial as I have one Tx and Rx I used them for debugging 
        You can see how to use serial software form here  https://www.arduino.cc/en/Tutorial/LibraryExamples/SoftwareSerialExample
        **GSM** 
			All about GSM you can know from here 
			https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial/
            don’t forget to connect DTR pin like me and waste much time for this silly mistake.
            I also used Software serial for this module
        **DHT** 
			To learn how to run this module and how does it work from here:
            https://create.arduino.cc/projecthub/pibots555/how-to-connect-dht11-sensor-with-arduino-uno-f4d239
            also read this if any problem faced you during using DHT library 
            https://forum.arduino.cc/t/compile-fatal-error-adafruit_sensor-h-no-such-file/452640/2
        
## Code Issues 
>1-	Used threshold values are not constant values they may differ from one device to other you can try and specify your suitable value.
>2-	When you use software serial Arduino allows you to use one at a time so you should use listen() function when you use any one of them.
>3-	During communication with STM I wait till it responds me to complete the process of the program so code stuck in loop till function read returns anything rather than -1

## Testing and Connections
You can watch connections explanation and project testing from here 
	https://youtu.be/etyl6wq8ejY

### For STM32 Code you will find it here 
https://github.com/safety-jacket/stm
it is written by Eng Ahmed Hassan


    


