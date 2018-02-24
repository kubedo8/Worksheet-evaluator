//
//  marker-finder.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "marker-finder.hpp"

#include "opencv2/core.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"

MarkerFinder::MarkerFinder(ExecutionContext executionContext): executionContext(executionContext){
    MarkerFinder::executionContext = executionContext;
}

vector<MarkerRelatedPoints> MarkerFinder::findRelatedPoints(Mat input){
    Mat inputResized;
    double scale = input.rows / MAX_HEIGHT;
    resize(input, inputResized, Size(input.cols / scale, input.rows / scale));
    
    Ptr<SURF> detector = SURF::create();
    vector<KeyPoint> keyPoints;
    Mat descriptor;
    
    detector->detectAndCompute(inputResized, Mat(), keyPoints, descriptor);
    
    vector<MarkerRelatedPoints> markerPoints;
    
    vector<MarkerInfo> markersInfo = executionContext.getMarkersInfo();
    for (int i = 0; i < markersInfo.size(); i++) {
        MarkerInfo markerInfo = markersInfo[i];
        
        Mat homography = findHomographyForMarker(markerInfo, keyPoints, descriptor);
        
        if(homography.empty()){
            continue;
        }
        
        vector<RelatedPoint> points = computePoints(markerInfo.points, homography, scale);
        markerPoints.push_back(MarkerRelatedPoints(markerInfo.id, points));
    }
    
    return markerPoints;
}

Mat MarkerFinder::findHomographyForMarker(MarkerInfo markerInfo, vector<KeyPoint> keyPoints, Mat descriptor){
    FlannBasedMatcher matcher;
    vector< DMatch > matches;
    matcher.match( markerInfo.descriptor, descriptor, matches );
    double min_dist = 1e10;
    
    for( int i = 0; i < matches.size(); i++ ){
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
    }
    
    vector< DMatch > good_matches;
    for( int i = 0; i < matches.size(); i++ ){
        if( matches[i].distance <= 3 * min_dist ){
            good_matches.push_back( matches[i]);
        }
    }
    
    vector<Point2f> markerPts;
    vector<Point2f> inputMarkerPts;
    for( size_t i = 0; i < good_matches.size(); i++ ){
        //-- Get the keypoints from the good matches
        markerPts.push_back(markerInfo.keyPoints[ good_matches[i].queryIdx ].pt);
        inputMarkerPts.push_back(keyPoints[ good_matches[i].trainIdx ].pt);
    }
    return findHomography(markerPts, inputMarkerPts, RANSAC);
}

vector<RelatedPoint> MarkerFinder::computePoints(Point2f markerPoints[4], Mat homography, double scale){
    // TODO improvement: filter points - is valid square shape?
    
    RotatedRect rotatedRect(markerPoints[0], markerPoints[1], markerPoints[2]);
    Size size = rotatedRect.size;
    
    vector<RelatedPoint> points;
    
    vector<Point2f> markerCorners(4);
    markerCorners[0] = cvPoint(0, 0);
    markerCorners[1] = cvPoint(size.width, 0);
    markerCorners[2] = cvPoint(size.width, size.height);
    markerCorners[3] = cvPoint(0, size.height);
    vector<Point2f> inputMarkerCorners(4);
    perspectiveTransform(markerCorners, inputMarkerCorners, homography);
    
    points.push_back(RelatedPoint(markerPoints[0], inputMarkerCorners[0] * scale));
    points.push_back(RelatedPoint(markerPoints[1], inputMarkerCorners[1] * scale));
    points.push_back(RelatedPoint(markerPoints[2], inputMarkerCorners[2] * scale));
    points.push_back(RelatedPoint(markerPoints[3], inputMarkerCorners[3] * scale));
    
    return points;
}
