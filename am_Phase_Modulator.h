/*
  ==============================================================================

	am_Phase_Modulator.h
	Created: 27 Oct 2022 10:17:48am
	Author:  Andrew McGillivray

  ==============================================================================
*/#pragma once

#include "Oscillators.h"

/**
*A class containing a phi modulator for sinusoids. The parameters to be set in this class are:
*1) Carrier Frequency
*2) Modulating Frequency
*3) Modulating Index (to be multiplied by the modulating frequency output value))
*and as always, Sample Rate.
*
*The variable will be applied as follows:
*output = sin((2 * pi * phase) + (modulatingOscillatorOutput * modultingIndex))
*/
class PhiModulator 
{
public:

	/**
	*set up an entire phi modulator
	*/
	void setUpPhiModulator(float _sampleRate, float carrierFrequency, int carrierWaveIndex, float modulatorFrequency, int modulatorWaveIndex, float carrierIndexValue)
	{
		sampleRate = _sampleRate;
		carrier.setUp(_sampleRate, carrierFrequency, carrierWaveIndex);
		modulator.setUp(_sampleRate, modulatorFrequency, 1);
		carrier.setPhiMod(carrierIndexValue);
	}

	/**
	*set the sample rate for all oscillators involved
	*@param sample rate in Hz
	*/

	void setSampleRate(float _sampleRate)
	{
		sampleRate = _sampleRate;
		carrier.setSampleRate(_sampleRate);
		modulator.setSampleRate(_sampleRate);
	}

	/**
	*set the frequencies for both the carrier oscillator and the modulator oscillator
	*@param frequency of carrier oscillator in Hz
	*@param frequency of modulator oscillator in Hz
	*@param index value in the oscillator
	*/
	void setUp(float carrierfrequency, float modulatorfrequency, float indexValue)
	{
		carrier.setFrequency(carrierfrequency);
		modulator.setFrequency(modulatorfrequency);
		carrier.setPhiMod(indexValue);
	}

	//modulates the phase of the carrier oscillator with the modulator oscillator and produces and output
	float process()
	{
		float modVal = modulator.process();					//find the modulator oscillator output
		carrier.setPhi(modVal);								//updates the carrier oscillator phi value.
		float mainVal = carrier.process();					//finds the new carrier oscillator output

		return mainVal;
	}
private:
	float sampleRate;
	Oscillator carrier;
	Oscillator modulator;
};
