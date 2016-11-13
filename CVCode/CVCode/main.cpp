//
//  main.cpp
//  CVCode
//
//  Created by Ryan Catoen on 11/12/16.
//  Copyright © 2016 Ryan Catoen. All rights reserved.
//

#include "main.hpp"

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

int main(){
    
    VideoCapture cap(0);
    
    while(true){
        Mat Webcam;
        cap.read(Webcam);
        imshow("Webcam", Webcam);
    }
    
}

