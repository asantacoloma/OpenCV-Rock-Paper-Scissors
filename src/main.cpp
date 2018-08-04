//Main program to run rockpaper scissor program using opencv files
//program utilizes image thresholding and image processing
//to identify hand signs thrown by user

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <array>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include "ron.h"
#include "RPS.h"

using namespace cv;
using namespace std;

//functions

//function to use a cursor in option selection menu
void select(int cursor, Mat screen)
{
    
    Vec3b marker = screen.at<Vec3b>(Point(0,0));//BGR color code
    marker[0] = 0;
    marker[1] = 0;
    marker[2] = 0;
    
    
    Vec3b color = screen.at<Vec3b>(Point(0,0));//BGR color code
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    
    //Initialization of start position for cursor
    int starty = 0;
    int startx = 0;
    
    if(cursor == 0)
    {
        starty = 330;
        startx = 90;
    }
    if(cursor == 1)
    {
        starty = 330;
        startx = 555;
    }
    
    int i = 10;
    //for loop to make cursor triangle
    for(int x = startx-20; x < startx  + 10; x++)
    {
        
        for(int y = starty-20 - i ; y < starty + 20 +i; y++)
        {
            screen.at<Vec3b>(Point(x,y)) = color;
            
        }
        i--;
    }
    //for loop to make cursor tail
    for(int x = startx - 45; x< startx -20; x++)
    {
        for (int y = starty - 10; y < starty +10; y++)
        {
            screen.at<Vec3b>(Point(x,y)) = color;
        }
    }
    
}

int menu(string filename,bool cursorexist)
{
    //valid filenames for this method: Menustart MenuCredits MenuFun MenuDone MenuLR MenuAgain
    
    bool done = false;
    int Uinput;
    Mat screen;
    
    //numeric value when key is pressed during waitKey()
    const int left = 2;
    const int right = 3;
    const int enter = 13;
    
    
    int cursor = 0;
    
    if (cursorexist == true)//menu images that require cursor use
    {
        while(done == false)//while loop runs until user selects a choice
        {
            screen = imread(filename);
            select(cursor,screen);
            imshow("Menu", screen);
            Uinput = waitKey(30);
            
            
            //if statement to move cursor left and right on menu
            if(Uinput == left && cursor != 0)
            {
                cursor = 0;
            }
            if(Uinput == right && cursor != 1)
            {
                cursor = 1;
            }
            if(Uinput == enter)
            {
                done = true;
            }
        }
    }
    else // menu images with no cursor option
    {
        //plays credits, group photo of team, and final image
        string credits[3] = {"MenuCredits.png","Group.png","MenuDone.png"};
        for(int i = 0; i < 3; i++)
        {
            screen = imread(credits[i]);
            imshow("Menu", screen);
            waitKey(3000);
        }
        return cursor;
    }
    return cursor;
}







//threshold function, generates binary images of photos taken by camera
void thresh(string basename, string handname)
{
    int rowsize = 200;
    int colsize = 200;
    Mat base = imread(basename);
    Mat hand = imread(handname);
    
    
    Mat binimg(200,200, CV_8UC3, Scalar(0,0,0));
    
    for(int row = 0; row < rowsize; row++)
    {
        for(int col = 0; col < colsize; col++)
        {
            //colors obtaind from image with user's hand
            Vec3b handcolor = 	hand.at<Vec3b>( col, row);
            uchar bluehand 	= 	handcolor.val[0];
            uchar greenhand = 	handcolor.val[1];
            uchar redhand 	= 	handcolor.val[2];
            
            //colors obtaind from image with
            Vec3b basecolor = 	base.at<Vec3b>( col, row);
            uchar bluebase 	= 	basecolor.val[0];
            uchar greenbase = 	basecolor.val[1];
            uchar redbase 	= 	basecolor.val[2];
            
            //Standard colors
            Vec3b WHITE = binimg.at<Vec3b>(Point(col,row));
            WHITE[0] = 255;
            WHITE[1] = 255;
            WHITE[2] = 255;
            
            Vec3b BLACK = binimg.at<Vec3b>(Point(col,row));
            BLACK[0] = 0;
            BLACK[1] = 0;
            BLACK[2] = 0;
            
            
            //Color difference of pixel
            
            int imgdiff1 = abs(bluehand - bluebase);
            int imgdiff2 = abs(greenhand - greenbase);
            int imgdiff3 = abs(redhand - redbase);
            
            
            int limit = 50; //threshold limit 50 was best during testing, test more limits later
            
            
            //check diff in pixel spot of image
            if( imgdiff1 > limit || imgdiff2 > limit || imgdiff3 > limit)
            {
                
                //makes pixel white if doesn't match bachground
                binimg.at<Vec3b>(Point(row,col)) = WHITE;
                
            }
            else
            {
                //makes pixel black if pixel matched background
                binimg.at<Vec3b>(Point(row,col)) = BLACK;
            }
            if(row == 199 && col == 199)
            {
                imwrite("binarytest.jpg", binimg);//save as binary image file
            }
            
        }
    }
    
    
}



