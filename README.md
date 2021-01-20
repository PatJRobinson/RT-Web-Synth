# RT-Web-Synth

A multi-processor project for the NUCLEO-F76ZI and Bela Mini. This project features a real-time digital instrument and a program control interface that can be accessed via any browser.

The project is currently a working prototype, demonstrating the core functionality and design of the system.
A short video demonstrating the system's core functionality: https://www.youtube.com/watch?v=iRrGBJRCmsc&t=3s

The inspiration for this project came from helping to run music workshops as a volunteer and being dissatisfied with the tech that was being used; an overreliance on iPad apps was causing the instruments available to be unresponsive due to the latency inherent in the system, which led to an overall experience that was not exciting and lacking in engagement. The aim of the project is to create a more versitle, easy-to-use, cost-effective solution as an alternative, using real-time signal processing to improve the quality of instruments used in the sessions.  

In order to make such a device 1) accessible to all users and 2) easily customisable, capable of being 'multiple instruments in one', a suitable software User Interface must be incorperated. Inputs requiring fast, tactile responsiveness - usually those directly causing an auditory response (midi controller or microphone) will still need to be handled by hardware inputs. Thus we have a dual-processor system.

This project offers hard real-time audio processing through the bela mini, perameters for which can be selected at ease on a mobile device which connected to the local WIFI. The Web-server downloads this information and transmits it to the Bela Mini over UART, to be interpreted as control information for the patch it is running.

The system could conceivably form part of a DJ or modular setup; it's dual USB and many analog/digital inputs allow for extra sensors or motion-tracking hardware to be connected. The analog outputs offered via the nucleo board will enable the controlling of external modular devices.






References

Article I used for programming the web server:
http://ausleuchtung.ch/stm32-nucleo-f767zi-web-server/

Series of tutorials for making music programs with Bela:
https://www.youtube.com/watch?v=aVLRUyPBBJk&list=PLCrgFeG6pwQmdbB6l3ehC8oBBZbatVoz3

A series of articles I used when trying to configure the Bela Mini to recieve UART:
https://github.com/BelaPlatform/Bela/wiki/Load-a-device-tree-overlay

https://forum.bela.io/d/622-enabling-uart-device-tree-overlay

https://forum.bela.io/d/479-bela-mini-bela-in-miniature

https://github.com/BelaPlatform/Bela/wiki/Bela-as-a-UART-adapter

https://github.com/ebai101/BelaSerialMidi


A multi-processor project for the NUCLEO-F76ZI and Bela Mini. This project features a real-time digital instrument and a program control interface that can be accessed via any browser.

The inspiration for this project came from helping to run music workshops for a local charity and consistently running into tech-issues due to an overreliance on iPad apps. The aim is to present a more versitle - yet uniform, easy-to-use and cost-effective - solution as an effective alternative. In particular, the real-time kernel featured on the Bela Mini ensures Input/Output latency stays below a specified level (<1ms, depending on project settings), and allows programs to account for jitter, thus allowing the creation of responsive, engaging and fun-to-use instruments.

In order to make such a device 1) accessible to all users and 2) easily customisable, capable of being 'multiple instruments in one', a suitable software User Interface must be incorperated. Inputs requiring fast, tactile responsiveness - usually those directly causing an auditory response (midi controller or microphone) will still need to be handled by hardware inputs. Thus we have a dual-processor system.


