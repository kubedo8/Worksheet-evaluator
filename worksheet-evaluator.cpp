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

WorksheetEvaluator::WorksheetEvaluator(string imagePath, string modelPath): executionContext(imagePath, modelPath), markerFinder(executionContext), aligner(executionContext), workspaceView(executionContext), evaluator(executionContext), feedbackGenerator(executionContext){
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
        Evaluate* evaluate = getEvaluateForId(newRects[i].evaluateId);
        if(dynamic_cast<EvaluateNumber*>(evaluate)){
            evaluator.predictNumber(evaluate->evaluateId, newRects[i].rectMatrix);
        }
    }
}

void WorksheetEvaluator::drawResults(Mat frame, vector<VisibleEvaluate> visibles){
    for (int i = 0; i < visibles.size(); i++) {
        VisibleEvaluate visible = visibles[i];
        Evaluate* evaluate = getEvaluateForId(visible.evaluateId);
        Answer* answer = getAnswerForId(visible.evaluateId);
        if(evaluate != NULL && answer != NULL){
            feedbackGenerator.drawResult(frame, visible.points, evaluate, answer);
        }
    }
}

Answer* WorksheetEvaluator::getAnswerForId(int id){
    vector<Answer*> answers = evaluator.getAnswers();
    for (int i = 0; i < answers.size(); i++) {
        if (answers[i]->evaluateId == id){
            return answers[i];
        }
    }
    return NULL;
}

Evaluate* WorksheetEvaluator::getEvaluateForId(int id){
    vector<Evaluate*> evaluates = executionContext.gerEvaluateRects();
    for (int i = 0; i < evaluates.size(); i++) {
        if (evaluates[i]->evaluateId == id){
            return evaluates[i];
        }
    }
    return NULL;
}
