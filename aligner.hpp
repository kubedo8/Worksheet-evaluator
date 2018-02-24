//
//  aligner.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include "execution-context.hpp"

#include <stdio.h>

using namespace std;

class Aligner{
private:
    ExecutionContext executionContext;
    
    void convertMarkerRelatedPointsToPoints(vector<MarkerRelatedPoints>, vector<Point2f>&, vector<Point2f>&);
    
public:
    Aligner(ExecutionContext);
    
    Mat findImageHomography(vector<MarkerRelatedPoints>);
};
