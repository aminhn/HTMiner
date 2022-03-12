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

extern bool b_disp, b_write, use_dic, just_build, pre_pro;

extern int N, M, L, theta, num_nodes, M_mult, N_mult, time_limit;

extern clock_t start_time; 
