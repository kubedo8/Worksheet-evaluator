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
#include "aligner.hpp"
#include "workspace-view.hpp"
#include "evaluator.hpp"
#include "feedback-generator.hpp"

using namespace cv;


class WorksheetEvaluator{
private:
    ExecutionContext executionContext;
    MarkerFinder markerFinder;
    Aligner aligner;
    WorkspaceView workspaceView;
    Evaluator evaluator;
    FeedbackGenerator feedbackGenerator;
    
    void evaluateNewRects();
    void drawResults(Mat, vector<VisibleEvaluate>);
    
    int getAnswerIndexForId(int);
    int getEvaluateIndexForId(int);
public:
    WorksheetEvaluator(string);
    void addFrame(Mat frame);
};
