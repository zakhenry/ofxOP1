/* ------------------------------------------------------------------------- */
/*  ofxOP1.cpp                                                               */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/*  Created by Zak Henry on 26.06.11.                                        */
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

#include "ofxOP1.h"

//colours grabbed from the pantone chart on the main site


int keyMap[] = {53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 54, 56, 58, 61, 63, 66, 68, 70, 73, 75};

//void OP1(OP1Screen* screen): myScreen(screen){
//    
//}

OP1::OP1(){ // constructor
//OP1::OP1(IScreen* screen): myScreen(screen){
    
    cout << "OP-1 created\n";
    
    ofSetDataPathRoot("../Resources/");
    
    blue = 0x0098D4;
    green = 0x01BB00;
    lightGrey = 0xDEE8E9;
    orange = 0xFE4F00;
    darkGrey = 0xC3C9C9;
    black = 0x000000;
    white = 0xFFFFFF;
    
    ofSetCircleResolution(100);
    
    ofFilePath filePath;
//    cout << "current working directory is: "<<filePath.getCurrentWorkingDirectory() << endl;
    
    keySpacing = 0.7;
    frameWidth = 4.5;

    ofSetRectMode(OF_RECTMODE_CENTER);
    
    midiIn = new ofxMidiIn;

    op1Connected = connectToExternalOP1();
    nextConnectionAttempt = ofGetSystemTime()+5000;
    
    internalMidi.openVirtualPort("Virtual OP-1");
    
    
    ofAddListener(internalMidi.newMessageEvent, this, &OP1::newVirtualMessageEvent); //send data to same callback as the external midi in
    
    for (int i=0; i<34; i++){
        buttonStatus.push_back(false);
    }
    
    for (int i=0; i<24; i++){
        keyStatus.push_back(false);
    }
    
    midiOut.openPort(0);
    
    currentNotePlaying = -1;
    
    // 0 output channels, 
	// 2 input channels
	// 22050 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	
    	ofSoundStreamSetup(0,2,this, 44100, 256, 4);	
    	left = new float[256];
    	right = new float[256];
    
    ofEnableAlphaBlending();
    
    op1Logo.loadImage("op1.png");
    
    //defining the buttons
    
    controlButton mic;
    mic.name = "Mic/Input";
    mic.midiId = 48;
    mic.graphicId = 0;
    mic.icon.loadImage("mic.png");
    controlButtons.push_back(mic);
    
    controlButton com;
    com.name = "Album/Computer Connection";
    com.midiId = 49;
    com.graphicId = 1;
    com.icon.loadImage("com.png");
    controlButtons.push_back(com);
    
    controlButton synth;
    synth.name = "Synthesizer";
    synth.midiId = 7;
    synth.graphicId = 2;
    synth.icon.loadImage("synth.png");
    controlButtons.push_back(synth);
    
    controlButton drum;
    drum.name = "Drum";
    drum.midiId = 8;
    drum.graphicId = 3;
    drum.icon.loadImage("drum.png");
    controlButtons.push_back(drum);
    
    controlButton tape;
    tape.name = "Tape";
    tape.midiId = 9;
    tape.graphicId = 4;
    tape.icon.loadImage("tape.png");
    controlButtons.push_back(tape);
    
    controlButton mixer;
    mixer.name = "Mixer";
    mixer.midiId = 10;
    mixer.graphicId = 5;
    mixer.icon.loadImage("mix.png");
    controlButtons.push_back(mixer);
    
    controlButton one;
    one.name = "One";
    one.midiId = -1;
    one.graphicId = 6;
    one.icon.loadImage("one.png");
    controlButtons.push_back(one);
    
    controlButton two;
    two.name = "Two";
    two.midiId = -1;
    two.graphicId = 7;
    two.icon.loadImage("two.png");
    controlButtons.push_back(two);
    
    controlButton three;
    three.name = "Three";
    three.midiId = -1;
    three.graphicId = 8;
    three.icon.loadImage("three.png");
    controlButtons.push_back(three);
    
    controlButton four;
    four.name = "Four";
    four.midiId = -1;
    four.graphicId = 9;
    four.icon.loadImage("four.png");
    controlButtons.push_back(four);
    
    controlButton voice_1;
    voice_1.name = "Voice 1/";
    voice_1.midiId = 50;
    voice_1.graphicId = 10;
    voice_1.icon.loadImage("voice_1.png");
    controlButtons.push_back(voice_1);
    
    controlButton voice_2;
    voice_2.name = "Voice 1/";
    voice_2.midiId = 51;
    voice_2.graphicId = 11;
    voice_2.icon.loadImage("voice_2.png");
    controlButtons.push_back(voice_2);
    
    controlButton voice_3;
    voice_3.name = "Voice 1/";
    voice_3.midiId = 52;
    voice_3.graphicId = 12;
    voice_3.icon.loadImage("voice_3.png");
    controlButtons.push_back(voice_3);
    
    controlButton voice_4;
    voice_4.name = "Voice 1/";
    voice_4.midiId = 21;
    voice_4.graphicId = 13;
    voice_4.icon.loadImage("voice_4.png");
    controlButtons.push_back(voice_4);
    
    controlButton voice_5;
    voice_5.name = "Voice 1/";
    voice_5.midiId = 22;
    voice_5.graphicId = 14;
    voice_5.icon.loadImage("voice_5.png");
    controlButtons.push_back(voice_5);
    
    controlButton voice_6;
    voice_6.name = "Voice 1/";
    voice_6.midiId = 23;
    voice_6.graphicId = 15;
    voice_6.icon.loadImage("voice_6.png");
    controlButtons.push_back(voice_6);
    
    controlButton voice_7;
    voice_7.name = "Voice 1/";
    voice_7.midiId = 24;
    voice_7.graphicId = 16;
    voice_7.icon.loadImage("voice_7.png");
    controlButtons.push_back(voice_7);
    
    controlButton voice_8;
    voice_8.name = "Voice 1/";
    voice_8.midiId = 25;
    voice_8.graphicId = 17;
    voice_8.icon.loadImage("voice_8.png");
    controlButtons.push_back(voice_8);
    
    controlButton sequencer;
    sequencer.name = "Sequencer";
    sequencer.midiId = 26;
    sequencer.graphicId = 18;
    sequencer.icon.loadImage("sequencer.png");
    controlButtons.push_back(sequencer);
    
    controlButton help;
    help.name = "Help";
    help.midiId = 5;
    help.graphicId = 19;
    help.icon.loadImage("help.png");
    controlButtons.push_back(help);
    
    controlButton metro;
    metro.name = "Metronome";
    metro.midiId = 6;
    metro.graphicId = 20;
    metro.icon.loadImage("metro.png");
    controlButtons.push_back(metro);
    
    controlButton lift;
    lift.name = "Lift";
    lift.midiId = 15;
    lift.graphicId = 21;
    lift.icon.loadImage("lift.png");
    controlButtons.push_back(lift);
    
    controlButton drop;
    drop.name = "Drop";
    drop.midiId = 16;
    drop.graphicId = 22;
    drop.icon.loadImage("drop.png");
    controlButtons.push_back(drop);
    
    controlButton join;
    join.name = "Join";
    join.midiId = 17;
    join.graphicId = 23;
    join.icon.loadImage("join.png");
    controlButtons.push_back(join);
    
    controlButton record;
    record.name = "Record";
    record.midiId = 38;
    record.graphicId = 24;
    record.icon.loadImage("record.png");
    controlButtons.push_back(record);
    
    controlButton play;
    play.name = "Play";
    play.midiId = 39;
    play.graphicId = 25;
    play.icon.loadImage("play.png");
    controlButtons.push_back(play);
    
    controlButton stop;
    stop.name = "Stop";
    stop.midiId = 40;
    stop.graphicId = 26;
    stop.icon.loadImage("stop.png");
    controlButtons.push_back(stop);
    
    controlButton arrow_left;
    arrow_left.name = "Octave Down/Rewind";
    arrow_left.midiId = 41;
    arrow_left.graphicId = 27;
    arrow_left.icon.loadImage("arrow_left.png");
    controlButtons.push_back(arrow_left);
    
    controlButton arrow_right;
    arrow_right.name = "Octave Up/Fast Forward";
    arrow_right.midiId = 42;
    arrow_right.graphicId = 28;
    arrow_right.icon.loadImage("arrow_right.png");
    controlButtons.push_back(arrow_right);
    
    controlButton shift;
    shift.name = "Shift";
    shift.midiId = -1;
    shift.graphicId = 29;
    shift.icon.loadImage("shift.png");
    controlButtons.push_back(shift);
    
    controlButton encoder_blue;
    encoder_blue.name = "encoder_blue";
    encoder_blue.midiId = 64;
    encoder_blue.graphicId = -1;
    controlButtons.push_back(encoder_blue);
    
    controlButton encoder_green;
    encoder_green.name = "encoder_green";
    encoder_green.midiId = 65;
    encoder_green.graphicId = -1;
    controlButtons.push_back(encoder_green);
    
    controlButton encoder_white;
    encoder_white.name = "encoder_white";
    encoder_white.midiId = 66;
    encoder_white.graphicId = -1;
    controlButtons.push_back(encoder_white);
    
    controlButton encoder_orange;
    encoder_orange.name = "encoder_orange";
    encoder_orange.midiId = 67;
    encoder_orange.graphicId = -1;
    controlButtons.push_back(encoder_orange);
    
}

