/***** Additive_Choir.cpp *****/
#include "Additive_Choir.h"

bool synth_multi::setup(int numVCOsc, float detune, 
				int wShape, int oct, int numOscillators, BelaContext * context)

{
	bool sucess = true;
		
		kNumVoices_ = numVCOsc;
		std::vector<synth> voices;
	
		// Create an array of oscillators
		for (unsigned int i = 0; i < kNumVoices_; i++) 
		{
			synth voice;
			voice.setup(wShape, oct, numOscillators, context);
			voices.push_back(voice);
		}
		
		gVoices_ = voices;
		
	return sucess;
}

void synth_multi::wShape(int wShape)
{
	for (unsigned int i = 0; i < kNumVoices_; i++)
	{
		gVoices_[i].wShape(wShape);
	}
}

void synth_multi::setFreq(float freq, float oct, float detune)
{
	for (unsigned int i = 0; i < kNumVoices_; i++)
	{
		// detune
		if (i % 2 != 0) {
			detune *= -1;
		}
		float frequency = freq * (1 + (detune * i + (i%2)));
		gVoices_[i].setFreq(frequency, oct);
	}
}

float synth_multi::process(void)
{
	float out = 0;
	
	for (unsigned int i = 0; i < kNumVoices_; i++)
	{
		// detune
		out += gVoices_[i].process() / kNumVoices_;
	}
	
	return out;
	
}
