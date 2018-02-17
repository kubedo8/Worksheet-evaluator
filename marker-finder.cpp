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

RelatedPoint::RelatedPoint(Point p1, Point p2){
    RelatedPoint::original = p1;
    RelatedPoint::computed = p2;
}

MarkerRelatedPoints::MarkerRelatedPoints(int id, vector<RelatedPoint> points){
    MarkerRelatedPoints::markerId = id;
    MarkerRelatedPoints::points = points;
}

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
    detector->detectAndCompute(input, Mat(), keyPoints, descriptor);
    
    FlannBasedMatcher matcher;
    
    vector<MarkerRelatedPoints> markerPoints;
    
    vector<MarkerInfo> markersInfo = executionContext.getMarkersInfo();
    for (int i = 0; i < markersInfo.size(); i++) {
        MarkerInfo markerInfo = markersInfo[i];
        
        Mat homography = findHomographyForMarker(markerInfo, keyPoints, descriptor);
        
        if(homography.empty()){
            continue;
        }
        
        vector<RelatedPoint> points = computePoints(markerInfo.region, homography);
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
    for( size_t i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        markerPts.push_back( markerInfo.keyPoints[ good_matches[i].queryIdx ].pt );
        inputMarkerPts.push_back( keyPoints[ good_matches[i].trainIdx ].pt );
    }
    return findHomography(markerPts, inputMarkerPts, RANSAC);
}

vector<RelatedPoint> MarkerFinder::computePoints(Region region, Mat homography){
    // TODO improvement: filter points - is valid square shape?
    
    vector<RelatedPoint> points;
    
    vector<Point> markerCorners(4);
    markerCorners[0] = cvPoint(0, 0);
    markerCorners[1] = cvPoint(region.width(), 0);
    markerCorners[2] = cvPoint(region.width(), region.height());
    markerCorners[3] = cvPoint(0, region.height());
    vector<Point> inputMarkerCorners(4);
    perspectiveTransform(markerCorners, inputMarkerCorners, homography);
    
    Point originalTopLeft = Point(region.x1, region.y1);
    points.push_back(RelatedPoint(originalTopLeft, inputMarkerCorners[0]));
    
    Point originalTopRight = Point(region.x2, region.y1);
    points.push_back(RelatedPoint(originalTopRight, inputMarkerCorners[1]));
    
    Point originalBottomRight = Point(region.x2, region.y2);
    points.push_back(RelatedPoint(originalBottomRight, inputMarkerCorners[2]));
    
    Point originalBottomLeft = Point(region.x1, region.y2);
    points.push_back(RelatedPoint(originalBottomLeft, inputMarkerCorners[3]));
    
    return points;
}
