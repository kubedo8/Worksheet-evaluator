//
//  feedback-generator.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "feedback-generator.hpp"

#include "opencv2/imgproc.hpp"

FeedbackGenerator::FeedbackGenerator(ExecutionContext executionContext): executionContext(executionContext){
    FeedbackGenerator::executionContext = executionContext;
}

Scalar FeedbackGenerator::getColorForAnswerType(int answerType){
    if(answerType == -1){
        return Scalar(255,0,0);
    }else if (answerType == 0){
        return Scalar(0,255,0);
    }else{
        return Scalar(0,0,255);
    }
}

void FeedbackGenerator::drawLines(Mat frame, vector<Point2f> points, Scalar color){
    for (int i = 0; i < points.size(); i++) {
        line(frame, points[i], points[(i + 1) % points.size()], color, 3);
    }
}

void FeedbackGenerator::drawResult(Mat frame, vector<Point2f> points, Evaluate evaluate, Answer answer){
    EvaluateNumber* evaluateNumber = static_cast<EvaluateNumber*>(&evaluate);
    AnswerNumber* answerNumber = static_cast<AnswerNumber*>(&answer);
    
    Scalar color;
    if(evaluateNumber != NULL && answerNumber != NULL){
        color = getColorForAnswerType(evaluateNumber->answer == answerNumber->prediction);
    }else{
        color = getColorForAnswerType(-1);
    }
    
    drawLines(frame, points, color);
}

