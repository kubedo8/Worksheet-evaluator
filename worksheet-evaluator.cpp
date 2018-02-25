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
    
    vector<Scalar> colors;
    colors.push_back(Scalar(255,0,0));
    colors.push_back(Scalar(0,255,0));
    colors.push_back(Scalar(0,0,255));
    colors.push_back(Scalar(100,20,80));
    
    for (int i = 0; i < markerPoints.size(); i++) {
        MarkerRelatedPoints mrp = markerPoints[i];
        cout << mrp.markerId << endl;
        
        Scalar color = colors[i];
        for (int j = 0; j < mrp.points.size(); j++) {
            RelatedPoint rp = mrp.points[j];
            cout << rp.original << " " << rp.computed << endl;
            circle(frame, rp.computed, 30, color);
        }
    }
    imwrite("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/debug/result.jpg", frame);
    
    Mat homography = aligner.findImageHomography(markerPoints);
    
    if (homography.empty()){
        cerr << "Hmography not found on frame!" << endl;
        return; // TODO throw error?
    }
    
    workspaceView.addFrame(frame, homography);
}
