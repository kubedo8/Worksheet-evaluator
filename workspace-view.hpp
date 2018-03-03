//
//  workspace-view.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include "execution-context.hpp"

#include <stdio.h>

using namespace std;

const double RATIO = 0.8;

class WorkspaceView{
private:
    ExecutionContext executionContext;
    
    vector<EvaluateRect> newRects;
    
    vector<EvaluateRect> allRects;
    
    vector<Point2f> getRectCorners(Rect);
    
    bool matContainsPoints(Mat, vector<Point2f>);
    
    void processRect(int, Mat);
    
    EvaluateRect* getRectById(int);
    
public:
    WorkspaceView(ExecutionContext);
    
    vector<VisibleEvaluate> addFrame(Mat, Mat);
    
    vector<EvaluateRect> getNewRects();
    
    vector<EvaluateRect> getAllRects();
};
