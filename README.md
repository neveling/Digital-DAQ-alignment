# Digital-DAQ-alignment/Thagomizer*

Data were acquired in a K600+BaGeL experiment where the signals from the HPGe detectors 
were digitized in both the K600 VME-based TRIUMF-MIDAS DAQ (event-by-event, with event defined by the K600 focal plane), 
as well as a free-runing PIXIE16-based DAQ, the so-called Pixie Acquisition and Analysis Software System (PAASS) DAQ. For more on this please see https://github.com/spaulaus/paass-lc.

The time difference between the K600 trigger and a fixed pulser was converted to a pulseheight through a TAC. In this way both DAQs were supplied with a randomly changing pulseheight for each K600 trigger. It is this random pulseheight that should be used to align the HPGe data from the digital DAQ with the K600 focal plane data.

There are 2 rootfiles in this repository. 
sorted00337.root 
is the K600 DAQ root file, and the TAC pulseheight can be found in the leave called "TAC".
The rootfile created from the digital DAQ is called 
runBaGeL_337-tree.root
and the TAC pulseheight is to be found in the leave "tac_energy".

The very basic script trying to align K600 DAQ with digital DAQ output is calle FileCompare5.
The assumption is that the ration of the TAC/tac_energy should be more or less constant, and that the moment 
you find it to fall outside of some predefined band, then the 2 events are no-longer aligned, and you have to
ignore 1 or a few digital DAQ events before you can find the alignment again; so you have to skip a few digital DAQ events.

*from PA: "If there's a separate code for this I would like to make a claim for "Thagomizer" for the name. See here: https://en.wikipedia.org/wiki/Thagomizer
I can come up with some reason for this if you want like how it flails backwards and forwards but mainly it's because I like the name."
