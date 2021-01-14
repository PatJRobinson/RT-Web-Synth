/***** PedalBoard.cpp *****/

#include "PedalBoard.h"
#include <vector>
#include <cmath>


	//	Constructer taking two perameters: delay time (s) and feedback (0 - 0.95)
	AudioDelay::AudioDelay(BelaContext * context) {
		
		setup(context);
	}		
	
	bool AudioDelay:: setup(BelaContext *context) {
		
		
		delayBuffer_.resize(0.5 * context->audioSampleRate);
		volume_ = 1;
		return true;
	}
	
	float AudioDelay:: process(float audioIn, float delayTime, float feedback, BelaContext * context) {
		
		delaySamples_ = delayTime * context->audioSampleRate;
		feedback_ = feedback;
		
		//	initialize readPointer 
		readPointer_ = (writePointer_ - delaySamples_ + delayBuffer_.size()) % delayBuffer_.size();
		
		//	read from delayBuffer n samples away from write location
		float out = delayBuffer_[readPointer_];

		//  write to delayBuffer current audio frame + delay feedback
		delayBuffer_[writePointer_] = audioIn + out * feedback_;
		

		//	update read and write pointers
		if (writePointer_ < delayBuffer_.size()) {
			writePointer_ ++;
		}	else {
			writePointer_ = 0;
		}
		
		if (readPointer_ < delayBuffer_.size()) {
			readPointer_ ++;
		}	else {
			readPointer_ = 0;
		}
		
		return out * volume_;
	}
	
	void AudioDelay::changeVolume(float vol) {
		volume_ = vol;
	}
	
	
	
void DistortionEffect:: setup(float gainIn, float gainOut, int distType) {
	
	distortionType_ = distType;
	inputGainDecibels_ = gainIn;
	outputGain = gainOut;
}

float DistortionEffect:: process(float inputSample) {
	
	// Calculate input gain once to save calculations
	inputGain = powf(10.0f, inputGainDecibels_ / 20.0f);
	


		float in = inputSample * inputGain;    
		float out = 0.0;
		
		// Apply distortion based on type   
		
		if (distortionType_ == kPassThrough) {
			out = in/inputGain;
		}
		else if(distortionType_ == kTypeHardClipping) {        // Simple hard clipping      
		
			float threshold = 1.0f;
			
			if(in > threshold) {           
			out = threshold;    
			}	else if (in < -threshold)  {          
			out = -threshold;        
			}
			else {    
			out = in; }
			} 
		
		else if(distortionType_ == kTypeSoftClipping) {       // Soft clipping based on quadratic function
		
			float threshold1 = 1.0f/3.0f;        
			float threshold2 = 2.0f/3.0f;        
		
			if(in > threshold2)  {
				out = 1.0f; }       
		
			else if(in > threshold1)  {
				out = (3.0f - (2.0f - 3.0f*in) * 
					  (2.0f - 3.0f*in))/3.0f; }        
			
			else if(in < -threshold2)  {
				out = -1.0f; }       
			
			else if(in < -threshold1)  {
				out = -(3.0f - (2.0f + 3.0f*in) *
					   (2.0f + 3.0f*in))/3.0f;   } 
					   
			else {          
			out = 2.0f* in;    }    
		}
			
		else if (distortionType_ == kTypeSoftClippingExponential)     {        // Soft clipping based on exponential function     
		
			if(in > 0)  {
				out = 1.0f - expf(-in);  }
			
			else  {
				out = -1.0f + expf(in);    }    
		}
		
		else if(distortionType_ == kTypeFullWaveRectifier) {        // Full-wave rectifier (absolute value)       
		
			out = fabsf(in);    }    
			
		else if(distortionType_ == kTypeHalfWaveRectifier) {        // Half-wave rectifier      
			if(in > 0)  {
				out = in;   }
			
			else   {         out = 0;}
		} else { out = in; }
			
		out *= outputGain;
		return out;
}