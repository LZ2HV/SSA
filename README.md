	
 Start-Stop memory Module for MQB platforms as VW: T-CROSS, T-ROC, Polo, Taigo.<br/>
 With: PIC16F684 or PIC16F676 or PIC12F683 or PIC12F675, from Microchip and similar. 
 
Requirements to compile and understanding:
1. Compilator, CCS PCWHD v5.115.
2. SCH, Autodesk EAGLE v9.6.1.
3. Simulator for testing, Proteus Professional v8.13.
4. For writing into PIC, PICkit2 or PICkit3.<br/>
For PIC16F684 HEX is "mssa_pic16f684.hex", for PIC12F683 HEX is "mssa_pic12f683.hex",<br/>
for PIC16F676 HEX is "mssa_pic16f676.hex", for PIC12F675 HEX is "mssa_pic12f675.hex".

Connections: For T-Cross (Switch module 1 in centre console, module EX23, 10 pins connector black),
1. LED = PWM signal 200Hz duty cycle 20-90% and or DC signal, from Start/Stop LED, (pin 6).
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
<strong>SMD variant: Gerber file sch/smd/mssaPIC12smd_2026-06-16.zip</strong>
1. All zero resistors are not placed at the beginning (R9, R13, R14).
2. Resistor R9 are placed after successfully programming.  
3. If Button Start-Stop work with GND (T-Cross) Add R14, Remove R10 and R13.
4. If Button Start-Stop work with Plus (some Skoda models) Add R13, Remove R14.<br/>
![Top](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/top_pic.png)
![Bottom](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/bottom_pic.png)<br/>
![Sch](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/Sch.png)

<strong>DIP PIC12 variant: Gerber file sch/mssaPIC12_2026-06-18.zip, This variant work only for Button with GND.</strong><br/>
![Top](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/top12dip.png)
![Bottom](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/bott12dip.png)<br/>
![Sch](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/sch12.png)

<strong>DIP PIC16 variant: Gerber file sch/mssaPIC16_2026-06-18.zip, This variant work only for Button with GND.</strong><br/>
![Top](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/top16dip.png)
![Bottom](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/bott16dip.png)<br/>
![Sch](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/sch16.png)

<strong>First prototype with pic16: In My VW T-Cross 2025</strong><br/>
![Sch](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/prototype.jpg)<br/>
<strong>Second prototype with pic12:</strong><br/>
![Sch](https://github.com/LZ2HV/SSA/blob/70e6b150872ba1d83c70f19b9f05fb2fb6ac7dda/pic/prototype2.jpg)