OP1::~OP1(){
    delete midiIn;
}

bool OP1::connectToExternalOP1(){
    
    delete midiIn;
    midiIn = new ofxMidiIn;
    
    midiIn->listPorts();
    int op1Port = -1;
    for (int portNum = 0; portNum<midiIn->portNames.size(); portNum++) {
        string portName = midiIn->portNames[portNum];
        string match = "OP-1";
        size_t found;
        
        found=portName.rfind(match);
        if (found!=string::npos){
            op1Port = portNum;
        }
//        cout << "port "<<portNum<<" is labelled "<<midiIn->portNames[portNum]<<endl;
    }
    
    if (op1Port>-1) {
        
//        ofxMidiIn midiIn; //reconstruct? hopefully this isn't leaky
        
        midiIn->openPort(op1Port);
        midiIn->setVerbose(false);
        ofAddListener(midiIn->newMessageEvent, this, &OP1::newMessageEvent);
//        cout << "OP1 found, listing ports\n";
        midiIn->listPorts();
        
        return true;
    }else{
        cout <<"OP1 external device not found"<<endl;
    }
    
    return false;
}

float OP1::keySpan(int span){
    return ((14.7*span)+keySpacing*(span-1));
}

void OP1::setDimensions(int _x, int _y, int _width){
    posX = _x;
    posY = _y;
    width = _width;
    
    scaleFactor = width/284;
}

