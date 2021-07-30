#pragma once
#include "main.h"
int StartAnalysis(string  ConfigFilePath, string Source);
void StartTrain(string Src, vector<Mat>images, vector<int>labels, vector<string>name, Mat testImg);