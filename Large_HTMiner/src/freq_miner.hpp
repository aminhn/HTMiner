#pragma once

#include "load_inst.hpp"
#include "build_mdd.hpp"

void Freq_miner();

class Pattern {
public:

	vector<int> seq;
	vector<unsigned long long int> str_pnt;
	vector<int> list;

	unsigned long long int freq;

	Pattern(vector<int>& _seq, int item) {
		seq.swap(_seq);
		seq.push_back(item);
		freq = 0;
	}

	Pattern(int item) {
		seq.push_back(item);
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
	vector<unsigned long long int> seq_ID;

	VPattern(unsigned long long int _patt) {
		ass_patt = _patt;
	}


	VPattern() {
		ass_patt = 0;
	}

};


extern unsigned long long int num_patt;
extern vector<Pattern> DFS;
extern vector<VPattern> VDFS;



