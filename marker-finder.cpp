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

MarkerFinder::MarkerFinder(ImageInfo imageInfo): imageInfo(imageInfo){
    MarkerFinder::imageInfo = imageInfo;
}

vector<RelatedPoint> MarkerFinder::findRelatedPoints(Mat input){
    Mat inputResized;
    double scale = input.rows / imageInfo.resized.height;
    resize(input, inputResized, Size(input.cols / scale, input.rows / scale));
    
    Ptr<SURF> detector = SURF::create();
    vector<KeyPoint> keyPoints;
    Mat descriptor;
    detector->detectAndCompute(input, Mat(), keyPoints, descriptor);
    
    vector<RelatedPoint> points;
    FlannBasedMatcher matcher;
    
    for (int i = 0; i < imageInfo.markersInfo.size(); i++) {
        MarkerInfo markerInfo = imageInfo.markersInfo[i];
        
        Mat homography = findHomographyForMarker(markerInfo, keyPoints, descriptor);
        
        if(homography.empty()){
            continue;
        }
        
        computePoints(markerInfo.region, homography, points);
    }
    
    return points;
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

void computePoints(Region region, Mat homography, vector<RelatedPoint> &points){
    // TODO improvement: check if points is valid square shape
    
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
}

