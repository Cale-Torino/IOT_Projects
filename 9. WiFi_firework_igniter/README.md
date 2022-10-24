# DIY battery powered WiFi remote fireworks igniter

Link to my article on my tutorial website [here](https://tutorials.techrad.co.za/2022/10/01/diy-battery-wifi-4ch-relay-pcb).

Creating a WiFi fireworks igniter with a backup LiPo battery.

With December just around the corner I wanted to design a remote fireworks igniter working on WiFi and battery power.

The board will use mosfets as switches to nichrome wire which will heat up and ignite fireworks from a safe distance.

The idea is to have the system self contained with the ability of remote control via a WiFi AP using an ESP8266 12F

Required components:

- 1x R25.30 | 100 x 100 PCB [here](https://www.robotics.org.za/VERO100100)
- 1x R52.90 | ESP8266 12F [here](https://www.robotics.org.za/ESP-12F)
- 4x R11.50 for 2 | IRL520N mosfet [here](https://www.robotics.org.za/IRL520N-TO-220)
- 4x Nichrome wire coils from old vape coils
- 2x R0.322 |R16.10 for a 50 pack | 100k resistor look for 1% [here](https://www.robotics.org.za/RES-100K-50)
- 5x R0.322 | 100k resistor 5% [here](https://www.robotics.org.za/RES-100K-50)
- 4x R0.322 | 10k resistor 5% is fine [here](https://www.robotics.org.za/RES-10K-025)
- 4x R0.322 | 680ohm resistors 5% is fine [here](https://www.robotics.org.za/RES-560E-50)
- 4x R0.92 | R23.00 for 25 pack  | BAT85 Schottky Diodes [here](https://www.robotics.org.za/1N5822-TH)
- 1x 23.00 | Boost Voltage Regulator [here](https://www.robotics.org.za/V228-2A)
- 1x R10.35 | battery holder [here](https://www.robotics.org.za/18650-1C-TH)
- 2x R0.184 | R4.60 25 pack | 100nf capacitors [here]()
- 1x R0.92 | R9.20 for 10 pack | 10uf cap [here]()
- 1x R2.07 | R20.70 for 10 pack | 1000uf cap [here]()
- 1x R25.30 | LiPo charger with protection [here](https://www.robotics.org.za/PK4056) or [R44.85 | LiPo charger & 5v boost](https://www.robotics.org.za/MH-CD42?search=lipo%20charger%203.7v%202a%20in%205v%20%20%20led%20level%20indicator)
- 1x button [here]()
- 1x jumper [here](https://www.robotics.org.za/CCC01505C-20?search=jumper)
- 4x R3.105 | R31.05 for 10 pack | 2 pin screw terminals [here](https://www.robotics.org.za/KF120-2P-254)
- 1x R6.9| R27.60 for 4 pack | 4 pin screw terminal [here](https://www.robotics.org.za/TC-4P-254)
- Female and male headers [here]()

Rough total for 1: R191.038

also get:
- PNP transisters bc557

---

MT3608 3.3v regulator
https://www.robotics.org.za/V228-2A

TP4056 with protection
https://www.robotics.org.za/PK4056?search=lipo%20charger

---





