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

const double MAX_HEIGHT = 800.0;

class RelatedPoint{
public:
    Point original;
    Point computed;
    
    RelatedPoint(Point, Point);
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
    
    vector<RelatedPoint> computePoints(Region, Mat, double);
public:
    MarkerFinder(ExecutionContext);
    vector<MarkerRelatedPoints> findRelatedPoints(Mat);
};