//image compare function, compares photo taken of user's hand to test photos in computer memory

int imgtest(String testhand, String userhand, bool righthand)
{
    int rowsize = 200;
    int colsize = 200;
    
    Mat tester = imread(testhand);
    Mat user   = imread(userhand);
    
    if(righthand == false)
    {
        flip(user, user, 1);//flip user hand so it looks right handed for image compare if user used left hand
    }
    
    int diffcount = 0;//counter for if pixels did not match up during pixel
    
    //nested for loops to go pixel by pixel in two images and compare if they match
    //If pixels do not match, diffcount increases by 1.
    for(int row = 0; row < rowsize; row++)
    {
        for(int col = 0; col < colsize; col++)
        {
            Vec3b binimgcolor   = 	user.at<Vec3b>( col, row);
            uchar bluebinimg 	= 	binimgcolor.val[0];
            
            Vec3b testercolor   = 	tester.at<Vec3b>( col, row);
            uchar bluetester 	= 	testercolor.val[0];
            
            
            
            
            int imgdiff = abs(bluetester - bluebinimg);
            
            int limit   = 35; //threshold limit
            
            //check diff in pixel spot of image
            if( imgdiff > limit )
            {
                diffcount++;
            }
        }
    }
    cout << "pixel difference against "<< testhand << " is "<<diffcount<< " pixels\n";
    return diffcount;
}

// function to make simple block configuration that together make lcd characters
// each block is fits in a 50 by 50 pixel square, total character is 50 by 100 pixels
//vertical block
void Vertblock(Mat screen, int startx, int starty)
{
    Vec3b BLUE = screen.at<Vec3b>(Point(0,0));
    BLUE[0] = 255;
    BLUE[1] = 0;
    BLUE[2] = 0;
    
    int size = 50;
    int i = 5;
    //for loop to make top tip
    for (int y = starty; y < starty + 5; y++)
    {
        
        for(int x = startx + i; x < startx  + (size/5); x++)
        {
            screen.at<Vec3b>(Point(x,y)) = BLUE;
        }
        i--;
    }
    
    //for loop to make base
    for (int x = startx; x < startx  + (size/5); x++)
    {
        for(int y = starty + 5; y < starty + size - 5; y++)
        {
            screen.at<Vec3b>(Point(x,y)) = BLUE;
        }
    }
    
    //for loop to make bottom tip
    i = 5;
    
    for (int y = starty+size; y >= starty+size - 5; y--)
    {
        for(int x = startx + i; x < startx + (size/5); x++)
        {
            screen.at<Vec3b>(Point(x,y)) = BLUE;
        }
        i--;
    }
    
    
}

//horizontal block function
void Horiblock(Mat screen, int startx, int starty)
{
    
    Vec3b BLUE = screen.at<Vec3b>(Point(0,0));
    BLUE[0] = 255;
    BLUE[1] = 0;
    BLUE[2] = 0;
    
    int size = 50;
    
    //for loop to make left tip
    int i = 0;
    for (int x = startx ; x <= startx + 5; x++)
    {
        for(int y = starty; y < starty  + i; y++)
        {
            screen.at<Vec3b>(Point(x,y)) = BLUE;
        }
        i+=2;
    }
    
    
    
    // for loop to make base block
    for (int x = startx + 5; x < startx  + size - 5; x++)
    {
        for(int y = starty; y < starty + (size/5); y++)
        {
            screen.at<Vec3b>(Point(x,y)) = BLUE;
        }
    }
    
    //for loop to make right tip
    i = 0;
    for (int x = startx + size ; x >= startx+ size - 5; x--)
    {
        
        for(int y = starty; y < starty  + i; y++)
        {
            screen.at<Vec3b>(Point(x,y)) = BLUE;
        }
        i+=2;
    }
    
    
}

