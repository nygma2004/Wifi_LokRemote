# ESP32 Wifi LocoRemote
This project is a wifi controller for large scale garden railway locomotives. It uses a ESP32 microcontroller to generate a webpage so you can control your loco on from your smartphone, or tablet. This provides and cheaper entry into battery control by not having to purchase a separate transmitter. And of course you are welcome to modify the code for your own scenario.
## Main Features
- Wifi microcontroller generates its own wifi network, or connects to yout home wifi network and hosts the webpage control your loco
- Supply voltage from 6V to 24V
- 3.6A motor controller
- Built in sounds (horns, whistles, bells, station announcement, etc)
- 2 outputs for front and rear headlights that change automatically change based on the direction of travel
- 2 additional auxillary output for lights 
- 2 switch / reed inputs to trigger sound effects
- 2 switch / reed inputs to trigger shuttle or station start/stop mode
- Built in configuration that saved in the local memory

![Prototype setup](/img/with_phone_01.jpg)
You can find a picture of the prototype setup above. On the top left a motor block connected for testing, and a generic 8 ohm speaker. The 4 LED strips simulate the 4 light outputs, two for headlight / tail light and 2 more for other lights. The barel jack provides power to the entire setup, but on a loco that would be a battery pack. The input channels are not connected on this prototype. 

You can also check out my introductory video on this project, which goes through the features implemented:
[![PCB prototype](https://img.youtube.com/vi/NFEwV57PYEw/0.jpg)](https://www.youtube.com/watch?v=NFEwV57PYEw)
## Version History / Upcoming Features / Known Issues
V1.0
- Access Point mode is not implemented yet
- Implement multi language support
## Hardware
The controller is build on a custom PCB that you can order from PCBWay: https://www.pcbway.com/project/shareproject/ESP32_Wifi_LocoRemote.html.
The components used was mostly purchased from Aliexpress, small components like resistors were purchased from local shop.
- ESP32 WROOM-32 model, on a 30-pin development board: https://www.aliexpress.com/item/32819056304.html (this is a 10 pcs listing)
- DRV8871 H-Bridge brushed DC motor driver board: https://www.aliexpress.com/item/4000336358857.html (this is a 10 pcs listing)
- MAX98357 based I2S 3W amplifier board: https://www.aliexpress.com/item/32999952454.html
- Mini DC-DC Buck converter: https://www.aliexpress.com/item/4000076471931.html
- LR7843 Mosfets: https://www.aliexpress.com/item/32871690649.html
- 2.54mm pitch screw terminal blocks (2P, 4P and 6P): https://www.aliexpress.com/item/4000867583795.html
- Pin headers
- 10K, 11K and 100K resistors
## The Controller Board
As mentioned above the Hardware section, you can download my PCB design to build the exact same controller. You can also [find the schematic here](/pcb/Schematic_LocoRemote_v1_0.pdf).

This is how the PCBs look like ordered from PCBWay:
![PCB](/img/pcb01.jpg)

This side shows the microcontroller, the motor driver board and the sound amp board:
![microcontroller side](/img/pcb02.jpg)

And the order side has the mostfets for the outputs and buck converter to power the microcontroller:
![microcontroller side](/img/pcb3.jpg)
