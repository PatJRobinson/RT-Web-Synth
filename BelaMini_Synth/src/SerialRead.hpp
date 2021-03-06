/*
* SerialRead.hpp
* Created by Ethan Bailey, 03/14/20
*/

#ifndef SerialRead_hpp
#define SerialRead

#include <libraries/Midi/Midi.h>
#include <sys/ioctl.h>
#include <asm/termbits.h>
#include <fcntl.h>
#include <poll.h>
#include "DataIn.hpp"


#define DEFAULT_SERIAL_PATH "/dev/ttyS4"
#define DEFAULT_BAUD 31250

/*
A class that reads MIDI messages from a serial port and sends them to a MidiParser.
*/
class SerialRead
{
public:

    /*
    sets up the SerialRead instance to read from dev, at baud rate bd, and pipe the results to Midi device m. If no device is specified, the default is /dev/ttyS4 (UART4). If no baud is specified, the default is 31250.
    */
    void setup (InputData * inStruct, const char dev[]=DEFAULT_SERIAL_PATH, int bd=DEFAULT_BAUD);

    /*
    starts the SerialRead read thread
    */
    void startRead();

    /*
    stops the SerialRead read thread
    */
    void stopRead();

    friend void readTask (void *arg);
    
private:

    /*
    sets the serial device dev to bd, which can be a non-standard baud rate (such as 31250 for MIDI)
    */
    int setupSerialDevice (const char dev[], int bd);

    /*
    thread for reading the serial device and sending output to the MidiParser
    */
    void readSerialDevice();

    /*
    the Midi device to send received Midi messages to
    */
    //Midi *_midi;

    /*
    file descriptor for the serial port
    */
    struct pollfd _serial_fd;
    
    InputData * inStruct_;
    
    int buf_[3];
	
};

#endif /* SerialRead_hpp */