//text on screen function
void screentext(Mat screen, int casetype)
{
    Vec3b BLUE = screen.at<Vec3b>(Point(0,0));
    BLUE[0] = 255;
    BLUE[1] = 0;
    BLUE[2] = 0;
    
    //all characters are 30 by 30 pixels in size
    
    //timer numbers are in upper left part of screen (0,0) and are 50 by 50 pixels
    //100 by 100?
    
    
    if(casetype == 1)
    {
        //prints '1'
        Vertblock(screen, 40,  0);
        Vertblock(screen, 40, 50);
    }
    else if(casetype == 2)
    {
        //prints '2'
        Horiblock(screen,  0,   0);
        Horiblock(screen,  0,  50);
        Horiblock(screen,  0, 100);
        Vertblock(screen, 40,   0);
        Vertblock(screen,  0,  50);
    }
    else if(casetype == 3)
    {
        //prints '3'
        Horiblock(screen,  0,   0);
        Horiblock(screen,  0,  50);
        Horiblock(screen,  0, 100);
        Vertblock(screen, 40,   0);
        Vertblock(screen, 40,  50);
    }
    else if(casetype == 0)
    {
        //prints '0'
        Horiblock(screen,  0,   0);
        Horiblock(screen,  0, 100);
        Vertblock(screen, 40,   0);
        Vertblock(screen, 40,  50);
        Vertblock(screen, 0,   0);
        Vertblock(screen, 0,  50);
    }
    
    //end of function
}


//Function to draw a Red Square to let user know the timer has not started
void waitgo(Mat screen,int xstart,int ystart, bool go)
{
    Vec3b GREEN = screen.at<Vec3b>(Point(0,0));
    GREEN[0] = 0;
    GREEN[1] = 255;
    GREEN[2] = 0;
    
    Vec3b RED = screen.at<Vec3b>(Point(0,0));
    RED[0] = 0;
    RED[1] = 0;
    RED[2] = 255;
    
    if(go == true)
    {
        for(int x = xstart; x<= xstart + 100; x++)
        {
            for(int y = ystart; y <= ystart + 100; y++)
            {
                screen.at<Vec3b>(Point(x,y)) = GREEN;
            }
        }
    }
    else if(go != true)
    {
        //Nested for loop to draw a Red Square to let user know the timer has not started
        for(int x = xstart; x<= xstart + 100; x++)
        {
            for(int y = ystart; y <= ystart + 100; y++)
            {
                screen.at<Vec3b>(Point(x,y)) = RED;
            }
        }
    }
    
    
    
}

//                  _
//  _ __ ___   __ _(_)_ __
// | '_ ` _ \ / _` | | '_ \
// | | | | | | (_| | | | | |
// |_| |_| |_|\__,_|_|_| |_|


