#include <Bela.h>
#include "Modules.h"
#include <cmath>
#include <cstring>
#include <libraries/Midi/Midi.h>
#include <libraries/Gui/Gui.h>
#include <libraries/GuiController/GuiController.h>
#include "Additive_Choir.h"
#include <libraries/ADSR/ADSR.h>
#include "SerialRead.hpp"
#include "PedalBoard.h"


ADSR envelope; // ADSR envelope
InputData ctrIn; // data structure to store incoming UART messages
SerialRead sm; // parses incoming UART messages

Midi midi; // parses incoming MIDI messages

/* additive Synth which can have multiple voices and detune -- CPU tradeoff  
between number of voices (kNumVoices) and quality of each voice (NumOscillators) */
synth_multi poly; 

const int kNumVoices = 1;
const int kNumOscillators = 80;

AudioDelay delay; // 1 of many potential effects!

// global variables
float gFrequency;
int gVelocity;
int gLastVelocity;
bool gIsNoteOn;
int gOctave = 1;
float gAmplitude;
float gPhaseIncrement;
int gWshape;
float gDetune;
float gAttack = 0.1; // Envelope attack (seconds)
float gDecay = 0.25; // Envelope decay (seconds)
float gRelease = 0.5; // Envelope release (seconds)
float gSustain = 1.0; // Envelope sustain level
int attSamples;
float velocity2;
const char* gMidiPort0 = "hw:1,0,0"; // our MIDI port

// GUI for alternative control via USB
Gui gGui;
GuiController gGuiController;


// set up as an auxilliary task to deal with incoming MIDI messages
void midiMessageCallback(MidiChannelMessage message, void* arg){
	if(arg != NULL){
		rt_printf("Message from midi port %s ", (const char*) arg);
	}
	message.prettyPrint();
	if(message.getType() == kmmNoteOn){
		gFrequency = powf(2, (message.getDataByte(0) - 69) / 12.f) * 440.f;
		gVelocity = message.getDataByte(1);
		gIsNoteOn = gVelocity > 0;
		rt_printf("v0:%f, ph: %6.5f, gVelocity: %d\n", gFrequency, gVelocity);
	}
}


bool setup(BelaContext *context, void *userData)
{
	//MIDI
	midi.readFrom(gMidiPort0);
	midi.writeTo(gMidiPort0);
	midi.enableParser(true);
	midi.setParserCallback(midiMessageCallback, (void*) gMidiPort0);
	
	sm.setup (&ctrIn);
	
	poly.setup(kNumVoices, 0.005, 1, 1, kNumOscillators, context);
	delay.setup(context);
	
	// Set up the GUI
	gGui.setup(context->projectName);
	gGuiController.setup(&gGui, "Synth Controls");
	//gPartialSelector.setup(&gGui, "Partial Selector");
	
	// Arguments: name, default value, minimum, maximum, increment
	gGuiController.addSlider("Amplitude (dB)", 0, -41, 0, 0);
	gGuiController.addSlider("Oct", -2, -2, 4, 1);
	gGuiController.addSlider("wShape", 0, 0, 3, 1);
	gGuiController.addSlider("detune", 0, 0, 0.01, 0);
	gGuiController.addSlider("Attack", 0.1, 0.01, 1, 0);
	gGuiController.addSlider("Decay", 0.1, 0.01, 1, 0);
	gGuiController.addSlider("Sustain", 1, 0.01, 1, 0);
	gGuiController.addSlider("Release", 0.1, 0.01, 10, 0);
	gGuiController.addSlider("fb", 0, 0.0, 1, 0);
	gGuiController.addSlider("t", 0, 0.0, 5, 0);
		

	return true;
}

void render(BelaContext *context, void *userData)
{
	gAmplitude = gGuiController.getSliderValue(0);
	gOctave = gGuiController.getSliderValue(1);
	gWshape = gGuiController.getSliderValue(2);
	gDetune = gGuiController.getSliderValue(3);
	
	gAttack = gGuiController.getSliderValue(4);
	gDecay = gGuiController.getSliderValue(5);
	gSustain = gGuiController.getSliderValue(6);
	gRelease = gGuiController.getSliderValue(7);
	float feedback = gGuiController.getSliderValue(8);
	float dTime = gGuiController.getSliderValue(9);
	
		// Set ADSR parameters
	envelope.setAttackRate(gAttack * context->audioSampleRate);
	envelope.setDecayRate(gDecay * context->audioSampleRate);
	envelope.setReleaseRate(gRelease * context->audioSampleRate);
	envelope.setSustainLevel(gSustain);
	
	// UART IN -- get parameter values
		gWshape = ctrIn.getVal(0x90,0);
		gOctave = ctrIn.getVal(0x90,1) - 2;
		int gFXtog = ctrIn.getVal(0x90, 3);
		int gInTog = 0; 

	// Set synth perameters
	poly.wShape(gWshape);
	
	float amplitude = powf(10.0, gAmplitude / kNumOscillators);		// Convert dB to linear amplitude
	poly.setFreq(gFrequency, powf(2, gOctave), gDetune); 
		float velocity = map(gVelocity, 0, 127, 0, 1);
	    
	    
	for (unsigned int n = 0; n < context->audioFrames; n++) {
	
	    	// If gate button has been pressed...
		if((gVelocity > 0)&& (gLastVelocity == 0)) {
			// ...turn ON the envelope's gate
			envelope.gate(true);
			attSamples = 0;
			velocity2 = velocity;
			rt_printf("Gate on\n");
		// If button has been depressed...
		} else if ((gVelocity == 0) && (gLastVelocity > 0)) {
			// ...turn OFF the envelope's gate
			envelope.gate(false);
			attSamples = 0;
			rt_printf("Gate off\n");
		}
		// Store last status
		gLastVelocity = gVelocity;
		attSamples++;
	
		float out = 0;
	    float envValue = envelope.process();
	    
		//synth process
		if(envValue != 0){
		out = poly.process() * amplitude * velocity2 * envValue;
		}
		
		float in = audioRead(context, n, 0);
		
		
		if (gInTog == 1)
		{
			out += in;
		}
		if(gFXtog == 1)
		{
			out += delay.process(out, dTime, feedback, context);
		}
		

		for (unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			audioWrite(context, n, channel, out);
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{

}