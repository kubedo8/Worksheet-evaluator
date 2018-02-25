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
#include <iostream>

using namespace std;

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
        if (!points.empty()) {
            markerPoints.push_back(MarkerRelatedPoints(markerInfo.id, points));
        }
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
    for( int i = 0; i < good_matches.size(); i++ ){
        //-- Get the keypoints from the good matches
        markerPts.push_back(markerInfo.keyPoints[ good_matches[i].queryIdx ].pt);
        inputMarkerPts.push_back(keyPoints[ good_matches[i].trainIdx ].pt);
    }
    return findHomography(markerPts, inputMarkerPts, RANSAC);
}

vector<RelatedPoint> MarkerFinder::computePoints(Point2f markerPoints[4], Mat homography, double scale){
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
    
    for (int i = 0; i < 4; i++) {
        inputMarkerCorners[i] *= scale;
    }
    
    if (validRectangle(inputMarkerCorners)){
        points.push_back(RelatedPoint(markerPoints[0], inputMarkerCorners[0]));
        points.push_back(RelatedPoint(markerPoints[1], inputMarkerCorners[1]));
        points.push_back(RelatedPoint(markerPoints[2], inputMarkerCorners[2]));
        points.push_back(RelatedPoint(markerPoints[3], inputMarkerCorners[3]));
    }
    
    return points;
}

bool MarkerFinder::validRectangle(vector<Point2f> points) {
    if(points.size() != 4){
        return false;
    }
    double dist1 = norm(points[0] - points[1]);
    double dist2 = norm(points[1] - points[2]);
    double dist3 = norm(points[2] - points[3]);
    double dist4 = norm(points[3] - points[0]);
    
    if(dist1 < MIN_MARKER_SIZE || dist2 < MIN_MARKER_SIZE || dist3 < MIN_MARKER_SIZE || dist4 < MIN_MARKER_SIZE){
        return false;
    }
    
    float angle1 = calculateAngle(points[0], points[1], points[2]);
    float angle2 = calculateAngle(points[1], points[2], points[3]);
    float angle3 = calculateAngle(points[2], points[3], points[0]);
    float angle4 = calculateAngle(points[3], points[0], points[1]);
    
    if(angle1 < MIN_RECTANGLE_ANGLE || angle1 > MAX_RECTANGLE_ANGLE || angle2 < MIN_RECTANGLE_ANGLE || angle2 > MAX_RECTANGLE_ANGLE ||
       angle3 < MIN_RECTANGLE_ANGLE || angle3 > MAX_RECTANGLE_ANGLE || angle4 < MIN_RECTANGLE_ANGLE || angle4 > MAX_RECTANGLE_ANGLE){
        return false;
    }
    
    return true;
}

float MarkerFinder::calculateAngle(Point2f a, Point2f b, Point2f c){
    Point2f ab = { b.x - a.x, b.y - a.y };
    Point2f cb = { b.x - c.x, b.y - c.y };
    
    float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
    float cross = (ab.x * cb.y - ab.y * cb.x); // cross product
    
    float alpha = atan2(cross, dot);
    
    return abs(alpha * 180. / M_PI + 0.5);
}
