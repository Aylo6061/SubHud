# Welcome the the Firmware Directory!

Lots going on in here, lets take a look:

-State machine machinery (files that make the state machine)
-X test
-HUDFirmware

building the firmware is done in arduino 1.8.13, and it uses many libraries with many dependencies.

# State Machine Machinery

The tool csvtosm.py accepts a .csv that describes the states and events that can happen, and outputs a header file that allows switching between the states when fed an event.  (see smtable_EXAMPLE.csv)

what this tool does not do is move that file to the HUDFirmware/src and rename it stateMachine.

the current syntax to call this is:

`python csvtosm.py inputfilename.csv outputifilename.h`

# X test

Ideally I would like to have a suite of test code to exercise the libraries against real hardware (or to exercise hardware against code). the arduino IDE is a little annoying about this, because it does not like relative paths.

In order for this to work nicely, I will need to do some careful testing/copying, or to write some python to make sure the libraries are synced up.

# HUDFirmware

This is where the actual FW lives /src has libraries in it and /data has data that needs uploading to SPIFFS.

# TODO

Its managable but painful to carefully copy everything from place to place as I develop this, but it would be much nicer if I didnt have to do that.