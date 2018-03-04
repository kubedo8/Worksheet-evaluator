//
//  evaluator.hpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#pragma once

#include "execution-context.hpp"

#include "opencv2/ml.hpp"
#include "opencv2/objdetect.hpp"
#include <stdio.h>

using namespace cv;
using namespace std;
using namespace cv::ml;

const int DIGIT_SIZE = 25;
const int DIGIT_MARGIN = 3;

class Evaluator{
private:
    ExecutionContext executionContext;
    
    Ptr<SVM> svm;
    
    HOGDescriptor hog;
    
    vector<Answer> answers;
    
    Mat deskew(Mat);
    
    vector<Mat> splitImageByDigits(Mat);
    
    bool isValidDigitRect(Rect);
    
    int getAnswerIndexForId(int);
    
public:
    Evaluator(ExecutionContext);
    
    void predictNumber(int, Mat);
    
    vector<Answer> getAnswers();
};
