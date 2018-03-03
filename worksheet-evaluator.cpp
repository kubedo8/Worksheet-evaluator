//
//  test-evaluator.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 17/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "worksheet-evaluator.hpp"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace std;

WorksheetEvaluator::WorksheetEvaluator(string modelPath): executionContext(modelPath), markerFinder(executionContext), aligner(executionContext), workspaceView(executionContext){
}

void WorksheetEvaluator::addFrame(Mat frame){
    vector<MarkerRelatedPoints> markerPoints = markerFinder.findRelatedPoints(frame);
    
    Mat homography = aligner.findImageHomography(markerPoints);
    
    if (homography.empty()){
        cerr << "Hmography not found on frame!" << endl;
        return; // TODO throw error?
    }
    
    vector<VisibleEvaluate> visibleEvalutes = workspaceView.addFrame(frame, homography);
}