void OP1::draw(){ //all is drawn as 1px = 1mm, then scaled up
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    if(!op1Connected){
        if (nextConnectionAttempt<ofGetSystemTime()){
//            cout << "attempting reconnection\n";
            op1Connected = connectToExternalOP1();
            nextConnectionAttempt = ofGetSystemTime()+5000; //retry every 5 seconds
        }else{
//            cout << "reconnecting in"<<nextConnectionAttempt-ofGetSystemTime()<<" us\n";
        }
//        cout <<"nextConnectionAttempt is: "<<nextConnectionAttempt<<endl;
    }
    
    ofPushMatrix();
        
        
        ofTranslate(posX, posY);
        ofScale(scaleFactor, scaleFactor);
        ofTranslate(-(284/2), -(102/2)); //set coords to top left
        
        

        drawFrame();
        drawKeyboard();
        drawSpeakerGrille();
        drawVolumeKnob(1);
        drawKnobs();
        drawButtons();
        
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(frameWidth+keySpan(4)+keySpacing*2, frameWidth+keySpacing);
        drawScreenBorder();
//        drawOP1Screen();
        ofPopStyle();
        ofPopMatrix();
    
//    ofSetHexColor(0x0000ff);
//    ofCircle(cursorX, cursorY, 2);
    
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(cursorX, cursorY);
    ofPopMatrix();
    
}

float OP1::getScreenHeight(){
    return keySpan(2);
}

float OP1::getScreenWidth(){
    return keySpan(4);
}

void OP1::getScreenDimensions(float &x, float&y, float&w, float&h){
    
//    x = posX;
    
    x = posX-(((284/2)-(frameWidth+keySpan(4)+keySpacing*2))*scaleFactor);
    y = posY-(((102/2)-(frameWidth+keySpacing))*scaleFactor);
    
    w = getScreenWidth()*scaleFactor;
    h = getScreenHeight()*scaleFactor;
    
    return;
}


