//Can be altered to create a chorus class

/*
  ==============================================================================

    Chorus.h
    Created: 10 Dec 2022 12:01:20am
    Author:  Andrew McGillivray

  ==============================================================================
*/
#pragma once


class DoubleCombFilter
{
    
public:

    //destructor
    ~DoubleCombFilter()
    {
        if (delayLine != nullptr)
        {
            delete[] delayLine;
        }
    }
    
    /// set the sample rate (Hz)
    void setSampleRate(float sampleRateIn)
    {
        sampleRate = sampleRateIn;
    }

    ///set the maximum size of the delay line
    void setMaxDelay(int maxDelayIn)
    {
        maxLength = maxDelayIn * sampleRate;             
        
        //free up data
        if (delayLine != nullptr)
            delete[] delayLine;
        
        delayLine = new float[maxLength]; //create array
        
        //initialise array
        for (int i=0; i<maxLength; i++)
        {
            delayLine[i] = 0.0;
        }
    }
    
    
    ///set the delay time in seconds
    void setDelayTimes(float delayTimeOneIn, float delayTimeTwoIn)
    {
        delayTimeOne = delayTimeOneIn * sampleRate;
        delayTimeTwo = delayTimeTwoIn * sampleRate;
        
        // set read indexes
        readIndexOne = writeIndex - delayTimeOne;
        readIndexTwo = writeIndex - delayTimeTwo;
        
        if (readIndexOne < 0)
        {
            readIndexOne = readIndexOne + maxLength;
        }

        if (readIndexTwo < 0)
        {
            readIndexTwo = readIndexTwo + maxLength;
        }
        
    }
    
    ///use delay line
    float process(float input)
    {

        // read in current value
        float outputSampleOne = linearInterpolationOne();
        float outputSampleTwo = linearInterpolationTwo();

        float outputSampleTotal = input + (outputSampleOne * feedbackOne) + (outputSampleTwo * feedbackTwo);

        // store value in delay line
        delayLine[writeIndex] = input;


        // find next read and write values;
        readIndexOne += 1;
        if (readIndexOne >= maxLength)
        {
            readIndexOne -= maxLength;
        }

        readIndexTwo += 1;
        if (readIndexTwo >= maxLength)
        {
            readIndexTwo -= maxLength;
        }

        writeIndex += 1;
        if (writeIndex >= maxLength)
        {
            writeIndex -= maxLength;
        }

        return outputSampleTotal;

    }
    
    ///uses linear interpolation to find the correct sample
    float linearInterpolationOne()
    {  
        //find relevant indexes
        int indexOne = int(readIndexOne);        // e.g. 2
        int indexTwo = indexOne + 1;            // e.g. 3
        
        //failsafe on index two
        if (indexTwo > maxLength)
        {
            indexTwo -= maxLength;
        }
        
        //read values
        float valOne = delayLine[indexOne];          // e.g. data[2]
        float valTwo = delayLine[indexTwo];          // e.g. data[3]
        
        // calculate difference
        float difference = readIndexOne - indexOne;   // e.g. 2.3 - 2 = 0.3
        
        // work out interpolated sample between two indexes
        float interpolatedSample = (1 - difference) * valOne + difference * valTwo;
        
        
        return interpolatedSample;
    }

    ///uses linear interpolation to find the correct sample
    float linearInterpolationTwo()
    {
        //find relevant indexes
        int indexOne = int(readIndexTwo);        // e.g. 2
        int indexTwo = indexOne + 1;            // e.g. 3

        //failsafe on index two
        if (indexTwo > maxLength)
        {
            indexTwo -= maxLength;
        }

        //read values
        float valOne = delayLine[indexOne];          // e.g. data[2]
        float valTwo = delayLine[indexTwo];          // e.g. data[3]

        // calculate difference
        float difference = readIndexTwo - indexOne;   // e.g. 2.3 - 2 = 0.3

        // work out interpolated sample between two indexes
        float interpolatedSample = (1 - difference) * valOne + difference * valTwo;


        return interpolatedSample;
    }
    
    ///sets the feedback in the interval [0, 1]
    void setFeedback(float feedbackInOne, float feedbackInTwo)
    {
        feedbackOne = feedbackInOne;
        feedbackTwo = feedbackInTwo;

        //failsafes
        if (feedbackOne < 0)
        {
            feedbackOne = 0;
        }
        if (feedbackOne > 1.0)
        {
            feedbackOne = 1.0;
        }

        if (feedbackTwo < 0)
        {
            feedbackTwo = 0;
        }
        if (feedbackTwo > 1.0)
        {
            feedbackTwo = 1.0;
        }
    }
    
    
    
    
private:
    
    float readIndexOne = 0;  //read position 1
    float readIndexTwo = 0;  //read position 2
    int writeIndex = 0; //write position
    
    int maxLength;           //maximum possible delay time
    
    float* delayLine = nullptr;  //stores delay data
    
    //delay times
    int delayTimeOne;  
    int delayTimeTwo;
    
    
    float feedbackOne = 0.5;     //must be in [0, 1]
    float feedbackTwo = 0.5;     //must be in [0, 1]
    float sampleRate;
};
