//
//  workspace-view.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "workspace-view.hpp"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;

WorkspaceView::WorkspaceView(ExecutionContext executionContext): executionContext(executionContext){
    WorkspaceView::executionContext = executionContext;
}

vector<VisibleEvaluate> WorkspaceView::addFrame(Mat frame, Mat homography){
    vector<Evaluate> evaluateRects = executionContext.gerEvaluateRects();
    double scale = max(1.0, (frame.rows * 1.0) / 2.0 / (executionContext.getWorksheet().rows * 1.0));
    
    vector<VisibleEvaluate> visibleEvaluates;
    
    for (int i = 0; i < evaluateRects.size(); i++) {
        Rect rect = evaluateRects[i].rect;
        
        vector<Point2f> rectCorners = getRectCorners(rect);
        vector<Point2f> computedCorners(4);
        
        perspectiveTransform(rectCorners, computedCorners, homography);
        
        if (!matContainsPoints(frame, computedCorners)) {
            continue;
        }
        
        visibleEvaluates.push_back(VisibleEvaluate(evaluateRects[i].id, computedCorners));
        
        Size2f cropSize = Size2f(rect.width * scale, rect.height * scale);
        
        vector<Point2f> corners(4);
        corners[0] = Point2f(0,0);
        corners[1] = Point2f(cropSize.width, 0);
        corners[2] = Point2f(cropSize.width, cropSize.height);
        corners[3] = Point2f(0, cropSize.height);
        
        Mat perspectiveTransform = getPerspectiveTransform(computedCorners, corners);
        
        Mat cropped;
        warpPerspective(frame, cropped, perspectiveTransform, cropSize);
        
        processRect(evaluateRects[i].id, cropped);
    }
    
    return visibleEvaluates;
}

void WorkspaceView::processRect(int id, Mat rect){
    Mat gray;
    cvtColor(rect, gray, CV_RGB2GRAY);
    
    EvaluateRect* evaluateRect = getRectById(id);
    
    if (evaluateRect != NULL){
        Mat currentMatrix = evaluateRect->rectMatrix;
        Mat newMatrix = RATIO * currentMatrix + (1 - RATIO) * gray;
        evaluateRect->rectMatrix = newMatrix;
    }else{
        EvaluateRect newRect(id, gray);
        allRects.push_back(newRect);
        newRects.push_back(newRect);
    }
}

EvaluateRect* WorkspaceView::getRectById(int id){
    for (int i = 0; i < allRects.size(); i++) {
        if (allRects[i].evaluateId == id) {
            return &allRects[i];
        }
    }
    return NULL;
}

bool WorkspaceView::matContainsPoints(Mat mat, vector<Point2f> points){
    int width = mat.size().width;
    int height = mat.size().height;
    
    for (int i = 0; i < points.size(); i++) {
        Point2f point = points[i];
        if (point.x > width || point.y > height) {
            return false;
        }
    }
    return true;
}

vector<Point2f> WorkspaceView::getRectCorners(Rect rect){
    vector<Point2f> corners(4);
    corners[0] = cvPoint(rect.x, rect.y);
    corners[1] = cvPoint(rect.x + rect.width , rect.y);
    corners[2] = cvPoint(rect.x + rect.width, rect.y + rect.height);
    corners[3] = cvPoint(rect.x , rect.y + rect.height);
    
    return corners;
}

vector<EvaluateRect> WorkspaceView::getNewRects(){
    vector<EvaluateRect> toReturn = newRects;
    newRects.clear();
    return toReturn;
}

vector<EvaluateRect> WorkspaceView::getAllRects(){
    return allRects;
}