void OP1::drawFrame(){
    
//    ofSetHexColor(0xff0000);
//    ofCircle(0, 0, 10);
    
    ofPushMatrix();
        ofSetHexColor(darkGrey);
        roundedRect(284, 102, 4);
        ofSetHexColor(black);
    ofTranslate(frameWidth, frameWidth);
        roundedRect(keySpan(17)+2*keySpacing, keySpan(6)+2*keySpacing, 2);
    ofPopMatrix();
    
    //mic grill
    
    ofPushMatrix();
    ofTranslate(274, 11.05);
        ofCircle(0, 0, 0.5);
        ofCircle(2.5, 0, 0.5);
        ofCircle(0, 2.5, 0.5);
        ofCircle(2.5, 2.5, 0.5);
    ofPopMatrix();
    
    //vu grill
    
    ofPushMatrix();
    ofTranslate(275.5, 37);
        ofCircle(0, 0, 0.5);
        ofCircle(0, 3, 0.5);
        ofCircle(0, 6, 0.5);
        ofCircle(0, 9, 0.5);
        ofCircle(0, 12, 0.5);
    ofPopMatrix();
    
    
    
    ofPushMatrix();
        ofTranslate(274.5, 82);
        ofScale(0.06, 0.06);
        op1Logo.draw(0,0);
//        ofRotateZ(-90);
//        ofScale(0.07, 0.07);
//        ofSetHexColor(0x555555);
//        spyroclassic.drawString("OP-1", 0, 0);
    ofPopMatrix();
    
}

void OP1::drawKeyboard(){
    
    ofPushMatrix();
    ofTranslate(frameWidth+2*keySpacing+keySpan(3), frameWidth+2*keySpacing+keySpan(4));
    
    int keyNumber = 0;
        
    //white notes
    for (int i=0; i<14; i++){
        ofPushMatrix();
        
            ofSetHexColor(darkGrey);
            roundedRect(keySpan(1), keySpan(2), 0.5);
            ofTranslate(keySpan(1)/2, keySpan(2)/2);
            ofSetHexColor(lightGrey);
            roundedRect(10, 25, 5, true);
        
        
        
        if (keyStatus[keyNumber]){
            ofSetHexColor(0x000000);
            ofCircle(0, 0, 3);
        }
            
        ofPopMatrix();
        
        ofTranslate(keySpan(1)+keySpacing, 0);
        
        keyNumber++;
    }
    
    ofPopMatrix();
    //black notes
    float blackSpan = (keySpan(4)-(keySpan(1)+2*keySpacing))/2;
    ofPushMatrix();
    ofTranslate(frameWidth+2*keySpacing+keySpan(3), frameWidth+2*keySpacing+keySpan(3));
    for (int i=1; i<=10; i++){
        ofPushMatrix();
        ofSetHexColor(darkGrey);
        
        if (i==2||i==7){
            roundedRect(keySpan(1), keySpan(1), 0.5);
            ofTranslate(blackSpan/3, keySpan(1)/2);
        }else if (i==3||i==5||i==8||i==10){
            roundedRect(blackSpan, keySpan(1), 0.5);
            ofTranslate(blackSpan/3, keySpan(1)/2);
        }else{
            roundedRect(blackSpan, keySpan(1), 0.5);
            ofTranslate(2*blackSpan/3, keySpan(1)/2);
        }
        
        ofSetHexColor(lightGrey);
        ofCircle(0, 0, 5);
        ofSetHexColor(black);
        ofCircle(0, 0, 4.5);
        if (keyStatus[keyNumber]){
            ofSetHexColor(0xffffff);
            ofCircle(0, 0, 2.5);
        }
        
        ofPopMatrix();
        
        if (i==2||i==7){
            ofTranslate(keySpan(1)+keySpacing, 0);
        }else{
            ofTranslate(blackSpan+keySpacing, 0);
        }
     keyNumber++;   
    }
    ofPopMatrix();
        
    
    
    
}

void OP1::drawSpeakerGrille(){
    ofPushMatrix();
    ofTranslate(frameWidth+keySpacing, frameWidth+keySpacing);
        ofSetHexColor(darkGrey);
        roundedRect(keySpan(2), keySpan(2), 0.5);
    
    ofTranslate(1.2, 1.2);
    ofSetHexColor(black);
    for (int x=1; x<=13; x++){
        for (int y=1; y<=13; y++){
            //this is truly a horrific implementation. needs fixing
            if (!(x==1&&y==1)&&!(x==2&&y==1)&&!(x==3&&y==1)&&!(x==1&&y==2)&&!(x==1&&y==3)){
                if(!(x==11&&y==1)&&!(x==12&&y==1)&&!(x==13&&y==1)&&!(x==13&&y==2)&&!(x==13&&y==3)){
                    if(!(x==1&&y==11)&&!(x==1&&y==12)&&!(x==1&&y==13)&&!(x==2&&y==13)&&!(x==3&&y==13)){
                        if(!(x==13&&y==11)&&!(x==13&&y==12)&&!(x==13&&y==13)&&!(x==12&&y==13)&&!(x==11&&y==13)){
                            ofCircle(x*2, y*2, 0.6);
                        }
                    }
                }
            }
        }
    }
    
    ofPopMatrix();
}

