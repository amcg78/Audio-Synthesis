/*
  ==============================================================================

    am_Duration.h
    Created: 28 Oct 2022 11:12:52am
    Author:  Andrew McGillivray

  ==============================================================================
*/

#pragma once
#include "Oscillators.h"

/**
Duration Class
*
*Allows an input sound to be 'turned on' between chosen seconds in a decided window
*Note: this allows gain to be 1.0dB when 'turned on' and 0.0dB when 'turned off'
*/
class DurationWave
{
public:

	//Sets up a complete duration wave
	void setUpDuration(float _sampleRate, float totalDurationInSeconds, float startValueInSeconds, float endValueInSeconds)
	{
		osc.setUp(_sampleRate, 1/totalDurationInSeconds, 0);
		duration = totalDurationInSeconds;
		start = startValueInSeconds / duration;
		end = endValueInSeconds / duration;
	}

	/**
	*set the sample rate for the duration wave
	*@param sample rate of phasor in Hz. Must be matched with the class affected by the wave
	*/
	void setSampleRate(float _sampleRate)
	{
		osc.setSampleRate(_sampleRate);
	}

	/**
	*Choose the length of piece (in seconds) the duration will be set in
	*Note: a piece length shorter than what will be heard will result in repetition of the sound
	*@param piece length in seconds
	*/
	void setPieceLength(float durationInSeconds)
	{
		duration = durationInSeconds;
		osc.setFrequency(1 / durationInSeconds); //sets a phasor which will increase from 0 to 1 over the duration set
	}

	/**
	*Choose when (in seconds) the start and end values within the piece length for which the sound will be heard
	*@param start value in seconds
	*@param end value in seconds
	*/
	void setWindow(float startValueInSeconds, float endValueInSeconds)
	{
		start = startValueInSeconds / duration;			 //a fraction between 0 and 1 which will be used to compare vs the phasor to start the sound
		end = endValueInSeconds / duration;				 //a fraction between 0 and 1 which will be used to compare vs the phasor to end the sound
	
		
	}

	/**
	*will give 1.0dB of gain when within the set window and 0.0dB elsewhere.
	*/
	float process()
	{
		sample = osc.process();									//starts the phasor on a ramp from 0 to 1.
		if (sample >= start)									// if the phasor decimal reaches the start decimal
		{	
			fadeIn = fadeIn + 0.00005;							//a 0.5s short fadein to avoid clicks
			outVal = fadeIn;									//outputs the fadein

			if (fadeIn >= 1.0f)									//overwrites the fadein to stay at 1.0dB and not clip
			{
			    outVal = 1.0f;									//outputs a constant gain
			}

			if (sample >= end - 0.001)									//if the phasor reaches the end decimal
			{
				fadeOut = fadeOut - 0.00005;				//a 0.5s short fadeout to avoid clicks
				outVal = fadeOut;								//outputs the fadeout
				fadeIn = 0.0f;									//resets the fadein

				if (fadeOut <= 0.0f)
				{
					outVal = 0.0f;								//keeps the gain at 0.0dB
					
				}
			}
		}
		else
		{
			outVal = 0.0f;										//zero gain before start time
		}

		return outVal;											//return the gain
	}

private:
	Oscillator osc;												//phasor to compare vs start/end times
	float sample;												//value to compare against start/end times

	//without initialising start and end times, the gain will always be 1.0dB
	float start;										//failsafe on start time
	float end;											//failsafe end time

	float outVal;
	float duration;												//length of piece
	
	float fadeIn = 0.0f;										//initalise fadein
	float fadeOut = 1.0f;										//initialise fadeout
	

};

