//
//  test-evaluator.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 17/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "worksheet-evaluator.hpp"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace std;

WorksheetEvaluator::WorksheetEvaluator(string modelPath): executionContext(modelPath), markerFinder(executionContext), aligner(executionContext), workspaceView(executionContext), evaluator(executionContext), feedbackGenerator(executionContext){
}

void WorksheetEvaluator::addFrame(Mat frame){
    Mat gray;
    cvtColor(frame, gray, COLOR_RGB2GRAY);
    
    vector<MarkerRelatedPoints> markerPoints = markerFinder.findRelatedPoints(gray);
    
    Mat homography = aligner.findImageHomography(markerPoints);
    
    if (homography.empty()){
        cerr << "Homography not found on frame!" << endl;
        return;
    }
    
    vector<VisibleEvaluate> visibleEvalutes = workspaceView.addFrame(gray, homography);
    
    evaluateNewRects();
    
    drawResults(frame, visibleEvalutes);
}

void WorksheetEvaluator::evaluateNewRects(){
    vector<EvaluateRect> newRects = workspaceView.getNewRects();
    for (int i = 0; i < newRects.size(); i++) {
        int evaluateIndex = getEvaluateIndexForId(newRects[i].evaluateId);
        if (evaluateIndex == -1){
            continue;
        }
        Evaluate evaluate = executionContext.gerEvaluateRects()[evaluateIndex];
        EvaluateNumber* evaluateNumber = static_cast<EvaluateNumber*>(&evaluate);
        if (evaluateNumber != NULL){
            evaluator.predictNumber(evaluateNumber->evaluateId, newRects[i].rectMatrix);
        }
    }
}
void WorksheetEvaluator::drawResults(Mat frame, vector<VisibleEvaluate> visibles){
    for (int i = 0; i < visibles.size(); i++) {
        VisibleEvaluate visible = visibles[i];
        int evaluateIndex = getEvaluateIndexForId(visible.evaluateId);
        int answerIndex = getAnswerIndexForId(visible.evaluateId);
        if(evaluateIndex != -1 && answerIndex != -1){
            feedbackGenerator.drawResult(frame, visible.points, executionContext.gerEvaluateRects()[evaluateIndex], evaluator.getAnswers()[answerIndex]);
        }
    }
}

int WorksheetEvaluator::getAnswerIndexForId(int id){
    vector<Answer> answers = evaluator.getAnswers();
    for (int i = 0; i < answers.size(); i++) {
        if (answers[i].evaluateId == id){
            return i;
        }
    }
    return -1;
}

int WorksheetEvaluator::getEvaluateIndexForId(int id){
    vector<Evaluate> evaluates = executionContext.gerEvaluateRects();
    for (int i = 0; i < evaluates.size(); i++) {
        if (evaluates[i].evaluateId == id){
            return i;
        }
    }
    return -1;
}
