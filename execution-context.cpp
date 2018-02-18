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

Region::Region(float x1, float y1, float x2, float y2){
    Region::x1 = x1;
    Region::y1 = y1;
    Region::x2 = x2;
    Region::y2 = y2;
}

float Region::width(){
    return Region::x2 - Region::x1;
}

float Region::height(){
    return Region::y2 - Region::y1;
}

MarkerInfo::MarkerInfo(int id, Region region, vector<KeyPoint> keyPoints, Mat descriptor) : region(region){
    MarkerInfo::id = id;
    MarkerInfo::region = region;
    MarkerInfo::keyPoints = keyPoints;
    MarkerInfo::descriptor = descriptor;
}

Evaluate::Evaluate(Region region) : region(region){
    Evaluate::region = region;
}

EvaluateNumber::EvaluateNumber(Region region, int answer): Evaluate(region){
    EvaluateNumber::answer = answer;
}

ExecutionContext::ExecutionContext(string modelPath){
    FileStorage fs(modelPath, FileStorage::READ);
    
    string imagePath = fs["imagePath"];
    worksheet = imread(imagePath);
    double scale = worksheet.cols / 1000.0;
    resize(worksheet, worksheet, Size(worksheet.cols / scale, worksheet.rows / scale));
    
    initMarkers(fs, scale);
    initRegions(fs, scale);
}

Mat ExecutionContext::cropWorksheetByRegion(Region region){
    if (worksheet.empty()){
        return Mat();
    }
   
    Rect roi(region.x1 , region.y1, region.width(), region.height());
    Mat imgRoi = worksheet(roi);
    Mat cropped;
    imgRoi.copyTo(cropped);
    return cropped;
}

void ExecutionContext::initMarkers(FileStorage fs, double scale){
    FileNode markersNode = fs["markers"];
    
    Ptr<SURF> detector = SURF::create();
    int id = 1;
    for (FileNodeIterator markerIt = markersNode.begin(); markerIt != markersNode.end(); ++markerIt) {
        vector<int> coords;
        (*markerIt) >> coords;
        Region region = Region(coords[0]/ scale, coords[1]/ scale, coords[2]/ scale, coords[3]/ scale);
        
        Mat worksheetMarker = cropWorksheetByRegion(region);
        vector<KeyPoint> keyPoints;
        Mat descriptor;
        detector->detectAndCompute(worksheetMarker, Mat(), keyPoints, descriptor);
        
        markersInfo.push_back(MarkerInfo(id, region, keyPoints, descriptor));
        
        id++;
    }
}

void ExecutionContext::initRegions(FileStorage fs, double scale){
    FileNode evalNode = fs["eval"];
    for (FileNodeIterator evalIt = evalNode.begin(); evalIt != evalNode.end(); ++evalIt) {
        int answer = (*evalIt)["answer"];
        vector<int> coords;
        (*evalIt)["pos"] >> coords;
        Region region = Region(coords[0], coords[1], coords[2], coords[3]);
        
        evaluateRegions.push_back(EvaluateNumber(region, answer));
    }
}

Mat ExecutionContext::getWorksheet(){
    return worksheet;
}

vector<MarkerInfo> ExecutionContext::getMarkersInfo(){
    return markersInfo;
}

vector<EvaluateNumber> ExecutionContext::gerEvaluateRegions(){
    return evaluateRegions;
}

