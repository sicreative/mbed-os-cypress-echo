# Demo of using Cypress CY8CKIT-062-WIFI-BT PDM and I2S under Mbed  

 Licensed under the Apache License, Version 2.0

Demo for  element14  Connected Cloud Challenge 

The Demo will echo mic sound to sound card 60s, plug a earphone to listen.

Don't set the I2S under ModusToolbox Device Configurator if you use Mbed UART as it will conflict.
Manual load/unload to switch between UART and I2S should work.

More detail: 
https://www.element14.com/community/community/design-challenges/connected-cloud-challenge-with-cypress-and-aws-iot/blog/2020/04/24/echo-the-sound-by-the-pioneer-kit-6

## Quick Install 
1. Install mbed cli
2. Clone the source code and to this directory
3. mbed config root .
4. mbed deploy
6. mbed compile -t GCC_ARM -m CY8CKIT_062_WIFI_BT -f --sterm

