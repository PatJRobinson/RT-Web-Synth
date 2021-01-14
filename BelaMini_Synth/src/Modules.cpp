/***** Modules.cpp *****/

//created by : Paddy Robinson
//date revised: 07/10/2020


// Sequencer takes in a 2 byte (hex) pattern (held in an array) and counts number of beats since start() is called. It uses hex addresses to
// compare the pattern to the current beat. If the current beat is in the pattern, the sampleplayer will be triggered on the initial sample of that beat. 

#include "Modules.h"
#include "Bela.h"
#include <math.h>


//	initialise 2 byte hexidecimal pattern, name of sample file, BPM, and %swing
void Sequencer::setup(int beatsPerMinute, const std::string & filename, short int sequence, int swing, BelaContext * context) {
	
	pattern = sequence;
	BPM = beatsPerMinute * 4;
	sample.setup(filename, false, false);
	
	// needs to know how many samples to count in between beat
	samplesPerBeat = 60.0/(float)BPM * context->audioSampleRate;
	// uses setSwing method defined below
	this->setSwing(swing);
	
}

// method to start the Sequencerr
bool Sequencer::start(){
	isPlaying = true;
	return isPlaying;
}
// method to stop the sequencer
bool Sequencer::stop(){
	isPlaying = false;
	readPointer = 0;
	return isPlaying;
}

// getter for samplesPerBeat
int Sequencer::getSamplesPerBeat(){
	return samplesPerBeat;
}

//start of sequence flag
bool Sequencer::startFlag(){
	if (readPointer == 1) {
		return true;
	} else {
		return false;
	}
}

// counts samples, compares the hex bytes from current beat and selected pattern, triggers sample on first frame of 'active' beat
float Sequencer::process(){
	
	if (isPlaying) {
		
		readPointer++;
		
		// apply swing by subtracting swingSamples from readpointer to create a delay
		int adjustedReadPointer;
		if (currentBeatIndex % 2 == 0) {
			adjustedReadPointer = readPointer - swingSamples;
		} else {
			adjustedReadPointer = readPointer;
		}
		
		// make sure readpointer 'wraps aroung' when counted beats reach the end
		if (adjustedReadPointer == (samplesPerBeat * 16)) {
			readPointer -= (samplesPerBeat * 16);
			adjustedReadPointer -= (samplesPerBeat * 16);
		}
		
		
		// calculate 16bit hex representing current beat
		currentBeatIndex = (float)adjustedReadPointer/samplesPerBeat; //if readPointer == 0 current beat will be 0x8000
		currentBeat = beatGrid[currentBeatIndex];
		
		// need to set state to false before first frame of 'active' beat otherwise this flag- used to 
		//ignore repeated triggers - would cause consecuative 'active' beats not to trigger
		if (currentBeatIndex != lastBeatIndex) {
			state = false;	
			}
		lastBeatIndex = currentBeatIndex;
		
	} else { 
		//	sequencer is off
		currentBeat = 0x0000;
		//float out = 0;
		//return out;
	}
		
		// compare 2 byte pattern with 2 byte 'currentBeat', and only trigger if first frame of 'active' beat
	if (((currentBeat & pattern) !=0)&&(state == false)) {
		
		
		state = true;
		sample.trigger();	// Sets MonoFilePlayer: isPlaying to true only on first sample of beat
	}
	// beat inactive
	else if ((currentBeat && pattern) == 0) {
		
		state = false;
	}
	
	float out = sample.process();	// Will return 0 if MonoFilePlayer not playing
	return out;
	
}

// converts % of beat period to number of samples (to be subtracted from readPointer, effectively causing a delay)
void Sequencer::setSwing(int swing) {
	
	swingSamples = (int)((float)swing * (float)samplesPerBeat / 100);			//ERROR: irregularity occurs at end of sequence 
}


float Looper::process(float in) {
	if (is_recording) {
	 recordingBuffers[currentBuffer][readPointer] = in;
	 if ((switchState == LOW)&&(lastState != LOW)) {
		for (unsigned int i = 0; i < 4; i++) {
			recordingBuffers[i].resize(readPointer);
		}
	 }
		currentBuffer ++;
		if (currentBuffer > 4) {
			 currentBuffer = 4;
			 is_recording = false;
		}
		
		readPointer = 0;
	
	}
	float out = 0;
	
	for (unsigned int i = 0; i < 4; i++) {
		out += recordingBuffers[i][readPointer];
	}
	
	return out;
	
}

