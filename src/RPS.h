//
//  RPS.h
//  RockPaperScissors
//
//  Created by Alejandro Santacoloma on 4/16/17.
//  Copyright © 2017 Alejandro Santacoloma. All rights reserved.
//

#ifndef RPS_h
#define RPS_h

void select(int cursor, Mat screen);

int menu(string filename,bool cursorexist);

void thresh(string basename, string handname);

int imgtest(String testhand, String userhand, bool righthand);

void Vertblock(Mat screen, int startx, int starty);

void Horiblock(Mat screen, int startx, int starty);

void screentext(Mat screen, int casetype);

void waitgo(Mat screen,int xstart,int ystart, bool go);

#endif /* RPS_h */
