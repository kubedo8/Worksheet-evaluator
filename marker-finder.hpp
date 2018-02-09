//
//  marker-finder.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "model.hpp"

using namespace std;

class MarkerFinder {
private:
    ImageInfo imageInfo;
    
    Mat findHomographyForMarker(MarkerInfo, vector<KeyPoint>, Mat);
    
    void computePoints(Region, Mat, vector<RelatedPoint>);
public:
    MarkerFinder(ImageInfo);
    vector<RelatedPoint> findRelatedPoints(Mat);
};
