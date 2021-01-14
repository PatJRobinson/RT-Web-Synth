/***** modules.h *****/

#include <string>
#include "MonoFilePlayer.h"
#include "Bela.h"
#include <cmath>
#include <vector>


class Sequencer {
	
	public:
	
		Sequencer(){};
		void setup(int beatsPerMinute, const std::string & filename, short int sequence, int swing, BelaContext * context);
		bool start();
		bool stop();
		float process();
		void setSwing(int swing);
		int getSamplesPerBeat();
		bool startFlag();
		
	private:
		
		MonoFilePlayer sample;
		int BPM = 80;
		int samplesPerBeat;
		short int pattern = 0xFFFF;
		short int currentBeat = 0x0000;
	    int beatGrid [16] = {0x8000, 0x4000, 0x2000, 0x1000, 
								   0x0800, 0x0400, 0x0200, 0x0100, 
								   0x0080, 0x0040, 0x0020, 0x0010, 
								   0x0008, 0x0004, 0x0002, 0x0001};
		int readPointer = 0;
		bool isPlaying = false;
		bool state = false;
		int currentBeatIndex = 0;
		int lastBeatIndex = 0;
		int swingSamples = 0;
		
		
	
};

class Looper {
	
	public:
	
		Looper(){};
		void setup();
		void resize(int size);
		float process(float in);
		
	private:
	
		std::vector <float> recordingBuffers[4];
		int readPointer;
		int currentBuffer = 0;
		int switchState = 0;
		int lastState = 0;
		bool is_recording = false;

	
};