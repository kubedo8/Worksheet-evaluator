//
//  model.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include <stdio.h>
#include "model.hpp"

RelatedPoint::RelatedPoint(Point p1, Point p2){
    RelatedPoint::original = p1;
    RelatedPoint::computed = p2;
}

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

MarkerInfo::MarkerInfo(Region region, vector<KeyPoint> keyPoints, Mat descriptor) : region(region){
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

ImageInfo::ImageInfo(vector<MarkerInfo> markersInfo, Size original, Size resized) {
    ImageInfo::markersInfo = markersInfo;
    ImageInfo::original = original;
    ImageInfo::resized = resized;
}

float ImageInfo::scale(){
    return ImageInfo::original.height / ImageInfo::resized.height;
}
