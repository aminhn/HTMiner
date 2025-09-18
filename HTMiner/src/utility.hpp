#pragma once

#include <vector>
#include <time.h>
#include <string>
#include "build_mdd.hpp"

using namespace std;

float give_time(clock_t kk);
bool check_parent(unsigned int cur_arc, unsigned int str_pnt, unsigned int start, vector<unsigned int>& strpnt_vec);


