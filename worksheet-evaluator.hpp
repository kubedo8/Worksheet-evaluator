//
//  test-evaluator.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 17/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include "execution-context.hpp"
#include "marker-finder.hpp"

using namespace cv;


class WorksheetEvaluator{
private:
    ExecutionContext executionContext;
    MarkerFinder markerFinder;
public:
    WorksheetEvaluator(string);
    void addFrame(Mat frame);
};
