//Magic Wand - This program looks for a green object, finds its center of mass and depending on its motion 
//finds if a gesture from a set of three has been used and uses it as a magic spell to attack a monster. 

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "recognitionFunctions.h"
#include "AreaRecognition.h"
#include "GestureManager.h"
#include "GameScreen.h"


using namespace std;



int main(int argc, char** argv)
{

const char* opencv_info= 0;
const char* plugin_info= 0;
cvGetModuleInfo(0, &opencv_info, &plugin_info);


	printf("Libraries: %s/nModules: %s/n", opencv_info, plugin_info);
	//useful set up variables
		int tolerance = 100;
		int imageSubtractionD = 2;

		//Set the size for the history to be kept for each gesture model
		int areaHistorySizeX = 4;
		int areaHistorySizeY = 4;
		int areaHistorySizeCircle = 5;
		
		//book keeping variables for mode selection
		int mode = 0;
		int spell = false;
		int counter = 0;
		int count1 = -1;
		int count2 = -1;

		//area information
		CvPoint currentCenter;
		int currentX = 0;
		int currentY = 0;

		//score info
		int life = 6;
		int score = 0;

		//Spell info
		Spell currentSpell = noSpell;

		//Creates a capture variable
		CvCapture* g_capture;

		//creates the window that will display the camera images
		// cvNamedWindow("mine", 0 );
		
		 //creates the window that shows the green filtered image
		//cvNamedWindow("green", CV_WINDOW_AUTOSIZE );

		//creates the window that shows the green filtered image
		cvNamedWindow("gameScreen", CV_WINDOW_AUTOSIZE );

		//Creates the game image variable pointer
		IplImage* playImage;
		
		//assignes the capture variable to a new camera capture (-1 is the default camera)
		g_capture = cvCreateCameraCapture(-1);


		//creates an image variable pointer called frame
		IplImage* frame;

		//takes an image from the camera and assigns it to frame. THis is done here first because the first few captures can simply be NULL
		frame = cvQueryFrame(g_capture);

		//an other image pointer we will use to turn the image into the green filtered image
		IplImage* filteredImage;

		//Declaration of a GameScreen instance
		GameScreen gameImage;

		//Create a font and initiate it
		CvFont scoreFont;
		cvInitFont(&scoreFont, CV_FONT_HERSHEY_DUPLEX, 1.0 , 1.0, 0, 2, 8);

		//This loop runs through the first 60 images first... then it observes the amount of green in two consecutive frames 
		//and decides on which detection mode to use, either a straight green filter or also including an image subtraction algorithm.
		while(mode == 0){

			//gets next frame
			frame = cvQueryFrame(g_capture);

			//flips image to make camera output a mirror image
			cvFlip(frame, NULL, 1);

			//calls the function that takes the image and leaves only the green colors
			filteredImage = GreenFilter(frame, 1);

			//cleans up noise
			cvErode(filteredImage, filteredImage, NULL, 5);

			if (!frame) break;
			
			//outputs image on to windows
			//cvShowImage("mine", frame );
			//cvShowImage("green", filteredImage);

			int pixelTotal = 0;
			
			//if we have used less than 60 frames simply add to counter but do nothing else
			if (counter < 60){
				counter++;
			}
			
			//if we are at over 60 frames but count1 is still -1 (which means no frames have been analized for the amount of green in the background
			else if( count1 == -1){
				
				//goes through each line in y
				for( int y=0; y<filteredImage->height; y++ ) {
				
					//Creates a pointer to the data at each y
					uchar* ptrImage = (uchar*) (
						filteredImage->imageData + y * filteredImage->widthStep);

					//goes through each memeber in that line of the matrix(each pixel) and if the value found there is > than zero adds 1 to counter
					for( int x=0; x < filteredImage->width; x++ ) {
						 if (ptrImage[x] > 0){
							pixelTotal++;
					
						 }
					 
					}
				}
				//set total to count1
				count1 = pixelTotal;
			}
			//if count1 was no longer -1 then the first frame after 60 was analized... time to analize the second
			else{

				//using the same process as above goes through each pixel value and if that value is greater than 0 adds it to a pixel total
				for( int y=0; y<filteredImage->height; y++ ) {
				
					uchar* ptrImage = (uchar*) (
						filteredImage->imageData + y * filteredImage->widthStep);

					for( int x=0; x < filteredImage->width; x++ ) {
						 if (ptrImage[x] > 0){
							pixelTotal++;
					
						 }
					 
					}
				}
				//set count2 to pixel total
				count2 = pixelTotal;

				//if the count of green pixels in each of the two frames is less than the value specified in the tolerance variable use mode 1(no background subtraction)
				//otherwise use mode 2 ( background subtraction)
				if (count1 < tolerance && count2 < tolerance)
					mode = 1;
				else
					mode = 2;
			}
		}

		cout <<"Mode: " << mode << "\n";

		//Sets up the areas according to screen size and is the object used to call the AreaRecognition functions
		AreaRecognition area(frame);
		
		//Creates a new gesture manager with the specified history sizes... history size for flick and point gestures is not necessary
		GestureManager gesture(areaHistorySizeX, areaHistorySizeY, areaHistorySizeCircle);
		
		// this contains the loop that continues to execute when mode 1 is used
	if(mode == 1){
		while(1){
			
			//get the next frame
			frame = cvQueryFrame(g_capture);

			//create a mirrored image
			cvFlip(frame, NULL, 1);

			//filter the image for green. the last variable is the mode. Mode 1 sets any value that is green to 255 in the image that is returned
			filteredImage = GreenFilter(frame, 1);

			//clean up the image - the last value specifies number of iterations for the clean up technique
			cvErode(filteredImage, filteredImage, NULL, 5);
			
			//using the cleaned up filtered image find the center of mass of all the green pixels in the image
			currentCenter = CenterofMass(filteredImage);

			

			//using the gesture object created above adds the next areas to the narrow x history que using a narrow x area model (4 subdivisions of x, 3 of y)
			gesture.NextAreasNX(area.AreasNarrowX(currentCenter));
			//using the gesture object created above adds the next areas to the narrow y history que using a narrow y area model (3 subdivisions of x, 4 of y)
			gesture.NextAreasNY(area.AreasNarrowY(currentCenter));
			//using the gesture object created above adds the next areas to the circle history que using a narrow x area model (4 subdivisions of x, 3 of y)
			gesture.NextAreasCircle(area.AreasNarrowX(currentCenter));

			//if no spell has been made
			if (!spell){
				//using the areas in the narrow x history que finds out if a horizontal gesture has been performed
				if(gesture.HorizontalGesture()){
					currentSpell = greenSpell;
					spell = true;
				}
				//using the areas in the narrow y history que finds out if a vertical gesture has been performed
				else if(gesture.VerticalGesture()){
					currentSpell = purpleSpell;
					spell = true;

				}
				//using the areas in the circle history finds out if a circle gesture has been performed
				else if(gesture.CircularGesture()){
					currentSpell = orangeSpell;
					spell = true;
				}
			}

			//if a spell has been made
			else{

				//using the current history finds if a point gesture has been performed
				if(gesture.PointGesture()){
					if(area.AreaWideX(currentCenter) == gameImage.monsterArea && currentSpell == gameImage.killSpell){
						score++;
						gameImage.CreateScene();
					}
					else{
						life--;
						if(life == 0){
							char n = cvWaitKey(1000);
							break;
						}
					}
					currentSpell = noSpell;
					spell = false;
				}

				//using the current history finds if a flick gesture has been performed
				else if (gesture.FlickGesture()){
					currentSpell = noSpell;
					spell = false;
				}
			}
			

			if (!frame) break;

			playImage = gameImage.showScene(currentCenter, currentSpell);
			
			/*//Draws a circle on top of the center of mass on each of the images that will be output on to windows
			cvCircle(filteredImage, currentCenter , 10, CV_RGB(10, 250, 100), -1, 8);
			cvCircle(frame, currentCenter, 10, CV_RGB(10, 250, 100), -1, 8);

			// show images
			cvShowImage("mine", frame );
			cvShowImage("green", filteredImage);*/


			char buffLife[100];
			sprintf( buffLife, "Life: %d", life );
			char buffScore[100];
			sprintf( buffScore, "Score: %d", score );
			

			cvCircle(playImage, currentCenter, 20, CV_RGB(255, 255, 255), -1, 4);
			cvPutText(playImage, buffLife,  cvPoint(180, 30), &scoreFont,  CV_RGB(255, 255, 255));
			cvPutText(playImage, buffScore,  cvPoint(330, 30), &scoreFont,  CV_RGB(255, 255, 255));

			if(life == 0){
					
					cvPutText(playImage, "You Lose!!",  cvPoint(180, 400), &scoreFont,  CV_RGB(255, 255, 255));
					cvShowImage("gameScreen", playImage);
					char n = cvWaitKey(5000);
					break;
			}

			cvShowImage("gameScreen", playImage);

			//release the image space in memory
			cvReleaseImage(&playImage);
			cvReleaseImage(&filteredImage);
			
			//if escape is pressed break loop
			char c = cvWaitKey(10);
			if(c==27) break;
		}
	}

	else{
		
		//book keeping variables for mode 2
		int backgroundRefresh = 0;
		CvPoint previousCenter = cvPoint(0 , 0);

		//creates two grayscale images
		IplImage* background  = cvCreateImage( cvGetSize(frame), 8, 1 );
		IplImage* result =  cvCreateImage( cvGetSize(frame), 8, 1 );

		while(1){
			//set frame to the current camera iamge and flip it for mirror image
			frame = cvQueryFrame(g_capture);
			cvFlip(frame, NULL, 1);

			//filter the image for green. Anything other than 1 returns a grayscale image with the original image's green values
			//as the values of those pixels that were green
			filteredImage = GreenFilter(frame, 2);

			//this if statememnt is used to decide how old the previous image used for comparison is. Because imageSubtractionD is 2 this happens every other frame
			if (backgroundRefresh % imageSubtractionD == 0 && frame){
				//get rid of the old image for comparison from memory
				cvReleaseImage(&background);
				//make a new grayscale image
				background  = cvCreateImage( cvGetSize(frame), 8, 1 );
				//copy the filtered image to the background image
				cvCopy(filteredImage, background, NULL);
				
			}
			backgroundRefresh++;

			//find the difference between each pixel in the background difference and the current filtered image and set it to result
			cvAbsDiff(background, filteredImage, result);

			//In result, any pixel with a value greater than 15 set to 255
			cvThreshold(result, result, 15, 255, CV_THRESH_BINARY);
			
			//Clean up. Last number dictantes how many times the clean up technique wil be iterated
			cvErode(result, result, NULL, 3);
			
			//find the center of mass of the resulting filtered background subtraction image
			currentCenter = CenterofMass(result);
			if (currentCenter.x == 0 && currentCenter.y == 0)
				currentCenter = previousCenter;
			else
				previousCenter = currentCenter;
			
			

			//As above, adds areas to the areas lists 
			gesture.NextAreasNX(area.AreasNarrowX(currentCenter));
			gesture.NextAreasNY(area.AreasNarrowY(currentCenter));
			gesture.NextAreasCircle(area.AreasNarrowX(currentCenter));
			
			//if no spell has been made
			if (!spell){
				//using the areas in the narrow x history que finds out if a horizontal gesture has been performed
				if(gesture.HorizontalGesture()){
					currentSpell = greenSpell;
					spell = true;
				}
				//using the areas in the narrow y history que finds out if a vertical gesture has been performed
				else if(gesture.VerticalGesture()){
					currentSpell = purpleSpell;
					spell = true;

				}
				//using the areas in the circle history finds out if a circle gesture has been performed
				else if(gesture.CircularGesture()){
					currentSpell = orangeSpell;
					spell = true;
				}
			}

			//if a spell has been made
			else{

				//using the current history finds if a point gesture has been performed
				if(gesture.PointGesture()){
					if(area.AreaWideX(currentCenter) == gameImage.monsterArea && currentSpell == gameImage.killSpell){
						score++;
						gameImage.CreateScene();
					}
					else{
						life--;
						
					}
					currentSpell = noSpell;
					spell = false;
				}

				//using the current history finds if a flick gesture has been performed
				else if (gesture.FlickGesture()){
					currentSpell = noSpell;
					spell = false;
				}
			}
			

			if (!frame) break;

			playImage = gameImage.showScene(currentCenter, currentSpell);

			/*//As above, adds circles to the images
			cvCircle(result, currentCenter, 10, CV_RGB(10, 250, 100), -1, 8);
			cvCircle(frame, currentCenter, 10, CV_RGB(10, 250, 100), -1, 8);
			
			//outputs images on to windows
			cvShowImage("mine", frame );
			cvShowImage("green", result );*/

			char buffLife[100];
			sprintf( buffLife, "Life: %d", life );
			char buffScore[100];
			sprintf( buffScore, "Score: %d", score );

			

			cvCircle(playImage, currentCenter, 20, CV_RGB(255, 255, 255), -1, 4);
			cvPutText(playImage, buffLife,  cvPoint(180, 30), &scoreFont,  CV_RGB(255, 255, 255));
			cvPutText(playImage, buffScore,  cvPoint(330, 30), &scoreFont,  CV_RGB(255, 255, 255));
			if(life == 0){
					
					cvPutText(playImage, "You Lose!!",  cvPoint(180, 400), &scoreFont,  CV_RGB(255, 255, 255));
					cvShowImage("gameScreen", playImage);
					char n = cvWaitKey(5000);
					break;
			}
			 cvShowImage("gameScreen", playImage);
			
			 cvReleaseImage(&playImage);
			cvReleaseImage(&filteredImage);
			
			
			char c = cvWaitKey(10);
			if(c==27) break;


		}
	

		//release captures and windows
		cvReleaseCapture(&g_capture);
		cvDestroyWindow("mine");
		cvDestroyWindow("green");
		cvDestroyWindow("gameScreen");


		return 0;
}
	}

