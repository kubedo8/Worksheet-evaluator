//
//  execution-context.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 17/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include <string.h>
#include "opencv2/core.hpp"
#include "opencv2/xfeatures2d.hpp"

#include <fstream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

class MarkerInfo {
public:
    int id;
    Point2f points[4];
    vector<KeyPoint> keyPoints;
    Mat descriptor;
    
    MarkerInfo(int, Point2f[4], vector<KeyPoint>, Mat);
};

class RelatedPoint{
public:
    Point2f original;
    Point2f computed;
    
    RelatedPoint(Point2f, Point2f);
};

class MarkerRelatedPoints{
public:
    int markerId;
    vector<RelatedPoint> points;
    
    MarkerRelatedPoints(int, vector<RelatedPoint>);
};

class Evaluate{
public:
    int id;
    Rect rect;
    
    Evaluate(int, Rect);
};

class EvaluateNumber: public Evaluate {
public:
    int answer;
    
    EvaluateNumber(int, Rect, int);
};

class EvaluateRect{
public:
    int evaluateId;
    Mat rectMatrix;
    
    EvaluateRect(int, Mat);
};

class VisibleEvaluate{
public:
    int evaluateId;
    vector<Point2f> points;
    
    VisibleEvaluate(int, vector<Point2f>);
};

class ExecutionContext{
private:
    Mat worksheet;
    vector<MarkerInfo> markersInfo;
    vector<Evaluate> evaluateRects;
    
    Mat cropWorksheetByPoints(Point2f[4]);
    void initMarkers(FileStorage, double);
    void initEvaluateRects(FileStorage, double);
public:
    ExecutionContext(string);
    Mat getWorksheet();
    vector<MarkerInfo> getMarkersInfo();
    vector<Evaluate> gerEvaluateRects();
};
