#pragma once

#include<vector>
#include <cmath>
#include "load_inst.hpp"


void Build_MDD(vector<int>& items);

class Arc {
public:

	unsigned long long int chld;	
	unsigned long long int sibl;
	unsigned long long int freq;		
	unsigned long long int anct;
	int itmset;
	int item;

	Arc(int _itm, int _itmset, unsigned long long int _anc) {
		itmset = _itmset;
		anct = _anc;
		item = _itm;
		freq = 0;
		chld = 0;
		sibl = 0;
	}

	Arc(int _itm, int _anc) {
		item = _itm;
		anct = _anc;
		freq = 0;
		chld = 0;
		sibl = 0;
	}

	Arc() {
		freq = 0;
		chld = 0;
		sibl = 0;
	}


};

extern vector<Arc> Tree;