int main(int, char**)
{
    
    // Memory memo = *new Memory();
    
    
    bool righthand = false;
    
    
    
    //call on menu for game
    int choice;
    choice = menu("MenuStart.png",true);
    
    if (choice == 1)
    {
        menu("MenuCredits.png",false);
        return 0;
    }
    else
    {
        choice = menu("MenuLR.png",true);
        if(choice == 0)
        {
            righthand = false;
        }
        else
        {
            righthand = true;
        }
    }
    
    
    
    
    int Uinput = -1;
    
    Ron Ron;
    Mat img;
    int w = 800;
    
    Mat frame;
    Mat cropframe;
    
    
    
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID + apiID);
    // check if we succeeded
    
    
    
    int timer = 0;
    
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //int i = 0;
    
    //--- GRAB AND WRITE LOOP
    cout << "Live Video now running" << "\n";

    //for (;;)
    
    int ready = 0;
    
    
    //cout<<getBuildInformation();
    int errorcount = 0;
    
    for(int i = 0; i < 110; i++)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame); //frame is a 640 by 480 pixel image
        // check if we succeeded
        
        
        if(errorcount >9)
        {
            cout<<"error timeout";
            return -1;
        }
        
        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            errorcount++;
            cout<<errorcount<<endl;
            continue;
            //break;
        }
        flip(frame, frame, 1);//flip frame so it is not a mirror
        
        
        rectangle(frame,
                  Point( (w/4)-5, (w/4)-5 ),
                  Point( (w/2)+5, (w/2)+5),
                  Scalar( 255, 255 - i * 1.78, 255),
                  5,
                  LINE_8);
        // create a target in the image for user hand
        
        
        //if statements for timer
        // every 33 frames is one second
        // total loop is ~ 4.29 seconds
        if(i < 1 )
        {
            waitgo(frame,0,0, false);
        }
        else if(i >= 1 && i < 33)
        {
            screentext(frame,3);
        }
        else if(i >=33 && i < 66)
        {
            screentext(frame,2);
        }
        else if(i >=66 && i < 99)
        {
            screentext(frame,1);
        }
        else if(i >=99)
        {
            screentext(frame,0);
        }
        
        
        
        // if statement to save image of hand
        if(i == 105)
        {
            try
            {
                imwrite("imtest.jpg", frame);
                
                //Rect myROI(200, 200, 200, 200);
                Rect myROI(200, 200, 200, 200);
                
                cropframe = frame(myROI);
                
                imwrite("imtestcrop.jpg", cropframe);
                
                frame.deallocate();
                destroyAllWindows();
                //cap.release();
                
                break;
            }
            catch(...)
            {
                // cout << "An exception occured\n"; debug comment
                break;
            }
            
            
        }
        
        
        
        
        //window is originally 640 by 480 pixels before resized
        resize(frame, frame, cv::Size(frame.cols * 1.4,frame.rows * 1.4), 0, 0, CV_INTER_LINEAR);
        // show game screen and wait for a key with timeout long enough to show images
        imshow("GAME ON", frame);
        Uinput = waitKey(30);
        timer++;
        // cout<<Uinput<<"\n";
        
        
        
        //if statements for user
        if(Uinput == 32 && ready == 0) //user hit space key letting program know it is ready to start game
        {
            cout <<"getting base image\n";
            imwrite("imtestbase.jpg", frame);
            
            Rect myROI(200*1.4, 200*1.4, 200, 200);
            
            cropframe = frame(myROI);//cropped to 200 by 200 pixel image
            
            imwrite("imtestbasecrop.jpg", cropframe); //saves a base image of users shirt
            // to help detect hand in later image.
            
            ready = 1; //prevents user from restarting game after stating user was ready
        }
        
        // if user has not stated that he/she is ready, reset i in order to prevent loop from ending
        if(ready == 0)
        {
            i = -1;
        }
        
    }
    
    //image comparison starts
    string basefile = "imtestbasecrop.jpg";
    string handfile = "imtestcrop.jpg";
    
    thresh(basefile, handfile); //run image threshold of base image and hand thrown image
    
    
    
    
    
    //when mid is included, rock and mid get confused,
    //may need to make a finger counter and other feature finder functions
    
    //    int range = 19;
    //    string namelist [19] =
    //    {
    //
    //        "rock0.jpg",    "rock1.jpg",    "rock2.jpg",                               //0->2   case 0
    //        "paper0.jpg",   "paper1.jpg",   "paper2.jpg" , "paper3.jpg","paper4.jpg",  //3->7   case 1
    //        "scissor0.jpg", "scissor1.jpg", "scissor2.jpg",                            //8->10  case 2
    //        "blank.jpg",                                                               //11->14 case 3
    //        "high0.jpg",    "high1.jpg",    "high2.jpg",                               //15->17 case 4
    //        "mid0.jpg",     "mid1.jpg",     "mid2.jpg",   "mid3.jpg",                  //18->18 case 5
    //
    //    };
    
    
    
    //call test image function
    //array of strings with filenames in each index
    int range = 14;
    string namelist [14] = {
        "rock0.jpg",    "rock1.jpg",    "rock2.jpg",
        "paper0.jpg",   "paper1.jpg",   "paper2.jpg" ,  "paper3.jpg",  "paper4.jpg",
        "scissor0.jpg", "scissor1.jpg", "scissor2.jpg", "scissor3.jpg","scissor4.jpg",
        "blank.jpg"
    };
    
    
    int testtype = -1;
    int result = -1;
    int bestresult =10000000;
    string ufile = "binarytest.jpg";
    
    
    for(int ii = 0; ii < range; ii++)
    {
        string tfile = namelist[ii];
        result = imgtest(tfile, ufile,righthand);
        
        if(result<bestresult)
        {
            bestresult = result;
            testtype = ii;//stores which scenario is best
        }
        
    }
    
    
    //Certain is a variable to store what percentage the program is sure tha the user image matches a test image
    double certain = 100 - (bestresult/40000.0)*100;
    
    cout<<"best hand was "<< namelist[testtype] <<", I am "<<certain<< "% sure"<<"\n";
    
    
    string handoptions[6] = {"rock", "paper", "scissors","blank","high-five","flip off"};
    
    int userhand;
    
    if(testtype <=2)
    {
        userhand = 0;
    }
    else if(testtype <=7 && testtype>=3)
    {
        userhand = 1;
        
    }
    else if(testtype <=10 && testtype>=8)
    {
        userhand = 2;
        
    }
    else if(testtype <=14 && testtype>=11)
    {
        userhand = 3;
        
    }
    else if(testtype <=17 && testtype>=15)
    {
        userhand = 4;
        
    }
    else if(testtype >=18)
    {
        userhand = 5;
        
    }
    
    
    int uwin; // initialize uwin variable, stores outcomu
    
    
    srand(timer);
    int computerdecision = rand() % 3;
    
    
    cout << "The computer throws " << handoptions[computerdecision] << "\nUser threw "<< handoptions[userhand] << endl;
    
    
    
    
    
    //game logic summaraized
    
    // if unwin = 0; then user won
    // if unwin = 1; then tie
    // if unwin = 2; then user loss
    // if unwin = 3; then user flipped the bird
    // if unwin = 4; then user gave high five
    
    //game logic
    if(userhand == 0 && computerdecision == 2  )
    {
        uwin = 0;
    }
    else if(userhand == 1 && computerdecision == 0  )
    {
        uwin = 0;
    }
    else if(userhand == 2 && computerdecision == 1 )
    {
        uwin = 0;
    }
    else if(userhand == computerdecision)
    {
        uwin = 1;
    }
    else if(userhand == 3)
    {
        uwin = 3;
    }
    else if(userhand == 4)
    {
        uwin = 4;
    }
    else
    {
        uwin = 2;
    }
    
    //ron hand animation
    //ronshake(computerdecision);
    
    Ron.shakeHand(computerdecision);
    
    
    //ronmood animation
    
    Ron.mood(uwin);
    
    //ronmood(uwin);
    
    
    
    //    Mat testimage;
    //    testimage = imread("binarytest.jpg");
    //    imshow("this is only a test",testimage);
    //    waitKey(30);
    
    //game result logic
    if (uwin == 0)
    {
        cout<<"user wins!\n";
    }
    else if (uwin == 1)
    {
        cout<<"tie!\n";
    }
    else if (uwin == 2)
    {
        cout<<"computer wins!\n";
    }
    else if (uwin == 3)
    {
        cout<<"... Did you even try?\n";
        
    }
    else if (uwin == 4)
    {
        //randomly generate options to say for easter eggs
        cout<<"Stop trying to high five me and lets play dude\n";
        cout<<"If I wanted to be your friend I would have asked by now, stop high fiving me and lets play\n";
        cout<<"Not sure whether you just tried to badly cover the camera or if you tried to high five me...\n"<<"either way, stop it!";
    }
    else if (uwin == 5)
    {
        cout<<"... Did you just flip me off?\n";
        cout<<"not cool dude\n";
    }
    
    //call on menu functions
    menu("MenuFun.png", true);
    menu("MenuDone.png",false);
    
    
    
    // close windows and deinitialize
    try
    {
        frame.deallocate();
        destroyAllWindows();
        cap.release();
    }
    catch(...)
    {
        // cout << "An exception occured\n"; debug comment
    }
    
    return 0;
}


//                  _          __                   _
//    ___ _ __   __| |   ___  / _|  _ __ ___   __ _(_)_ __
//   / _ \ '_ \ / _` |  / _ \| |_  | '_ ` _ \ / _` | | '_ \
//  |  __/ | | | (_| | | (_) |  _| | | | | | | (_| | | | | |
//   \___|_| |_|\__,_|  \___/|_|   |_| |_| |_|\__,_|_|_| |_|
//

//___________________________________
//< check out our kickstarter for dlc >
//-----------------------------------
//  \   ^__^
//   \  (oo)\_______
//      (__)\       )\/\
//          ||----w |
//          ||     ||
