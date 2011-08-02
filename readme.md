#ofxOP1

This is to be an openframeworks addon for the [Teenage Engineering OP-1](http://www.teenageengineering.com/products/op-1)

##Current plan

To simplify full midi IO for the OP-1 with openframeworks

##Status
*   Rough graphical simulation of the OP-1
*   Midi input controlling encoders on screen
*   Midi input controlling control buttons on screen
*   Midi input controllong keyboard keys on screen (with octave correction when in CTRL mode)
*   On screen keyboard (controlled by mouse) giving midi output to OP-1
*   OP1 screen shows stereo visualisation
*   Midi data passed through a ofEvent listener

###Dependencies
*   [ofxMidi.h](https://github.com/kylemcdonald/TheJanusMachine/tree/365e15c1966ca3f61e93859e32a2422d9ebadc70/CaptureApp/src/addons/ofxMidi) (and linking to the Core MIDI framework
*   ofEvents.h


