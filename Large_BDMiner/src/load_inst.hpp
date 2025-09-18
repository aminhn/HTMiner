#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

bool Load_instance(string& items_file, double thresh);

extern string out_file, folder;

extern bool b_disp, b_write, use_dic, just_build, pre_pro, itmset_exists;

extern int M, L, time_limit;

extern unsigned long long int N, num_nodes, theta;

extern clock_t start_time; 
