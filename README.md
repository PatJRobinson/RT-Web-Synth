# RT-Web-Synth

A multi-processor project for the NUCLEO-F76ZI and Bela Mini. This project features a real-time digital instrument and a program control interface that can be accessed via any browser.

The project is currently a working prototype, demonstrating the core functionality and design of the system.

The inspiration for this project came from running music workshops and imagining a more versitle, easy-to-use, cost-effective solution to replace the current litany of iPad apps; all from different developers, varying in design and level of polish, each subject to the lack of responsiveness and reliability that often comes with using non-bespoke software/hardware solutions.

This project offers hard real-time audio processing through the bela mini, perameters for which can be selected at ease on a mobile device which connected to the local WIFI. The Web-server downloads this information and transmits it to the Bela Mini over UART, to be interpreted as control information for the patch it is running.

The system could conceivably form part of a DJ or modular setup; it's dual USB and many analog/digital inputs allow for extra sensors or motion-tracking hardware to be connected. The analog outputs offered via the nucleo board will enable the controlling of external modular devices.


Features

Ethernet, CGI, SSI, RMII, Lwip, DHCP, UAR
