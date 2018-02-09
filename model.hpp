//
//  model.h
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include <string.h>
#include "opencv2/core.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

class RelatedPoint{
public:
    Point original;
    Point computed;
    
    RelatedPoint(Point, Point);
};

class Region {
public:
    float x1;
    float y1;
    float x2;
    float y2;
    
    float width();
    float height();
    
    Region(float, float, float, float);
};

class MarkerInfo {
public:
    Region region;
    vector<KeyPoint> keyPoints;
    Mat descriptor;
    
    MarkerInfo(Region, vector<KeyPoint>, Mat);
};

class Evaluate{
public:
    Region region;
    
    Evaluate(Region);
};

class EvaluateNumber: public Evaluate {
public:
    int answer;
    
    EvaluateNumber(Region, int);
};

class ImageInfo {
public:
    vector<MarkerInfo> markersInfo;
    Size original;
    Size resized;
    
    float scale();
    
    ImageInfo(vector<MarkerInfo>, Size, Size);
};
