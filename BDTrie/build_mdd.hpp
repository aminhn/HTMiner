#pragma once

#include<vector>
#include <cmath>
#include "load_inst.hpp"


void Build_MDD(vector<int>& items);

class Arc {
public:

	int chld = -1;	
	int sibl = -1;
	int freq = 0;		
	int anct;
	int itmset;
	int item;

	Arc(int _itm, int _itmset, int _anc) {
		itmset = _itmset;
		anct = _anc;
		item = _itm;
	}

	Arc(int _itm, int _anc) {
		item = _itm;
		anct = _anc;
	}

	Arc() {
		chld = -1;	
		sibl = -1;
		freq = 0;
	}


};

extern vector<Arc> Tree;
