//
//  main.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "worksheet-evaluator.hpp"

#include "opencv2/highgui.hpp"

#include <stdio.h>
#include <iostream>

using namespace std;

void processVideo(WorksheetEvaluator we){
    VideoCapture vc("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/debug/video.mp4");
    
    double dWidth = vc.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = vc.get(CV_CAP_PROP_FRAME_HEIGHT);
    int fps = vc.get(CV_CAP_PROP_FPS);
    int ex = static_cast<int>(vc.get(CV_CAP_PROP_FOURCC));
    
    VideoWriter vw("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/debug/video_result.mp4", ex, fps, Size(dHeight, dWidth), true);
    
    while (true) {
        Mat frame;
        vc >> frame;
        if(!frame.data) return;
        
        rotate(frame, frame, ROTATE_90_CLOCKWISE);
        
        we.addFrame(frame);
        
        vw.write(frame);
        
        imshow("image", frame);
        if (waitKey(10) == 27){
            break;
        }
    }
    
    vw.release();
}

void processImage(WorksheetEvaluator we){
    Mat frame = imread("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/data/image101.jpg");
    we.addFrame(frame);
    
    imwrite("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/debug/corners.jpg", frame);
}

int main(int argc, const char * argv[]) {
    WorksheetEvaluator we("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/data/model101.yml");
    
    //processVideo(we);
    processImage(we);
}
