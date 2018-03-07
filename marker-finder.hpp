//
//  marker-finder.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include "execution-context.hpp"

#include <stdio.h>

using namespace std;

const double MAX_HEIGHT = 1000.0;
const double MIN_MARKER_SIZE = 50.0;
const double MIN_RECTANGLE_ANGLE = 75.0;
const double MAX_RECTANGLE_ANGLE = 105.0;

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

class MarkerFinder {
private:
    ExecutionContext executionContext;
    
    Mat findHomographyForMarker(MarkerInfo, vector<KeyPoint>, Mat);
    
    vector<RelatedPoint> computePoints(Point2f[4], Mat, double);
    
    bool validRectangle(vector<Point2f>);
    
    float calculateAngle(Point2f, Point2f, Point2f);
public:
    MarkerFinder(ExecutionContext);
    vector<MarkerRelatedPoints> findRelatedPoints(Mat);
};
