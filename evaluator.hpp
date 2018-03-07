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

using namespace cv;
using namespace cv::ml;

class Answer{
    public:
    int evaluateId;
    
    Answer(int);
};

class AnswerNumber: public Answer{
    public:
    long long prediction;
    
    AnswerNumber(int, long long);
};

class Evaluator{
private:
    ExecutionContext executionContext;
    
    Ptr<SVM> svm;
    
    HOGDescriptor hog;
    
    vector<Answer> answers;
    
    int digitSize;
    
    int digitMargin;
    
    Mat deskew(Mat);
    
    vector<Mat> splitImageByDigits(Mat);
    
    bool isValidDigitRect(Rect);
    
    int getAnswerIndexForId(int);
    
public:
    Evaluator(ExecutionContext);
    
    void predictNumber(int, Mat);
    
    vector<Answer> getAnswers();
};
