#pragma once

#include <vector>
#include <time.h>
#include <string>
#include "build_mdd.hpp"

using namespace std;

float give_time(clock_t kk);
bool check_parent(unsigned long long int cur_arc, unsigned long long int str_pnt, unsigned long long int start, vector<unsigned long long int>& strpnt_vec);

