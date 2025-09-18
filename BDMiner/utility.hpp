#pragma once

#include <vector>
#include <time.h>
#include <string>
#include "build_mdd.hpp"

using namespace std;

bool find_pnt(Arc* pnt, vector<Arc*>& vec, int pos);
int find_ID(vector<int>& vec, int itm);
float give_time(clock_t kk);
bool check_parent(int cur_arc, int str_pnt, unordered_set<int>& counted);


