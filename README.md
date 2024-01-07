The headers contained in this depository are examples of audio synthesis using C++, particularly for the JUCE engine.
The following files are contained:
1) am_Oscillators:
   - This file contains code to put out phasor, sine, square, triangle and sawtooth waves which can be changed through wave index.
   - Frequency is of course a variable here.
   - There are multiple variables which can be changed from a simple wave such as phase change, phase modultion, phase width (for square waves) and phase offset.
2) am_Chords
   - This code uses the am_Oscillators file and vectors to create chords of a certain wave type.
   - Major, minor and cluster chords can be created here, chosen from base frequencies.
   - The number of octaves included in the chord can be chosen as well.
3) am_DoubleCombFilter
   - This code uses 2 buffer delay lines that can be created at will to create 2 separate comb filters for incoming audio.
   - The code uses simple linear interpolation for the delays.
   - Variables here include: delay length and strength (feedback) [0,1].
4) am_Duration
   - The code in this header file allows for choice of when audio plays during a certain duration
   - The user must set duration length (for which after that the clock will reset so choosing the length of the piece will mean no repetition) in **seconds**.
   - The user must also set start and end times in seconds.
   - There is a 0.5 fade in and fade out to avoid clicks.
5) am_Phase_Modulator
   - This code allows for continuous phase modulation of the sine waves in am_Oscillators by using a secondary wave of choice to affect the phase of the primary sine wave.
   - The code will insert a changing phase in the 'Phi' variable of oscillators.
  
All of these files are header files containing classes to be used in other processors which must include the <vector> and <cmath> packages (or others if changing the code).
