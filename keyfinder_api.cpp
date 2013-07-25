#include "keyfinder_api.h"
#include "keyfinder.h"
#include <limits.h>
#include <iostream>

using namespace std;

#define XSTR(x) #x
#define STR(x) XSTR(x)

const char* kfinder_get_version()
{
    return STR(VERSION);
}

const char* kfinder_get_key(short signed int   *samples,
                            unsigned int        nb_samples,
                            short unsigned int  frame_rate,
                            short unsigned int  nb_channels)
{
    // Check input parameter.
    if ((samples == NULL) || (nb_samples == 0) || (frame_rate == 0) || (nb_channels == 0))
    {
        return "";
    }

    // Build the main computing object.
    KeyFinder::KeyFinder k;

    // Build an empty audio object
    KeyFinder::AudioData a;

    // Prepare the object for your audio stream
    a.setFrameRate(frame_rate);
    a.setChannels(nb_channels);
    a.addToSampleCount(nb_samples);

    // Copy your audio into the object (as float).
    for (unsigned int i = 0; i < nb_samples; i++)
    {
        a.setSample(i, (float)samples[i]);
    }

    // Run the analysis
    KeyFinder::KeyDetectionResult r;
    try
    {
        r =  k.keyOfAudio(a);
    }
    catch(const std::exception& e)
    {
        cerr << "libKeyFinder: exception: " << e.what() << endl;
        return "";
    }
    catch(...)
    {
        cerr << "libKeyFinder: unknown exception" << endl;
        return "";
    }


    // And do something with the result!
    KeyFinder::key_t key = r.globalKeyEstimate;
    switch(key)
    {
        case KeyFinder::A_MAJOR:      return "AM";
        case KeyFinder::A_MINOR:      return "Am";
        case KeyFinder::B_FLAT_MAJOR: return "BbM";
        case KeyFinder::B_FLAT_MINOR: return "Bbm";
        case KeyFinder::B_MAJOR:      return "BM";
        case KeyFinder::B_MINOR:      return "Bm";
        case KeyFinder::C_MAJOR:      return "CM";
        case KeyFinder::C_MINOR:      return "Cm";
        case KeyFinder::D_FLAT_MAJOR: return "DbM";
        case KeyFinder::D_FLAT_MINOR: return "Dbm";
        case KeyFinder::D_MAJOR:      return "DM";
        case KeyFinder::D_MINOR:      return "Dm";
        case KeyFinder::E_FLAT_MAJOR: return "EbM";
        case KeyFinder::E_FLAT_MINOR: return "Ebm";
        case KeyFinder::E_MAJOR:      return "EM";
        case KeyFinder::E_MINOR:      return "Em";
        case KeyFinder::F_MAJOR:      return "FM";
        case KeyFinder::F_MINOR:      return "Fm";
        case KeyFinder::G_FLAT_MAJOR: return "GbM";
        case KeyFinder::G_FLAT_MINOR: return "Gbm";
        case KeyFinder::G_MAJOR:      return "GM";
        case KeyFinder::G_MINOR:      return "Gm";
        case KeyFinder::A_FLAT_MAJOR: return "AbM";
        case KeyFinder::A_FLAT_MINOR: return "Abm";
        case KeyFinder::SILENCE:      return "";
        default:                      return "";
    }
}
