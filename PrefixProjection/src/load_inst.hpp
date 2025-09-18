#pragma once

#include<vector>
#include<string>
#include <fstream>
#include <map>

using namespace std;

bool Load_instance(string &items_file, double thresh);

extern vector<vector<int>> items;

extern string out_file;

extern bool b_disp, b_write, use_dic, use_list, pre_pro;

extern unsigned int M, L, time_limit;

extern unsigned long long int N, theta;

extern clock_t start_time;

