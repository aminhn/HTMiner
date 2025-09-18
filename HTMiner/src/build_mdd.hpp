#pragma once

#include<vector>
#include <cmath>
#include "load_inst.hpp"

void Build_MDD(vector<int>& items, vector<int>& items_lim);

class Arc {
public:

	unsigned int chld;	
	unsigned int sibl;
	unsigned int freq;		
	unsigned int anct;
	int itmset;
	int item;

	Arc(unsigned int _itm, int _itmset, unsigned int _anc) {
		chld = 0;	
		sibl = 0;
		freq = 0;
		itmset = _itmset;
		anct = _anc;
		item = _itm;
	}

	Arc() {
		chld = 0;	
		sibl = 0;
		freq = 0;
	}

};

class VArc {
public:

	unsigned int sibl;	
	vector<int> seq;

	VArc(vector<int>& items, unsigned int _sib) {
		sibl = _sib;
		seq.swap(items);
	}
	
	VArc() {
		sibl = 0;
	}

};


class CArc {
public:

	vector<int> seq;
	vector<unsigned int> ancest;

	CArc(vector<unsigned int>& _anc, vector<int>& items) {
		ancest.swap(_anc);
		seq.swap(items);
	}
	
};

extern vector<Arc> Tree;
extern vector<VArc> VTree;
extern vector<CArc> CTree;
