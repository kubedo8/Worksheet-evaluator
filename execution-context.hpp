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

const string TRAIN_DIGITS_PATH = "/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/data/train_digits.yml";

class MarkerInfo {
public:
    int id;
    Point2f points[4];
    vector<KeyPoint> keyPoints;
    Mat descriptor;
    
    MarkerInfo(int, Point2f[4], vector<KeyPoint>, Mat);
};

class Evaluate{
public:
    int evaluateId;
    Rect rect;
    
    virtual string type() = 0;
    
    Evaluate();
};

class EvaluateNumber: public Evaluate {
public:
    long answer;
    
    string type();
    
    EvaluateNumber();
};

class DigitTrainData{
    public:
    string trainPath;
    int digitSize;
    int digitMargin;
    
    DigitTrainData(string, int, int);
};

class ExecutionContext{
private:
    Mat worksheet;
    vector<MarkerInfo> markersInfo;
    vector<Evaluate*> evaluateRects;
    string trainDigitsPath;
    
    Mat cropWorksheetByPoints(Point2f[4]);
    void initMarkers(FileStorage, double);
    void initEvaluateRects(FileStorage, double);
public:
    ExecutionContext(string, string);
    Mat getWorksheet();
    vector<MarkerInfo> getMarkersInfo();
    vector<Evaluate*> gerEvaluateRects();
    DigitTrainData loadDigitTrainData();
    
};
