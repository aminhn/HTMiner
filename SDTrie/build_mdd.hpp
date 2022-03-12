#pragma once

#include<vector>
#include <cmath>
#include "load_inst.hpp"


void Build_MDD(vector<int>& items);
void Branch_tree();

class Arc {
public:

	int anct;
	int itmset;
	int freq = 0;
	int item;

	vector<int> chld;


	Arc(int _itm, int _itmset, int _anc) {
		itmset = _itmset;
		anct = _anc;
		item = _itm;
	}

	Arc(int _itm, int _anc) {
		item = _itm;
		anct = _anc;
	}


};


extern vector<Arc> Tree;
