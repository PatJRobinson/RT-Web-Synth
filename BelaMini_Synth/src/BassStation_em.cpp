/***** BassStation_em.cpp *****/
/***** bassstaion_em.hpp *****/

#include "BassStation_em.hpp"

	void synth::wShape(int wShape)
	{
		
		
		switch (wShape) 
		{
			case 0:
			
			for (unsigned i = 0; i < kNumOscillators_; i++)
			{

				partials[i][0] = 1.0;						//1st partial only
				partials[i][1] = 0.2;						//amplitude of 1
			}

			break;
			
			case 1:

			for (unsigned i = 0; i < kNumOscillators_; i++)
			{
				partials[i][0] = (i + 1);					//all integer partials
				partials[i][1] = 1.0/ (i + 1) * 0.6;				//amplitude inverse of n

			}
			break;
			
			case 2:
			
			for (unsigned i = 0; i < kNumOscillators_; i++)
			{
				partials[i][0] = (2*i) +1;				//odd partials only
				partials[i][1] = 1 / powf((2*i) +1, 2);	//amplitude inverse square of n

			}
			break;
			
			case 3:

			for (unsigned i = 0; i < kNumOscillators_; i++)
			{
				partials[i][0] = (2*i) +1;				//odd partials only
				partials[i][1] = 1.0/ ((2*i) +1);			//amplitude inverse of n

			}
			break;
		}
	}
	void synth::setFreq(float freq, float oct)
	{
		freq_ = freq * oct; 
	}

	float synth::process(void)
	{
		float out = 0.0;
		
			for(unsigned int i = 0; i < kNumOscillators_; i++) {

			// Set the frequency of this oscillator as a multiple of the fundamental frequency
			gOscillators_[i].setFrequency(freq_ * partials[i][0]);
		
		
			//	Check if below niquist rate to avoid alliasing
			if(freq_ * partials[i][0] < audioSampleRate_/2) 
			{
		
				out += gOscillators_[i].process() * partials[i][1] / kNumOscillators_;
				
			}
		
		}
		return out; 
	}
		
	bool synth::setup(int wShape, int oct, int numOscillators, BelaContext * context )
	{
		bool sucess = true;
		
		audioSampleRate_ = context->audioSampleRate;
		kNumOscillators_ = numOscillators;
		
		this->wShape(wShape);
		
		
		// Populate a buffer with a sine wave
		wavetable_.resize(512);
		for(unsigned int n = 0; n < wavetable_.size(); n++) 
		{
			wavetable_[n] = sinf(2.0 * M_PI * (float)n / (float)wavetable_.size());
		}
		
		std::vector<Wavetable> oscillators;
		
		// Create an array of oscillators
		for (unsigned int i = 0; i < kNumOscillators_; i++) {
	
			Wavetable oscillator(audioSampleRate_, wavetable_, false);
			oscillators.push_back(oscillator);
		}
		
		gOscillators_ = oscillators;
		
		return sucess;
	}