void OP1::drawVolumeKnob(float volume){
    ofPushMatrix();
    ofTranslate(frameWidth+2*keySpacing+keySpan(2), frameWidth+keySpacing);
        ofSetHexColor(darkGrey);
        roundedRect(keySpan(2), keySpan(1), 0.5);
        ofTranslate(7, 7);
        ofSetHexColor(white);
        ofCircle(0, 0, 5);
        ofRotateZ(volume*360*0.8+135);
        ofTranslate(3, 0);
        ofSetHexColor(lightGrey);
        ofCircle(0, 0, 1.4);
        ofSetHexColor(white);
        ofCircle(0, 0, 1);
    
    ofPopMatrix();
}

void OP1::drawButton(int buttonNumber){
    ofSetHexColor(darkGrey);
    roundedRect(keySpan(1), keySpan(1), 0.5);
    ofPushMatrix();
        ofTranslate(keySpan(1)/2, keySpan(1)/2);
        ofSetHexColor(lightGrey);
        ofCircle(0, 0, 5);
        
        ofScale(0.1, 0.1);
        if (controlButtons[buttonNumber].graphicId>=0){ //only draw if it has a graphic id
            controlButtons[buttonNumber].icon.draw(0,0);
        }
    ofPopMatrix();
    
    if (buttonStatus[buttonNumber]){
        //highlight keydown
        ofPushMatrix();
        ofTranslate(14.5/2, 14.5/2);
        ofSetHexColor(0xff0000);
        ofCircle(0, 0, 3);
        ofPopMatrix();
    }
    
}

