//
//  workspace-view.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "workspace-view.hpp"

WorkspaceView::WorkspaceView(ExecutionContext executionContext): executionContext(executionContext){
    WorkspaceView::executionContext = executionContext;
}

void WorkspaceView::addFrame(Mat frame, Mat homography){
    
}

vector<Point2f> WorkspaceView::getRectCorners(Rect rect){
    vector<Point2f> corners(4);
    corners[0] = cvPoint(rect.x, rect.y);
    corners[1] = cvPoint(rect.x + rect.width , rect.y);
    corners[2] = cvPoint(rect.x + rect.width, rect.y + rect.height);
    corners[3] = cvPoint(rect.x , rect.y + rect.height);
    
    return corners;
}

//vector<EvaluateRect> WorkspaceView::getNewRegions(){
//    vector<EvaluateRect> newRegions(0);
//    return newRegions;
//}
//
//vector<EvaluateRect> WorkspaceView::getAllRegions(){
//    vector<EvaluateRect> newRegions(0);
//    return newRegions;
//}





//vector<Point2f> worksheetCorners = computeCorners(executionContext.getWorksheet());
//vector<Point2f> frameCorners(4);
//perspectiveTransform(worksheetCorners, frameCorners, homography);
//
//double ratio = (executionContext.getWorksheet().rows * 1.0) / (executionContext.getWorksheet().cols * 1.0);
//Size cropSize(frame.rows / ratio , frame.rows);
//Mat cropped(cropSize, frame.type());
//
//vector<Point2f> corners = computeCorners(cropped);
//
//Mat perspectiveTransform = getPerspectiveTransform(frameCorners, corners);
//warpPerspective(frame, cropped, perspectiveTransform, cropSize);
//
//return cropped;

