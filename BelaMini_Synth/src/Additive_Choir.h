/***** additive_choir *****/
#include "BassStation_em.hpp"
#include <cmath>
#include <vector>

/*
Creates an array of synth obects which can be controlled globally, and incorperates a detune function
*/
class synth_multi 
{
	public:
	
	bool setup(int numVCOsc, float detune, int wShape, int oct, int numOscillators, BelaContext * context);
	void wShape(int wShape);
	void setFreq(float freq, float oct, float detune);
	float process(void);
	
	private:
	
	std::vector<synth> gVoices_;
	int kNumVoices_;
	
	
	
};