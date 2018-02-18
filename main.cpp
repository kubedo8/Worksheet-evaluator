//
//  main.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "worksheet-evaluator.hpp"

#include "opencv2/highgui.hpp"

#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    WorksheetEvaluator we("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/data/model101.yml");
    
    Mat frame = imread("/Users/jakub/Documents/thesis/Test evaluator/Test evaluator/data/image101.jpg");
    we.addFrame(frame);
    
}
