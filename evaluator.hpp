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
    
    virtual string type() = 0;
    
    Answer();
};

class AnswerNumber: public Answer{
    public:
    long prediction;
    
    string type();
    
    AnswerNumber();
};

class Evaluator{
private:
    ExecutionContext executionContext;
    
    Ptr<SVM> svm;
    
    HOGDescriptor hog;
    
    vector<Answer*> answers;
    
    int digitSize;
    
    int digitMargin;
    
    Mat deskew(Mat);
    
    vector<Mat> splitImageByDigits(Mat);
    
    bool isValidDigitRect(Rect);
    
    Answer* getAnswerForId(int);
    
public:
    Evaluator(ExecutionContext);
    
    void predictNumber(int, Mat);
    
    vector<Answer*> getAnswers();
};
