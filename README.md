	
 Start-Stop memory Module for MQB platforms as VW: T-CROSS, T-ROC, Polo, Taigo.
 PIC16F684 or PIC12F683, and similar. 
 
Requirements to compile and understanding:
1. Compilator, CCS PCWHD v5.115.
2. SCH, Autodesk EAGLE v9.6.1.
3. Simulator for testing, Proteus Professional v8.13.
4. For writing into PIC, PICkit2 or PICkit3.
Current compiled HEX is for PIC16F684.

Connections: For T-Cross (Switch module 1 in centre console, module EX23, 10 pins connector black),
1. LED = PWM signal from Start/Stop LED, 200Hz duty cycle 20-90%  (pin 6).
2. BUT = Button Start/Stop (pin 3).
3. 12V = KL15, ignition (pin 9).
4. GND = GND (pin 1).

How its work:
1. If Start/Stop System is switched to off from ((A) car button), this state will be saved after 1sec.
2. If Start/Stop System is switched to on from ((A) car button), this state will be saved after 4sec.
3. At start ignition saved (Start/Stop System is off) will be returned after 6sec (6sec is default value).
4. If you wanna to change this 6sec, just push once button in the module, module LED, will be flashing 7 times = 7sec,
next bush of the button, will be change to 8sec, module LED, will be flashing 8 times ... (2 to 8sec).
5. Main cicle of the device is indicated from module LED, every 3sec one short flashing.

Schematics and boards is in directory "sch", thers have three variants, two with DIP socket one with SMD elements,
 in down pictures is presented variant with SMD elements (directory sch/smd). 

<strong>Important: If you are not tech savvy, do not attempt this.</strong><br/><br/>
SMD variant: Gerber file sch/smd/mssaPIC12smd_2026-06-16.zip
1. All zero resistors are not placed at the beginning (R9, R13, R14).
2. Resistor R9 are placed after successfully programming.  
3. If Button Start-Stop work with GND (T-Cross) Add R14, Remove R10 and R13.
4. If Button Start-Stop work with Plus (some Skoda models) Add R13, Remove R14.<br/>
![Top](https://github.com/LZ2HV/SSA/blob/5163504e4d249dbd926a3ff0e3a01d93f237d9e3/pic/top_pic.png)
![Bottom](https://github.com/LZ2HV/SSA/blob/ab58418ad07e1950e381db28720cb779d567b959/pic/bottom_pic.png)<br/>
![Sch](https://github.com/LZ2HV/SSA/blob/d15fbafb7d327980c333e8f883bb0fbb9c20ada8/pic/Sch.png).<br/>
DIP PIC12 variant: Gerber file sch/mssaPIC12_2026-06-18.zip, This variant work only with GND.<br/>
![Top](https://github.com/LZ2HV/SSA/blob/b3624d15f0a9bdb992976c37291aef9ca946b0ae/pic/top12dip.png)
![Bottom](https://github.com/LZ2HV/SSA/blob/b3624d15f0a9bdb992976c37291aef9ca946b0ae/pic/bott12dip.png)<br/>
![Sch](https://github.com/LZ2HV/SSA/blob/7a2ee1edfffc32abe9e8e6a552045779106d7128/pic/sch12.png)<br/>
DIP PIC16 variant: Gerber file sch/mssaPIC16_2026-06-18.zip, This variant work only with GND.<br/>
![Top](https://github.com/LZ2HV/SSA/blob/b3624d15f0a9bdb992976c37291aef9ca946b0ae/pic/top16dip.png)
![Bottom](https://github.com/LZ2HV/SSA/blob/b3624d15f0a9bdb992976c37291aef9ca946b0ae/pic/bot16dip.png)<br/>
![Sch](https://github.com/LZ2HV/SSA/blob/7a2ee1edfffc32abe9e8e6a552045779106d7128/pic/sch16.png)<br/>
Prototype: In My VW T-Cross 2025<br/>
![Sch](https://github.com/LZ2HV/SSA/blob/aa41d21a928631c7bed4fb28985d4fc1029c4b20/pic/prototype.jpg)
