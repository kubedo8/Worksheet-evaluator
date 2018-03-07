//
//  evaluator.cpp
//  Test evaluator
//
//  Created by Jakub Rodak on 09/02/2018.
//  Copyright © 2018 Jakub Rodak. All rights reserved.
//

#include "evaluator.hpp"

#include <math.h>
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

Answer::Answer(int evaluateId){
    Answer::evaluateId = evaluateId;
}

AnswerNumber::AnswerNumber(int evaluateId, long long prediction): Answer(evaluateId){
    AnswerNumber::prediction = prediction;
}

Evaluator::Evaluator(ExecutionContext executionContext): executionContext(executionContext){
    Evaluator::executionContext = executionContext;
    DigitTrainData data = executionContext.loadDigitTrainData();
    Evaluator::svm = Algorithm::load<SVM>(data.trainPath);
    Evaluator::digitSize = data.digitSize;
    Evaluator::digitMargin = data.digitMargin;
    Evaluator::hog = HOGDescriptor(Size(digitSize, digitSize), //winSize
                                   Size(10,10), //blocksize
                                   Size(5,5), //blockStride,
                                   Size(10,10), //cellSize,
                                   9, //nbins,
                                   1, //derivAper,
                                   -1, //winSigma,
                                   0, //histogramNormType,
                                   0.2, //L2HysThresh,
                                   0,//gammal correction,
                                   64,//nlevels=64
                                   1);
}

Mat Evaluator::deskew(Mat image){
    Moments m = moments(image);
    if(abs(m.mu02) < 1e-2){
        // No deskewing needed.
        return image;
    }
    // Calculate skew based on central momemts.
    double skew = m.mu11/m.mu02;
    // Calculate affine transform to correct skewness.
    Mat warpMat = (Mat_<double>(2,3) << 1, skew, -0.5*digitSize*skew, 0, 1 , 0);
    
    Mat imgOut = Mat::zeros(image.rows, image.cols, image.type());
    warpAffine(image, imgOut, warpMat, imgOut.size(),WARP_INVERSE_MAP|INTER_LINEAR);
    
    return imgOut;
}

bool Evaluator::isValidDigitRect(Rect rect){
    return rect.width > 20 && rect.height > 20;
}

vector<Mat> Evaluator::splitImageByDigits(Mat image){
    vector<Mat> digits;
    
    Mat threshold;;
    adaptiveThreshold(image, threshold, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 81, 10);
    dilate(threshold, threshold, Mat(), Point(-1, -1), 2);
    
    vector<vector<Point> > contours;
    findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    vector<Rect> goodRects;
    for (int i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        if(isValidDigitRect(rect)){
            goodRects.push_back(rect);
        }
    }
    sort(goodRects.begin(), goodRects.end(), [this] (Rect &r1, Rect &r2){
        return r1.x < r2.x;
    });
    
    for (int i = 0; i < goodRects.size(); i++) {
        Rect rect = goodRects[i];
        int maxCoord = max(rect.width, rect.height);
        int plusRegion = (maxCoord / (digitSize - 2 * digitMargin)) * digitMargin; // plus region around digit
        maxCoord += 2 * plusRegion;
        
        Mat digit(maxCoord, maxCoord, threshold.type());
        digit.setTo(0);
        
        Mat originalDigitRoi = threshold(rect);
        originalDigitRoi.copyTo(digit(Rect((maxCoord - rect.width) / 2, (maxCoord - rect.height) / 2, rect.width, rect.height)));
        
        Mat digitScaled;
        resize(digit, digitScaled, Size(digitSize, digitSize));
        
        digits.push_back(deskew(digitScaled));
    }
    
    return digits;
}

void Evaluator::predictNumber(int evaluateId, Mat image){
    vector<Mat> split = splitImageByDigits(image);
    if(split.size() == 0) return;
    
    long long result = 0;
    long long base = (long long) pow(10,split.size() - 1);
    for(int i = 0; i < split.size(); i++){
        vector<float> descriptors;
        hog.compute(split[i],descriptors);
        
        Mat trainMat(1 ,(int) descriptors.size(), CV_32FC1);
        for(int j = 0;j<descriptors.size();j++){
            trainMat.at<float>(0,j) = descriptors[j];
        }
        int num =  (int) svm->predict(trainMat);
        result += base * num;
        
        base /= 10;
    }
    
    int answerIndex = getAnswerIndexForId(evaluateId);
    if(answerIndex == -1){
        answers.push_back(AnswerNumber(evaluateId, result));
    }else{
        AnswerNumber* answerNumber = static_cast<AnswerNumber*>(&answers[answerIndex]);
        answerNumber->prediction = result;
    }
}

int Evaluator::getAnswerIndexForId(int evaluateId){
    for (int i = 0; i < answers.size(); i++) {
        if(answers[i].evaluateId == evaluateId){
            return i;
        }
    }
    return -1;
}

vector<Answer> Evaluator::getAnswers(){
    return answers;
}
