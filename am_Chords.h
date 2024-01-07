/*
  ==============================================================================

    am_Chords.h
    Created: 3 Nov 2022 3:53:26pm
    Author:  Andrew McGillivray

  ==============================================================================
*/

/**
*3 classes outputting major chords and minor chords in different wave forms, and cluster chords 
*/

#pragma once

#include "Oscillators.h"
#include <vector>
#include <JuceHeader.h>

/**
*A class that can make major or minor chords out of sawtooth waves.
*/
class Chord
{
public:

    //create a chord of a chosen oscillator and frequency
    /**
    * @param sample rate in Hz
    * @param frequency of base note in Hz
    * @param choice of wave for the chord:
    * @param type of chord: 0 = major, 1 = minor
    * @param number of octaves in chord
    */
    void setUp(float _sampleRate, float baseFrequency, int waveChoice, int chordChoice, int octaves)
    {
        chordCount = octaves * 3;

        if (chordChoice > 1)
        {
            chordChoice = 1;
        }

        if (chordChoice < 0)
        {
            chordChoice = 0;
        }

        //major chord
        if (chordChoice == 0)
        {
            for (int i = 0; i < chordCount; i += 3)                       //will create a major chord for however many octaves are chosen
            {
                //root note
                chord.push_back(Oscillator());
                chord[i].setUp(_sampleRate, baseFrequency * (i + 1), waveChoice);

                //major third
                chord.push_back(Oscillator());
                chord[i + 1].setUp(_sampleRate, baseFrequency * (i + 1) * 1.26, waveChoice);

                //major fifth
                chord.push_back(Oscillator());
                chord[i + 2].setUp(_sampleRate, baseFrequency * (i + 1) * 1.5, waveChoice);
            }
        }

        //minor chord
        if (chordChoice == 1)
        {
            for (int i = 0; i < chordCount; i += 3)                       //will create a major chord for however many octaves are chosen
            {
                //root note
                chord.push_back(Oscillator());
                chord[i].setUp(_sampleRate, baseFrequency * (i + 1), waveChoice);

                //minor third
                chord.push_back(Oscillator());
                chord[i + 1].setUp(_sampleRate, baseFrequency * (i + 1) * 1.189, waveChoice);

                //major fifth
                chord.push_back(Oscillator());
                chord[i + 2].setUp(_sampleRate, baseFrequency * (i + 1) * 1.5, waveChoice);
            }
        }
    }


    //the following is created for phasor waves only:
    //set the number of octaves involved in the chord. Must be set before sample rate. Initial value of 1.
    void setOctaves(int octaveNumber)
    {
        chordCount = octaveNumber * 3;
    }

    /**
    * set the sample rate for all oscillators involved
    * @param sample rate in Hz
    */
    void setSampleRate(float _sampleRate)
    {
        for (int i = 0; i < chordCount; i++)                  //creates a triad of waves
        {
            chord.push_back(Oscillator());
            chord[i].setSampleRate(_sampleRate);            //creates a new oscillator and sets the sample rate
        }
    }

    /**
    *creates a major chord from an input value
    *@param base frequency value in Hz
    */
    void setMajorBaseFrequency(float input)
    {
        for (int i = 0; i < chordCount; i += 3)                       //will create a major chord for however many octaves are chosen
        {
            chord[i].setFrequency(input * (i + 1));                 //root note
            chord[i + 1].setFrequency(input * (i + 1) * 1.26);      //major third
            chord[i + 2].setFrequency(input * (i + 1) * 1.5);       //major fifth
        }
    }

    /**
    *creates a minor chord from an input value
    *@param base frequency value in Hz
    */
    void setMinorBaseFrequency(float input)                         
    {
        for (int i = 0; i < chordCount; i += 3)                     //will create a minor chord for however many octaves are chosen
        {
            chord[i].setFrequency(input * (i + 1));                 //root note
            chord[i + 1].setFrequency(input * (i + 1) * 1.189);     //minor third
            chord[i + 2].setFrequency(input * (i + 1) * 1.5);       //major fifth
        }
    }

    //finds the output of the chord
    float process()
    {
        float mix = 0;
        for (int i = 0; i < chordCount; i++)
        {
            mix += chord[i].process() / (float(chordCount) * (i+1)); //adds the output of each oscillator together, with each additional note decreasing in amplitude 
        }
        return mix;
    }

private:
    std::vector<Oscillator> chord;      //vector to contain the chords
    int chordCount = 3;           //number of chords involved

};


class clusterChord
{
public:

    void setUpCluster(float _sampleRate, int numOfChords, int waveIndex)
    {
        clusterCount = numOfChords;
        
        for (int i = 0; i < clusterCount; i++)
        {
            randomVal = (1000 * random.nextFloat() + 300);          //choose a random base frequency in the range 300 to 1300Hz
            cluster.push_back(Chord());

            if (i % 2 == 0)                                         //chord creation will alternate between major and minor
            {
                cluster[i].setUp(_sampleRate, randomVal, waveIndex, 0, 1);        //uses the random base frequency to create a major chord
            }
            else
            {
                cluster[i].setUp(_sampleRate, randomVal, waveIndex, 1, 1);        //uses the random base frequency to create a minor chord
            }
        }
    }


    void setChordNumber(int input)
    {
        clusterCount = input;
    }

    //creates the randomized cluster chord
    void setSampleRate(float sampleRate)
    {
        for (int i = 0; i < clusterCount; i++)
        {
            randomVal = (1000 * random.nextFloat() + 300);          //choose a random base frequency in the range 300 to 1300Hz
            cluster.push_back(Chord());                          
            cluster[i].setSampleRate(sampleRate);                   //create a new chord

            if (i % 2 == 0)                                         //chord creation will alternate between major and minor
            {
                cluster[i].setMajorBaseFrequency(randomVal);        //uses the random base frequency to create a major chord
            }
            else
            {
                cluster[i].setMinorBaseFrequency(randomVal);        //uses the random base frequency to create a minor chord
            }
        }
    }

    //outputs the cluster chord
    float process()
    {
        float clusterSample = 0.0f;
        for (int j = 0; j < clusterCount; j++)
        {
            clusterSample += cluster[j].process() / clusterCount;
        }

        return clusterSample;
    }

private:

    std::vector<Chord> cluster;
    float randomVal;
    float clusterCount = 1;
    juce::Random random;
};