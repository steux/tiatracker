# TIATracker #

A music tracker for making Atari VCS 2600 music on the PC, including a new sound routine for the VCS.

(c) 2016 by Andre "Kylearan" Wichmann (andre.wichmann@gmx.de)

* Manual: [https://bitbucket.org/kylearan/tiatracker/raw/master/data/TIATracker_manual.pdf](https://bitbucket.org/kylearan/tiatracker/raw/master/data/TIATracker_manual.pdf)
* Windows binaries: [http://www.compoundeye.net/demoscene/TIATracker.zip](http://www.compoundeye.net/demoscene/TIATracker.zip)
* For Linux and OS X, use "wine" or compile the source (uses Qt and SDL)
* Source: [https://bitbucket.org/kylearan/tiatracker](https://bitbucket.org/kylearan/tiatracker)
* Seminar talk from Revision 2016 about VCS music in general and TIATracker in particular: [https://www.youtube.com/watch?v=PVujzQySZls](https://www.youtube.com/watch?v=PVujzQySZls)

## Features ##

VCS sound routine features:

* Up to 7 melodic and 15 percussion instruments
* ADSR envelopes for volume and frequency for melodic instruments
* "Overlay" percussion which will play the next melodic notes immediately
* Arbitrary and variable pattern lengths individual for each channel
* Highly optimized and configurable replayer routine
* Richly documented source code, including specifications for all data structures

Tracker features:

* Graphical representation for available notes per waveform and how off-tune they are
* Customizable pitch guides optimizing the number of in-tune notes
* Graphical editors for melodic and percussion instruments
* Integrated pattern editor and sequencer
* Timeline "mini map" displaying the pattern sequences
* Sound emulation from the Stella emulator for playback
* Export to dasm

## Planned enhancements

* Export to .k65
* More precise playback timing in the tracker emulation
* Undo/Redo
* Backport the upcoming major improvements in Stella's sound emulation to the tracker

For feedback, bug reports and feature requests, send a mail to andre.wichmann@gmx.de!