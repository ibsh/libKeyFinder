# `libKeyFinder`

[![Build Status](https://travis-ci.org/ibsh/libKeyFinder.svg?branch=master)](https://travis-ci.org/ibsh/libKeyFinder)

`libKeyFinder` is a small C++11 library for estimating the musical key of digital audio. It is published under the GNU General Public License version 3 or later.

It was written by [Ibrahim Shaath](http://ibrahimshaath.co.uk/keyfinder/) who wrote it in 2011 as part of a master's thesis in computer science. A [GUI application](https://github.com/ibsh/is_KeyFinder) to use it is available for [macOS](http://www.ibrahimshaath.co.uk/keyfinder/bins/KeyFinder-OSX-2-4.zip) and [Windows](http://www.ibrahimshaath.co.uk/keyfinder/bins/KeyFinder-WIN-1-25.zip), however that is no longer maintained and does not build on contemporary Linux distributions.

In 2020, Ibrahim handed over maintenance of libKeyFinder to the [Mixxx DJ software](https://mixxx.org) team who incorporated it into Mixxx as of Mixxx 2.3. If you want to discuss anything related to libKeyFinder with us, please get in touch on the [Mixxx Zulip chat](https://mixxx.zulipchat.com/#narrow/stream/109171-development/topic/KeyFinder). Contributions are welcome by opening pull requests and issues on [GitHub](https://github.com/ibsh/libKeyFinder).

## Examples

For the most basic use case, do something like this:

```C++
// Static because it retains useful resources for repeat use
static KeyFinder::KeyFinder k;

// Build an empty audio object
KeyFinder::AudioData a;

// Prepare the object for your audio stream
a.setFrameRate(yourAudioStream.framerate);
a.setChannels(yourAudioStream.channels);
a.addToSampleCount(yourAudioStream.length);

// Copy your audio into the object
for (int i = 0; i < yourAudioStream.length; i++) {
  a.setSample(i, yourAudioStream[i]);
}

// Run the analysis
KeyFinder::KeyDetectionResult r =  k.keyOfAudio(a);

// And do something with the result
doSomethingWith(r.globalKeyEstimate);
```

Alternatively, you can transform a stream of audio into a chromatic representation, and make progressive estimates of the key:

```C++
KeyFinder::AudioData a;
a.setFrameRate(yourAudioStream.framerate);
a.setChannels(yourAudioStream.channels);
a.addToSampleCount(yourAudioStream.packetLength);

static KeyFinder::KeyFinder k;

// the workspace holds the memory allocations for analysis of a single track
KeyFinder::Workspace w;

while (someType yourPacket = newAudioPacket()) {

  for (int i = 0; i < yourPacket.length; i++) {
    a.setSample(i, yourPacket[i]);
  }
  k.progressiveChromagram(a, w);

  // if you want to grab progressive key estimates...
  KeyFinder::KeyDetectionResult r = k.keyOfChromagram(w);
  doSomethingWithMostRecentKeyEstimate(r.globalKeyEstimate);
}

// if you only want a single key estimate, or to squeeze
// every last bit of audio from the working buffer after 
// progressive estimates...
k.finalChromagram(w);

// and finally...
KeyFinder::KeyDetectionResult r = k.keyOfChromagram(w);

doSomethingWithFinalKeyEstimate(r.globalKeyEstimate);
```

## Installation

First, you will need to install `libKeyFinder`'s dependencies:

* [FFTW version 3](http://www.fftw.org/download.html)

  OSX and homebrew: `$ brew install fftw`

Once dependencies are installed, build `libKeyFinder`:

```sh
$ mkdir build
$ cmake -DCMAKE_INSTALL_PREFIX /where/you/want/to/install/to -S . -B build
$ cmake --build build --target install --parallel number-of-cpu-cores
```

If you want to build libKeyFinder statically, add `-DBUILD_STATIC_LIBS` to the first call to `cmake` above.

## Testing

The tests are built together with the library. Simply run the test executable from the build directory:

```sh
$ build/tests/test
```

If all goes well, you should see something like this:

```
===============================================================================
All tests passed (1705510 assertions in 65 test cases)
```

Note that there is a known intermittent failure in the `FftAdapterTest/ForwardAndBackward` test. Try running the tests a handful of times to determine whether you are hitting the intermittent or have introduced a new bug.

