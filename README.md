# RT-Web-Synth

A multi-processor project for the NUCLEO-F76ZI and Bela Mini. This project features a real-time digital instrument and a program control interface that can be accessed via any browser.

The project is currently a working prototype, demonstrating the core functionality and design of the system.

The inspiration for this project came from helping to run music workshops as a volunteer ... the aim is to create a more versitle, easy-to-use, cost-effective solution as an effective alternative. The tactile, immediate still gets RT  ....    customisable UI, easy to use for all ... 

This project offers hard real-time audio processing through the bela mini, perameters for which can be selected at ease on a mobile device which connected to the local WIFI. The Web-server downloads this information and transmits it to the Bela Mini over UART, to be interpreted as control information for the patch it is running.

The system could conceivably form part of a DJ or modular setup; it's dual USB and many analog/digital inputs allow for extra sensors or motion-tracking hardware to be connected. The analog outputs offered via the nucleo board will enable the controlling of external modular devices.


Features - diagram of full setup


Bela Mini
 - latency (watch intro vid)
 Device tree overlay - link
 
 Nucleo F767ZI
 - ethernet - link
 - analog outputs

LWIP

Ethernet, CGI, SSI, RMII, Lwip, DHCP, UART
