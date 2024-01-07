/*
  ==============================================================================

	Oscillators.h
	Created: 21 Nov 2022 10:01:20am
	Author:  B149642

  ==============================================================================
*/
#pragma once
#ifndef Oscillators_h
#define Oscillators_h
#include <cmath>

/**
* NOTE: When using this class and its inheritance classes, all gains are set to oscillate between 1.0dB and -1.0dB.
*/
class Oscillator
{
public:

	/**Sets up a basic oscillator completely
	* @param sample rate in Hz
	* @param frequency in Hz
	* @param wave index: 0 = Phasor, 1 = Sin, 2 = Square, 3 = Triangle, 4 = Sawtooth
	*/
	void setUp(float sampleRate, float _freq, int waveIndex)
	{
		setSampleRate(sampleRate);
		setFrequency(_freq);
		setWaveIndex(waveIndex);
	}


	//Individual Functions
	
	//set the sample rate in Hz
	void setSampleRate(float _sampleRate)
	{
		sampleRate = _sampleRate;
	
	}

	//set the frequency of the wave in Hz
	void setFrequency(float _freq)
	{
		freq = _freq;
		phaseDelta = freq / sampleRate;
	}

	//sets where the phase will start
	void setOffset(float offset)
	{
		phaseOffset = offset;
	}



	//Specific Functions

	//set the wave index: 0 = Phasor, 1 = Sin, 2 = Square, 3 = Triangle, 4 = Sawtooth
	void setWaveIndex(int waveIndex)
	{
		waveIndexVal = waveIndex;
	}

	//set phi value for a sine wave
	void setPhi(float phiInput)
	{
		phi = phiInput;
	}

	//set the phi modulation value
	void setPhiMod(float modInput)
	{
		phiMod = modInput;
	}

	//set phase width for a square wave
	void setPhaseWidth(float _pw)
	{
		pw = _pw;
	}


	//Output Functions
	
	//creates a wave
	//for Wave Index: 0 = Phasor, 1 = Sin, 2 = Square, 3 = Triangle, 4 = Sawtooth
	float process()
	{
		phase += (phaseDelta + phaseOffset);
		//wrap the phase

		if (phase > 1.0)
		{
			phase -= 1.0;
		}

		//phasor
		if (waveIndexVal == 0)
		{
			return phase;
		}

		//sine wave
		if (waveIndexVal == 1)
		{
			return std::sin(phase * 2.0 * 3.14159 + (phi * phiMod));
		}

		//square wave
		if (waveIndexVal == 2)
		{
			if (phase > pw)
			{
				outVal = -1.0f;
			}
			else
			{
				outVal = 1.0f;
			}
			return outVal;
		}

		//triangle wave
		if (waveIndexVal == 3)
		{
			return 4 * (fabs(phase - 0.5f) - 0.25);
		}

		//sawtooth wave
		if (waveIndexVal == 4)
		{
			return 2 * (phase - 0.5f);
		}
	}
	
private:
	float freq;
	float phase = 0.0f;
	float phaseDelta;
	float phaseOffset = 0.0f;
	float sampleRate;
	int waveIndexVal = 0;
	float phi = 0;
	float phiMod = 1;
	float outVal;
	float pw = 0.5;
};

#endif /* Oscillators_h */