void OP1::drawButtons(){
    
    int buttonNumber = 0;
    
    ofSetHexColor(lightGrey); //png matte
    
    ofPushMatrix();
    ofTranslate(frameWidth+2*keySpacing+keySpan(16), frameWidth+keySpacing);
    for (int i=1; i<=2; i++){
        drawButton(buttonNumber);
        ofTranslate(0, keySpan(1)+keySpacing);
        buttonNumber++;
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(frameWidth+keySpacing, frameWidth+2*keySpacing+keySpan(2));
    for (int i=1; i<=17; i++){
        drawButton(buttonNumber);
        ofTranslate(keySpan(1)+keySpacing, 0);
        buttonNumber++;
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(frameWidth+2*keySpacing+keySpan(2), frameWidth+2*keySpacing+keySpan(1));
    for (int i=1; i<=2; i++){
        drawButton(buttonNumber);
        ofTranslate(keySpan(1)+keySpacing, 0);
        buttonNumber++;
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(frameWidth+keySpacing, frameWidth+2*keySpacing+keySpan(3));
    for (int i=0; i<=2; i++){
        for (int j=0; j<=2; j++){
            ofPushMatrix();
                ofTranslate((keySpacing+keySpan(1))*j, (keySpacing+keySpan(1))*i);
                drawButton(buttonNumber);
                buttonNumber++;
            ofPopMatrix();
        }
    }
    ofPopMatrix();
        
    
}

void OP1::drawKnob(int knobNumber){
    ofSetHexColor(darkGrey);
    roundedRect(keySpan(2), keySpan(2), 0.5);
    ofPushMatrix();
    ofTranslate(keySpan(2)/2, keySpan(2)/2);
    ofSetHexColor(0xaaaaaa);
    ofCircle(0, 0, 9);
    ofSetHexColor(lightGrey);
    ofCircle(0, 0, 8.5);
    
    int buttonNumber = -1;
    
    int color = 0x000000; 
    float angle;
    switch (knobNumber) {
        case 0:
            color = blue;
            angle = bEncoder;
            buttonNumber = 30;
            break;
        case 1:
            color = green;
            angle = gEncoder;
            buttonNumber = 31;
            break;
        case 2:
            color = white;
            angle = wEncoder;
            buttonNumber = 32;
            break;
        case 3:
            color = orange;
            angle = oEncoder;
            buttonNumber = 33;
            break;
            
        default:
            break;
    }
    
    if (buttonNumber>0){
        if (buttonStatus[buttonNumber]){
            ofSetHexColor(0x000000);
            ofCircle(0, 0, 7);
        }
    }
    ofSetHexColor(color);
    ofCircle(0, 0, 5);
    
    ofRotateZ(angle*360);
    
    ofTranslate(-3, -1);
    ofSetHexColor(0x888888);
    roundedRect(6, 2, 1);
    ofSetHexColor(color);
    ofTranslate(0.2, 0.1);
    ofScale(0.9, 0.9);
    roundedRect(6, 2, 1);
    
    ofPopMatrix();
}

void OP1::drawKnobs(){
    ofPushMatrix();
    ofTranslate(frameWidth+2*keySpacing+keySpan(8), frameWidth+keySpacing);
    for (int i=0; i<4; i++){
        drawKnob(i);
        ofTranslate(keySpan(2)+keySpacing, 0);
    }
    ofPopMatrix();
}

void OP1::roundedRect(float w, float h, float r, bool rectModeCenter) {
    
    ofPushMatrix();
    if (rectModeCenter){
        ofTranslate(-(w/2), -(h/2));
    }
    
    ofBeginShape();
    ofVertex(r, 0);
    ofVertex(w-r, 0);
    quadraticBezierVertex(w, 0, w, r, w-r, 0);
    ofVertex(w, h-r);
    quadraticBezierVertex(w, h, w-r, h, w, h-r);
    ofVertex(r, h);
    quadraticBezierVertex(0, h, 0, h-r, r, h);
    ofVertex(0, r);
    quadraticBezierVertex(0, 0, r, 0, 0, r);
    ofEndShape();
    
    ofPopMatrix();
}

void OP1::roundedRect(float w, float h, float r) { //function overloader
    roundedRect(w, h, r, false);
}

void OP1::quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
    float cp1x = prevX + 2.0/3.0*(cpx - prevX);
    float cp1y = prevY + 2.0/3.0*(cpy - prevY);
    float cp2x = cp1x + (x - prevX)/3.0;
    float cp2y = cp1y + (y - prevY)/3.0;
    
    // finally call cubic Bezier curve function
    ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
}



/*Setting values*/

void OP1::setEncoder(int encoder, float angle){
    
    
    switch (encoder) {
        case 'b':
            bEncoder = angle;
            break;
            
        case 'g':
            gEncoder = angle;
            break;
            
        case 'w':
            wEncoder = angle;
            break;
            
        case 'o':
            oEncoder = angle;
            break;
            
        default:
            cout <<"bad encoder id\n";
            break;
    }
    
}

void OP1::incrementEncoder(int encoder, bool cw, string &encoderName){
    
    
    switch (encoder) {
        case 1:
            bEncoder = cw ? bEncoder+((float)1/32):bEncoder-((float)1/32);
            encoderName = "encoder_blue";
            break;
            
        case 2:
            gEncoder = cw ? gEncoder+((float)1/32):gEncoder-((float)1/32);
            encoderName = "encoder_green";
            break;
            
        case 3:
            wEncoder = cw ? wEncoder+((float)1/32):wEncoder-((float)1/32);
            encoderName = "encoder_white";
            break;
            
        case 4:
            oEncoder = cw ? oEncoder+((float)1/32):oEncoder-((float)1/32);
            encoderName = "encoder_orange";
            break;
            
        default:
            cout <<"bad encoder id\n";
            break;
    }
    
}

void OP1::changeButtonStatus(int buttonNum, bool buttonDown){
    if (buttonNum>=0){
        buttonStatus[buttonNum] = buttonDown;
//        cout << "button ["<<buttonNum<<"] status changed to ["<<buttonDown<<"]\n";
    }else{
//        cout <<"button not found\n";
    }
    
}

void OP1::buttonEvent(int button, bool buttonDown, string& buttonName){
    int buttonNum = -1; //changing midi id's to graphical ids
    
//    cout << "button checked for is "<<button<<" \n";
    
    for (int i=0; i<=controlButtons.size(); i++){
//        cout << "controlButtons[i].midiId is "<<controlButtons[i].midiId<<"\n";
        if (controlButtons[i].midiId==button){
            buttonName = controlButtons[i].name;
//            cout << "button with index "<<i<<"matches midi button "<<button<<"\n";
            buttonNum = i;
            break;
        }
    }
    
    if (buttonDown){
        changeButtonStatus(buttonNum, buttonDown);
    }else{
        changeButtonStatus(buttonNum, buttonDown);
    }
}

int OP1::midiToKeyId(int midiId, string &keyName){
    
//    cout << "midi id is "<<midiId<<endl;
    
    int key = midiId-53;
    
    while (key<0||key>23) {
        if(key<0) key+= 24;
        if(key>23) key-=24;
    }
    
    int keyNum = -1;
    keyName = "(Not Found)";
    
    
     switch (key) {
     //white keys
     case 0:
     keyNum = 0;
     keyName = "F";
     break;
     case 2:
     keyNum = 1;
     keyName = "G";
     break;
     case 4:
     keyNum = 2;
     keyName = "A";
     break;
     case 6:
     keyNum = 3;
     keyName = "B";
     break;
     case 7:
     keyNum = 4;
     keyName = "C";
     break;
     case 9:
     keyNum = 5;
     keyName = "D";
     break;
     case 11:
     keyNum = 6;
     keyName = "E";
     break;
     case 12:
     keyNum = 7;
     keyName = "F";
     break;
     case 14:
     keyNum = 8;
     keyName = "G";
     break;
     case 16:
     keyNum = 9;
     keyName = "A";
     break;
     case 18:
     keyNum = 10;
     keyName = "B";
     break;
     case 19:
     keyNum = 11;
     keyName = "C";
     break;
     case 21:
     keyNum = 12;
     keyName = "D";
     break;
     case 23:
     keyNum = 13;
     keyName = "E";
     break;
     
     //black keys
     
     case 1:
     keyNum = 14;
     keyName = "F#";
     break;
     case 3:
     keyNum = 15;
     keyName = "G#";
     break;
     case 5:
     keyNum = 16;
     keyName = "A#";
     break;
     case 8:
     keyNum = 17;
     keyName = "C#";
     break;
     case 10:
     keyNum = 18;
     keyName = "D#";
     break;
     case 13:
     keyNum = 19;
     keyName = "F#";
     break;
     case 15:
     keyNum = 20;
     keyName = "G#";
     break;
     case 17:
     keyNum = 21;
     keyName = "A#";
     break;
     case 20:
     keyNum = 22;
     keyName = "C#";
     break;
     case 22:
     keyNum = 23;
     keyName = "D#";
     break;
     
     default:
     //            cout <<"key not found\n";
     break;
     }
    
//    cout << "key id is "<<key<<endl;
    
    return keyNum;
    
}

void OP1::keyEvent(int key, bool keyDown, string& keyName){
    int keyNum = -1; //changing midi id's to graphical ids
    
//    cout <<"key input is ["<<key<<"]\n";
    
//    if (((key+3)%12)==0) {
//        cout << "A pressed\n";
//    }
    
    keyNum = midiToKeyId(key, keyName);

    
    if (keyDown){
        changeKeyStatus(keyNum, keyDown);
    }else{
        changeKeyStatus(keyNum, keyDown);
    }
}

void OP1::changeKeyStatus(int keyNum, bool keyDown){
    if (keyNum>=0){
        keyStatus[keyNum] = keyDown;
//        cout << "key ["<<keyNum<<"] status changed to ["<<keyDown<<"]\n";
    }else{
//        cout <<"key not found\n";
    }
    
}

void OP1::newMessageEvent (ofxMidiEventArgs & args){
    
    int port = args.port;
	int	channel = args.channel;
	int status = args.status;
	int byteOne = args.byteOne;
	int byteTwo = args.byteTwo;
	double 	timestamp = args.timestamp;
    
    string event;
    string elementName;
    
    if (status == 176){ //control input
        if (byteOne<=4){ //encoder
            incrementEncoder(byteOne, byteTwo==1, elementName);
            event = (byteTwo==1)?"encoder_cw":"encoder_ccw";
        }else{
            buttonEvent(byteOne, byteTwo==127, elementName); //buttonid buttondown
            event = (byteTwo==127)?"button_down":"button_up";
        }
    }else{ //keyboard input?
//        while (byteOne>64) { //octave shifting
//            byteOne-=24;
//            
//        }
//        
//        while (byteOne<53) { //octave shifting
//            byteOne+=24;
//        }
        
        keyEvent(byteOne, status==144, elementName); //keyid keydown
        event = (status==144)?"key_down":"key_up";
    }
    
    midiPacket newPacket;
    newPacket.channel = 0;
    newPacket.event = event;
    newPacket.elementId = byteOne;
    newPacket.elementName = elementName;
    newPacket.timestamp = timestamp;
    
    ofNotifyEvent(midiEvent, newPacket, this);
    
//        cout << "midi packet: port ["<<port<<"], channel ["<<channel<<"], status ["<<status<<"], byteOne ["<<byteOne<<"], byte2 ["<<byteTwo<<"], timestamp ["<<timestamp<<"]\n";

}

void OP1::newVirtualMessageEvent (ofxMidiEventArgs & args){
    
    int port = args.port;
	int	channel = args.channel;
	int status = args.status;
	int byteOne = args.byteOne;
	int byteTwo = args.byteTwo;
	double 	timestamp = args.timestamp;
    
    string event;
    string elementName;
    
    int keyId = byteOne;
    
//    keyEvent(byteOne, status==144, elementName); //keyid keydown
    event = (status==144)?"key_down":"key_up";
    
    if (event=="key_down"){
        sendNoteOn(byteOne);
    }else{
        sendNoteOff(byteOne);
    }
    
    midiPacket newPacket;
    newPacket.channel = 0;
    newPacket.event = event;
    newPacket.elementId = byteOne;
    newPacket.elementName = elementName;
    newPacket.timestamp = timestamp;
    
//    ofNotifyEvent(midiEvent, newPacket, this);
    
    cout << "virtual midi packet: port ["<<port<<"], channel ["<<channel<<"], status ["<<status<<"], byteOne ["<<byteOne<<"], byte2 ["<<byteTwo<<"], timestamp ["<<timestamp<<"]\n";

}


void OP1::sendNoteOn(int noteId){ //note is the absolute note id, key is remapped notes to fit the length of the keyboard
    midiOut.sendNoteOn(1, noteId, 144);
    string name;
    keyEvent(noteId, true, name);
}

void OP1::sendNoteOff(int noteId){
    midiOut.sendNoteOff(1, noteId, 128);
    string name;
    keyEvent(noteId, false, name);
}

void OP1::mouseDown(int x, int y){
    
//    cout << "mouse down at ("<<cursorX<<","<<cursorY<<")\n";
    handleKeystrokes();
}

void OP1::mouseUp(){
    
    sendNoteOff(currentNotePlaying);
    currentNotePlaying = -1;
}

void OP1::updateCursorPosition(int x, int y){ //global mouseX, mouseY is passed in
    
    cursorX = (x-posX)/scaleFactor+(284/2);
    cursorY = (y-posY)/scaleFactor+(102/2);
    
}

bool OP1::cursorInBoundingBox(float x, float y, float width, float height){
    
    if (cursorX>x && cursorX<x+width){
        if (cursorY>y && cursorY<y+height){
            ofSetHexColor(0x00ffff);
            return true;
        }
    }
    return false;
}

void OP1::handleKeystrokes(){
    
    int keyNumber = 0;
    
//white notes
    for (int i=0; i<14; i++){
        
            if (cursorInBoundingBox(50.5+i*15.5, 66, 14.5, 30)){
                currentNotePlaying = keyMap[keyNumber];
                sendNoteOn(currentNotePlaying);
            }
        
        keyNumber++;
    }
//black notes
    float xOffset = 0;
    
    for (int i=1; i<=10; i++){
        
        int keyWidth = (i==2||i==7)? 15.3:23.3;
        
        if (cursorInBoundingBox(50.5+xOffset, 50.5, keyWidth, 15.5)){
            currentNotePlaying = keyMap[keyNumber];
            sendNoteOn(currentNotePlaying);
        }
        
        if (i==2||i==7){
            xOffset += 15.3;
        }else{
            xOffset += 23.3;
        }
        
        keyNumber++;   
    }
}



void OP1::drawScreenBorder(){
    ofSetRectMode(OF_RECTMODE_CORNER);
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    ofSetHexColor(0x333333);
    roundedRect(getScreenWidth(), getScreenHeight(), 0.5);
    ofPopStyle();
    
//    cout << "drawOP1Screen() called"<<endl;
    drawOP1Screen();
    
}


void OP1::drawOP1Screen(){
	// draw the left:
    
    ofNoFill();
    ofBeginShape();
	ofSetHexColor(blue);
	for (int i = 0; i < 256; i++){
        float val = left[i]*20.0f;
        if (val>5){
            val = 5;
            ofSetHexColor(orange);
        }else if (val<-5){
            val = -5;
            ofSetHexColor(orange);
        }
		ofVertex(5+(float)i/5,10+val);
	}
    ofEndShape();
    
    ofSetHexColor(blue);
    ofBeginShape();
	for (int i = 0; i < 256; i++){
        float val = right[i]*20.0f;
        if (val>5){
            val = 5;
            ofSetHexColor(orange);
        }else if (val<-5){
            val = -5;
            ofSetHexColor(orange);
        }
        ofVertex(5+(float)i/5,20+val);
	}
    ofEndShape();
    
}

void OP1::audioReceived (float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"
	/*for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}*/
    
//    cout <<"audio recieved ["<<left[128]<<"]\n";
	
}




