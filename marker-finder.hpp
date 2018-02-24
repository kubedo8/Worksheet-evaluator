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

const double MAX_HEIGHT = 1500.0;

class MarkerFinder {
private:
    ExecutionContext executionContext;
    
    Mat findHomographyForMarker(MarkerInfo, vector<KeyPoint>, Mat);
    
    vector<RelatedPoint> computePoints(Point2f[4], Mat, double);
public:
    MarkerFinder(ExecutionContext);
    vector<MarkerRelatedPoints> findRelatedPoints(Mat);
};
