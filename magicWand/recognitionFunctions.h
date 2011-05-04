#pragma once

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <iostream>
#include <string>




CvPoint CenterofMass(IplImage* image);
IplImage* GreenFilter (IplImage* image, int greenFlag);

