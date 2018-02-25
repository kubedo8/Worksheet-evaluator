//
//  execution-context.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 17/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "execution-context.hpp"

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

MarkerInfo::MarkerInfo(int id, Point2f points[4], vector<KeyPoint> keyPoints, Mat descriptor){
    MarkerInfo::id = id;
    for (int i = 0; i < 4; i++) {
        MarkerInfo::points[i] = points[i];
    }
    MarkerInfo::keyPoints = keyPoints;
    MarkerInfo::descriptor = descriptor;
}

RelatedPoint::RelatedPoint(Point2f p1, Point2f p2){
    RelatedPoint::original = p1;
    RelatedPoint::computed = p2;
}

MarkerRelatedPoints::MarkerRelatedPoints(int id, vector<RelatedPoint> points){
    MarkerRelatedPoints::markerId = id;
    MarkerRelatedPoints::points = points;
}

Evaluate::Evaluate(int id, Rect rect){
    Evaluate::id = id;
    Evaluate::rect = rect;
}

EvaluateNumber::EvaluateNumber(int id, Rect rect, int answer): Evaluate(id, rect){
    EvaluateNumber::answer = answer;
}

EvaluateRect::EvaluateRect(int evaluateId, Mat rectMatrix){
    EvaluateRect::evaluateId = evaluateId;
    EvaluateRect::rectMatrix = rectMatrix;
}

ExecutionContext::ExecutionContext(string modelPath){
    FileStorage fs(modelPath, FileStorage::READ);
    
    string imagePath = fs["imagePath"];
    worksheet = imread(imagePath);
    double scale = worksheet.rows / 1000.0;
    resize(worksheet, worksheet, Size(worksheet.cols / scale, worksheet.rows / scale));
    
    initMarkers(fs, scale);
    initEvaluateRects(fs, scale);
}

Mat ExecutionContext::cropWorksheetByPoints(Point2f points[4]){
    if (worksheet.empty()){
        return Mat();
    }
   
    RotatedRect rect(points[0], points[1], points[2]);
    
    // matrices we'll use
    Mat M, rotated, cropped;
    // get angle and size from the bounding box
    float angle = rect.angle;
    Size rect_size = rect.size;

    if (rect.angle < -45.) {
        angle += 90.0;
        swap(rect_size.width, rect_size.height);
    }
    // get the rotation matrix
    M = getRotationMatrix2D(rect.center, angle, 1.0);
    // perform the affine transformation
    warpAffine(worksheet, rotated, M, worksheet.size(), INTER_CUBIC);
    // crop the resulting image
    getRectSubPix(rotated, rect_size, rect.center, cropped);
    return cropped;
}

void ExecutionContext::initMarkers(FileStorage fs, double scale){
    FileNode markersNode = fs["markers"];
    
    Ptr<SURF> detector = SURF::create();
    for (FileNodeIterator markerIt = markersNode.begin(); markerIt != markersNode.end(); ++markerIt) {
        int id = (*markerIt)["id"];
        Point2f points[4];
        FileNodeIterator pointsIt = (*markerIt)["points"].begin();
        for (int i=0; i<4; i++) {
            vector<float> coords;
            (*pointsIt) >> coords;
            points[i].x = coords[0] / scale;
            points[i].y = coords[1] / scale;
            ++pointsIt;
        }
        
        Mat worksheetMarker = cropWorksheetByPoints(points);
        vector<KeyPoint> keyPoints;
        Mat descriptor;
        detector->detectAndCompute(worksheetMarker, Mat(), keyPoints, descriptor);
        
        markersInfo.push_back(MarkerInfo(id, points, keyPoints, descriptor));
    
    }
}

void ExecutionContext::initEvaluateRects(FileStorage fs, double scale){
    FileNode evalNode = fs["eval"];
    int id = 1;
    for (FileNodeIterator evalIt = evalNode.begin(); evalIt != evalNode.end(); ++evalIt) {
        int answer = (*evalIt)["answer"];
        vector<float> coords;
        (*evalIt)["pos"] >> coords;
        Rect rect;
        rect.x = coords[0] / scale;
        rect.y = coords[1] / scale;
        rect.width = (coords[2] / scale) - rect.x;
        rect.height = (coords[3] / scale) - rect.y;
        
        evaluateRects.push_back(EvaluateNumber(id, rect, answer));
        
        id++;
    }
}

Mat ExecutionContext::getWorksheet(){
    return worksheet;
}

vector<MarkerInfo> ExecutionContext::getMarkersInfo(){
    return markersInfo;
}

vector<Evaluate> ExecutionContext::gerEvaluateRects(){
    return evaluateRects;
}

