//
//  aligner.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "aligner.hpp"

#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;

Aligner::Aligner(ExecutionContext executionContext): executionContext(executionContext){
    Aligner::executionContext = executionContext;
}

Mat Aligner::findImageHomography(vector<MarkerRelatedPoints> markerPoints) {
    if (markerPoints.size() < 2) {
        return Mat();
    }
    vector<Point2f> worksheetPoints;
    vector<Point2f> framePoints;
    convertMarkerRelatedPointsToPoints(markerPoints, worksheetPoints, framePoints);
    
    return findHomography(worksheetPoints, framePoints, 0);
}

void Aligner::convertMarkerRelatedPointsToPoints(vector<MarkerRelatedPoints> markerPoints, vector<Point2f> &worksheetPoints, vector<Point2f> &framePoints){
    for (int i = 0; i < markerPoints.size(); i++) {
        vector<RelatedPoint> points = markerPoints[i].points;
        for (int j = 0; j < points.size(); j++) {
            worksheetPoints.push_back(points[j].original);
            framePoints.push_back(points[j].computed);
        }
    }
}
