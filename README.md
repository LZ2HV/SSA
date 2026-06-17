	
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
3. 12V = KL15 (pin 9).
4. GND = GND (pin 1).

	Important:
	If you are not tech savvy, do not attempt this.

![Top](https://github.com/LZ2HV/SSA/blob/5163504e4d249dbd926a3ff0e3a01d93f237d9e3/pic/top_pic.png)
![Bottom](https://github.com/LZ2HV/SSA/blob/ab58418ad07e1950e381db28720cb779d567b959/pic/bottom_pic.png)
![Sch](https://github.com/LZ2HV/SSA/blob/d15fbafb7d327980c333e8f883bb0fbb9c20ada8/pic/Sch.png)
