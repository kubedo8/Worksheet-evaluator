//
//  execution-context.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 17/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "execution-context.hpp"

Region::Region(float x1, float y1, float x2, float y2){
    Region::x1 = x1;
    Region::y1 = y1;
    Region::x2 = x2;
    Region::y2 = y2;
}

float Region::width(){
    return Region::x2 - Region::x1;
}

float Region::height(){
    return Region::y2 - Region::y1;
}

MarkerInfo::MarkerInfo(int id, Region region, vector<KeyPoint> keyPoints, Mat descriptor) : region(region){
    MarkerInfo::id = id;
    MarkerInfo::region = region;
    MarkerInfo::keyPoints = keyPoints;
    MarkerInfo::descriptor = descriptor;
}

Evaluate::Evaluate(Region region) : region(region){
    Evaluate::region = region;
}

EvaluateNumber::EvaluateNumber(Region region, int answer): Evaluate(region){
    EvaluateNumber::answer = answer;
}

ExecutionContext::ExecutionContext(string modelPath){
    // TODO init everything
}

Mat ExecutionContext::getWorksheet(){
    return worksheet;
}

vector<MarkerInfo> ExecutionContext::getMarkersInfo(){
    return markersInfo;
}

