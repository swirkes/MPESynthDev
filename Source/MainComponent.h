/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent()
    {
        // Make sure you set the size of the component after
        // you add any child components.
        setSize (800, 600);

        // Some platforms require permissions to open input channels so request that here
        if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
            && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
        {
            RuntimePermissions::request (RuntimePermissions::recordAudio,
                                         [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
        }
        else
        {
            // Specify the number of input and output channels that we want to open
            setAudioChannels (2, 2);
        }
    }

    ~MainComponent()
    {
        // This shuts down the audio device and clears the audio source.
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        frequency = 440;
            phase = 0;
            wtSize = 1024;
            increment = frequency * wtSize / sampleRate;
            amplitude = 0.25;
            
            //one cycle of a sine wave
        //    for (auto i = 0; i < wtSize; i++)
        //    {
        //        waveTable.insert(i, sin(2.0 * double_Pi * i / wtSize));
        //    }
            //one cycle square wave
        //    for (auto i = 0; i < wtSize; i++)
        //    {
        //        if (i < wtSize / 2)
        //            waveTable.insert(i, -1);
        //        else
        //            waveTable.insert(i, 1);
        //    }
            
            
            //one cycle triangle wave
            for (auto i = 0; i < wtSize; i++)
            {
                waveTable.insert(i, phase);
                if (i < wtSize * 0.25)
                    waveTable.insert(i, (i / wtSize) * 4);
                else if (i < wtSize * 0.75)
                    waveTable.insert(i, 2 - ((i / wtSize) * 4));
                else
                    waveTable.insert(i, (i / wtSize) * 4 - 4.0);
            }
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!
        float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
        
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            leftSpeaker[sample] = waveTable[(int)phase] * amplitude;
            rightSpeaker[sample] = waveTable[(int)phase] * amplitude;
            phase = fmod ((phase + increment), wtSize);
            
        }

    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================
    // Your private member variables go here...
    Array<float> waveTable;
    
    double wtSize;
    double frequency;
    double phase;
    double increment;
    double amplitude;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
