/***** bassstaion_em.hpp *****/
#include <cmath>
#include <vector>
#include "wavetable.h"
#include <Bela.h>


/*
Additive synth: creates an array of wavetable Oscillators, constructs a waveshape from sine waves
The higher the number of Oscillators, the more realistic the wave created, the more CPU power is used 
*/
class synth

{
	public:
	
	void wShape(int wShape);
	void setFreq(float freq, float oct);
	float process(void);
	
	bool setup(int wShape, int oct, int numOscillators, BelaContext * context);
	
	private:
	
	enum waveshapes {
	
	SINE,
	SAWTOOTH,
	TRIANGLE,
	SQUARE
		
	};
	
	int audioSampleRate_;
	int kNumOscillators_;
	float freq_;
	float partials[200][2];
	
	std::vector<float> wavetable_;
	// Oscillators for the additive synths
	std::vector<Wavetable> gOscillators_;
	

};