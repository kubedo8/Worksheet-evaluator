//
//  feedback-generator.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include "execution-context.hpp"
#include "evaluator.hpp"

class FeedbackGenerator{
private:
    ExecutionContext executionContext;
    
    void drawLines(Mat, vector<Point2f>, Scalar);
    
    Scalar getColorForAnswerType(int);
public:
    FeedbackGenerator(ExecutionContext);
    
    void drawResult(Mat, vector<Point2f>, Evaluate*, Answer*);
};
