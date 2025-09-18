#include <iostream>
#include <time.h>
#include "freq_miner.hpp"
#include "utility.hpp"

void Out_patt(vector<int>& seq, unsigned int freq);
void Extend_patt(Pattern& _patt);

unsigned long long int num_patt = 0;

Pattern _patt;

void Freq_miner() {

	vector<int> islist;

	if (use_list) {
		for (int i = 0; i < L; ++i) {
			if (DFS[i].freq >= theta) 
				islist.push_back(i);
		}

		for (int i = 0; i < DFS.size(); ++i) {
			DFS[i].ilist = islist;
			DFS[i].slist = islist;
		}
	}
	
	while (!DFS.empty() && give_time(clock() - start_time) < time_limit) {
		if (DFS.back().freq >= theta)
			Extend_patt(DFS.back());
		else 
			DFS.pop_back();
	}

}

void Extend_patt(Pattern& _pattern) {

	swap(_patt, _pattern);
	DFS.pop_back();

	vector<bool> slist;
	vector<bool> ilist;
	
	if (use_list) {
		slist = vector<bool>(L, 0);
		ilist = vector<bool>(L, 0);
		for (vector<int>::iterator it = _patt.slist.begin(); it != _patt.slist.end(); ++it)
			slist[*it] = 1;
		for (vector<int>::iterator it = _patt.ilist.begin(); it != _patt.ilist.end(); ++it)
			ilist[*it] = 1;
	}

	vector<Pattern> pot_patt(L * 2);

	int last_neg = _patt.seq.size() - 1;
	while (_patt.seq[last_neg] > 0)
		--last_neg;

	for (int i = 0; i < _patt.str_pnt.size(); ++i) {

		vector<bool> found(L * 2, 0);		

		unsigned int seq = _patt.seq_ID[i];				
		unsigned int j = _patt.str_pnt[i] + 1;
		while (j < items[seq].size() && items[seq][j] > 0) {
			int cur_itm = items[seq][j];
			if (!use_list || ilist[cur_itm - 1]) {
				pot_patt[cur_itm - 1].seq_ID.push_back(seq);
				pot_patt[cur_itm - 1].str_pnt.push_back(j);
				++pot_patt[cur_itm - 1].freq;
				found[cur_itm - 1] = 1;
			}
			++j;
		}
		
		int num_itmfnd = 0;
		for (int k = j; k < items[seq].size(); ++k) {
			int cur_itm = abs(items[seq][k]);
			if (items[seq][k] < 0) 
				num_itmfnd = 0;
			if ((!use_list || slist[cur_itm - 1]) && !found[L + cur_itm - 1]) {
				pot_patt[L + cur_itm - 1].seq_ID.push_back(seq);
				pot_patt[L + cur_itm - 1].str_pnt.push_back(k);
				++pot_patt[L + cur_itm - 1].freq;
				found[L + cur_itm - 1] = 1;
			}
			if (num_itmfnd == _patt.seq.size() - last_neg) {
				if ((!use_list || ilist[cur_itm - 1]) && !found[cur_itm - 1]) {
					pot_patt[cur_itm - 1].seq_ID.push_back(seq);
					pot_patt[cur_itm - 1].str_pnt.push_back(k);
					++pot_patt[cur_itm - 1].freq;
					found[cur_itm - 1] = 1;
				}
			}
			else if (cur_itm == abs(_patt.seq[last_neg + num_itmfnd])) 
				++num_itmfnd;
		}
	}


	vector<int> slistp;
	vector<int> ilistp;

	if (use_list) {
		for (vector<int>::iterator it = _patt.ilist.begin(); it != _patt.ilist.end(); ++it) {
			if (pot_patt[*it].freq >= theta) 
				ilistp.push_back(*it);
		}

		for (vector<int>::iterator it = _patt.slist.begin(); it != _patt.slist.end(); ++it) {
			if (pot_patt[(*it) + L].freq >= theta) 
				slistp.push_back(*it);
		}

		for (vector<int>::iterator it = ilistp.begin(); it != ilistp.end(); ++it) {
			DFS.emplace_back();
			swap(DFS.back(), pot_patt[*it]);
			DFS.back().seq = _patt.seq;
			DFS.back().seq.push_back((*it) + 1);
			DFS.back().slist = slistp;
			DFS.back().ilist = ilistp;
			if (b_disp || b_write) 
				Out_patt(DFS.back().seq, DFS.back().freq);
			++num_patt;
		}
	

		for (vector<int>::iterator it = slistp.begin(); it != slistp.end(); ++it) {
			DFS.emplace_back();
			swap(DFS.back(), pot_patt[(*it) + L]);
			DFS.back().seq = _patt.seq;
			DFS.back().seq.push_back(-(*it) - 1);
			DFS.back().slist = slistp;
			DFS.back().ilist = slistp;
			if (b_disp || b_write) 
				Out_patt(DFS.back().seq, DFS.back().freq);
			++num_patt;
		}
	}
	else {
		for (int i = 0; i < 2 * L; ++i) {
			if (pot_patt[i].freq >= theta) {
				DFS.emplace_back();
				swap(DFS.back(), pot_patt[i]);
				DFS.back().seq = _patt.seq;
				if (i >= L) {
					DFS.back().seq.push_back(-(i - L + 1));
				}
				else
					DFS.back().seq.push_back(i + 1);
				if (b_disp || b_write) 
					Out_patt(DFS.back().seq, DFS.back().freq);
				++num_patt;
			}
		}
	}

}


void Out_patt(vector<int>& seq, unsigned int freq) {

	ofstream file_o;
	if (b_write) 
		file_o.open(out_file, std::ios::app);

	for (int ii = 0; ii < seq.size(); ii++) {
		if (b_disp)
			cout << seq[ii] << " ";
		if (b_write) 
			file_o << seq[ii] << " ";
	}
	if (b_disp)
		cout << endl;
	if (b_write) 
		file_o << endl;
	
	if (b_disp)
		cout << "************** Freq: " << freq << endl;
	if (b_write) {
		file_o << "************** Freq: " << freq << endl;
		file_o.close();
	}
}





