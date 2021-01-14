/***** PedalBoard.h *****/
#include <vector>
#include "Bela.h"

class AudioDelay {
	
	public: 
	AudioDelay() {};
	AudioDelay(BelaContext * context);
	bool setup(BelaContext * context); // Returns true on success
	float process(float audioIn, float delayTime, float feedback, BelaContext * context);
	void changeVolume(float vol);
	
	private:
	std::vector<float> delayBuffer_;			// Buffer that holds the record of the input n samples long 
	int readPointer_ = 0;						// Position of the last frame read from the bufffer and outputted
	int writePointer_ = 0;						// Position of the last frame written to the buffer
	int delaySamples_ = 0;						// Delay time in samples
	float feedback_ = 0.0;						// Feedback (0 - 0.95)
	float volume_ = 0.0;
	
};

class DistortionEffect {
	
	public:
	
	void setup(float gainIn, float gainOut, int distType);
	float process(float inputSample);
	
	private: 
	
	float inputGain;
	float outputGain;
	
	enum distortionType {kPassThrough, kTypeHardClipping, kTypeSoftClipping, kTypeSoftClippingExponential, kTypeFullWaveRectifier, kTypeHalfWaveRectifier} ;
	
	float inputGainDecibels_;
	int distortionType_;
	
	
};