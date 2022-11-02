# DIY WiFi power meter and logger

Link to my article on my tutorial website [here]().

Creating a wifi power monitoring and logging device with a LiPo battery.

Connect to device and see the data via wifi, view on LCD or extract from SD card.

Device must use `Wi-Fi station` or `Wi-Fi Access Point`. 

Rather use station mode to connect to device via wifi from further distances.

Removed programming components since the esp8366 can be plugged in via headers.

required components:

- 1x Esp8266
- ESP8266 components
    - 1x 100k 1% resistor
    - 1x 330k 1% resistor
    - 4x 100k 5% resistors
    - 1x 10k 5% resistor
    - 1x 100nf cap
    - 1x push button
    - 1x 3pin male header for flash mode or dip 3 point connector
    - 1x 4pin male header for programming
- 3.3v power
    - 1x AMS1117
    - 2x 10uf cap
    - 2x 100nf cap
- 1x Oled Lcd I2C
- 1x SD card reader SPI
- 1x Lipo charging and 5v boost pcb
- 1x 18650 Battery holder
- 1x INA219 current volts sensor I2C
- 1x Screw terminal 2 pin blue 5mm 16A
- 1x On off switch
- 1x 1000uf cap
- 1x 100 x 100 pcb

