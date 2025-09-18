#pragma once

#include "load_inst.hpp"
#include "build_mdd.hpp"

void Freq_miner();

class Pattern {
public:

	vector<int> seq;
	vector<unsigned int> str_pnt;
	vector<int> list;

	unsigned long long int freq;

	Pattern(int item) {
		seq.push_back(item);
		freq = 0;
	}

	Pattern(size_t _pnt, bool _res) {
		str_pnt.reserve(_pnt);
		freq = 0;
	}

	Pattern() {
		freq = 0;
	}


};

class VPattern {
public:

	unsigned long long int ass_patt;
	
	vector<int> str_pnt;
	vector<unsigned int> seq_ID;

	VPattern(unsigned long long int _patt) {
		ass_patt = _patt;
	}

	VPattern(size_t _pnt, bool a) {
		str_pnt.reserve(_pnt);
	}

	VPattern() {}
};


extern unsigned long long int num_patt;
extern vector<Pattern> DFS;
extern vector<VPattern> VDFS;



