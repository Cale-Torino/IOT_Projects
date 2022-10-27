# Simple Dying Gasp Circuit

large capacitor to supply enough current and voltage in order to send a last message (Dying Gasp) once the input power has been cutoff.

Once power is cut a transistor or relay can pull a pin low in order to signal the microcontroller that an power event has occurred.

There are dedicated IC's for this however in my case they are not commonly available.

Also if a supercapacitor that is normally charged by the system when it is running is connected it to a normally closed pole of a relay and mains power is lost the relay de-energizes then closes that pole, once that happens the system continues to run from the supercapacitor as a means of battery backup.

links
- [Dying Gasp Circuit Design on PCBA Level](https://medium.com/a-hardware-engineers-job-record/dying-gasp-circuit-design-on-pcba-level-27c6f9ecaead)
- ["Dying Gasp" from a Circuit Standpoint](https://www.mathscinotes.com/2011/02/dying-gasp-from-a-circuit-standpoint/)
- [Battery powered auto backup circuit](https://www.gadgetronicx.com/battery-auto-backup-circuit-dc/)

Old component list:
- 18650 battery holder
- 18650 battery
- Li-ion charger DC step up
- PCB
- Output barrel jack male + 2 core stranded wire