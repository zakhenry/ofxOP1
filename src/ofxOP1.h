/* ------------------------------------------------------------------------- */
/*  ofxOP1.h                                                                 */
/* ------------------------------------------------------------------------- */
/*                                                                           */
    /*  Created by Zak Henry on 26.06.11.                                    */
/*                                                                           */
/*  http://www.openFrameworks.cc                                             */
/*                                                                           */
/*  ofxOP1 breaks out the midi data from the Teenage Engineering             */
/*  OP-1 into ofEvents, with draw function for visualising the io            */
/*                                                                           */
/*                                                                           */
/* This program is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU General Public License               */
/* as published by the Free Software Foundation; either version 3            */
/* of the License, or (at your option) any later version.                    */
/*                                                                           */
/* See file LICENSE for further informations on licensing terms.             */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software Foundation,   */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA */
/*                                                                           */
/* ------------------------------------------------------------------------- */


/* IMPORTANT!
 You must include the ofxMidi addon for this addon to compile
 */

#ifndef OP_1 // if this class hasn't been defined, the program can define it
#define OP_1 // by using this if statement you prevent the class to be called more 
// than once which would confuse the compiler

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxMidi.h"

typedef struct midiPacket {
    string event;
    int elementId;
    string elementName;
    int channel;
    double timestamp;
} midiPacket;

typedef struct controlButton{
    string name;
    int midiId;
    int graphicId;
    ofImage icon;
    bool keyPressed;
    
} controlButton;

class IScreen
{
public:
    // Defines an interface named drawScreen
    virtual void drawScreen() = 0;
};

class OP1 : public ofBaseApp{
	
	//private
    
    
    
    
    float posX, posY, width;
    float cursorX, cursorY;
    float keySpacing, frameWidth;
    
    
    
    
    
    float keySpan(int span); //returns value in mm of a key spanning int grids
    
    void drawFrame();
    void drawKeyboard();
    void drawSpeakerGrille();
    void drawVolumeKnob(float volume);
    void drawButtons();
    void drawButton(int buttonNumber);
    void drawKnobs();
    void drawKnob(int knobNumber);
    
    void drawScreenBorder();
    
    
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);
    void roundedRect(float w, float h, float r);
    void roundedRect(float w, float h, float r, bool rectModeCenter);
    
    ofImage op1Logo;
    
    vector<controlButton>controlButtons;
    
    float scaleFactor;
    float volume, bEncoder, gEncoder, wEncoder, oEncoder; //angles
    
    //MIDI IN (FROM OP-1)
    
    ofxMidiIn * midiIn; //pointer to object
    void newMessageEvent (ofxMidiEventArgs & args);
    ofxMidiIn internalMidi;
    void newVirtualMessageEvent (ofxMidiEventArgs & args);
    
    bool op1Connected;
    bool connectToExternalOP1();
    int nextConnectionAttempt;
    
    
    vector<bool>buttonStatus;
    vector<bool>keyStatus;
    
    void changeButtonStatus(int keyNum, bool keyDown);
    void changeKeyStatus(int keyNum, bool keyDown);
    
    //MIDI OUT (TO OP-1)
    
    ofxMidiOut midiOut;
    
    void setEncoder(int encoder, float angle);
    void incrementEncoder(int encoder, bool cw);
    
    void buttonEvent (int key, bool keydown, string &keyName);
    void keyEvent (int key, bool keydown, string &keyName);
    
    
    
    
    bool mouseClicked;
    bool cursorInBoundingBox(float x, float y, float width, float height);
    int currentNotePlaying;
    
    void handleKeystrokes();
    
    //audio in (optional)
    void audioReceived 	(float * input, int bufferSize, int nChannels); 
    
    float * left;
    float * right;
    
    IScreen* myScreen;
    
    public:
    
    OP1(IScreen* screen);//constructor
    
//    OP1(); //constructor
    ~OP1(); //destructor
    
    
    
    void draw();
    void update(){};
    
//    void drawScreen();
    
    virtual void drawScreen();
    
    float getScreenHeight();
    float getScreenWidth();
    
    ofEvent<midiPacket> midiEvent;
    void setDimensions(int x, int y, int width);
    
    void sendNoteOn(int noteId, int keyId);
    void sendNoteOff(int noteId, int keyId);

    void mouseDown(int x, int y);
    void mouseUp();
    void updateCursorPosition(int x, int y);
    
    
};









#endif