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

class WorkspaceView{
private:
    ExecutionContext executionContext;
    
    vector<Point2f> getRectCorners(Rect);
public:
    WorkspaceView(ExecutionContext);
    
    void addFrame(Mat, Mat);
    
    vector<EvaluateRect> getNewRegions();
    
    vector<EvaluateRect> getAllRegions();
};
