#include <iostream>
#include <time.h>
#include "freq_miner.hpp"
#include "build_mdd.hpp"
#include "utility.hpp"


void Out_patt(vector<int>& seq, int freq);
void Extend_patt(Pattern _patt);

int num_patt = 0;

void Freq_miner() {

	vector<int> islist;

	for (int i = 0; i < L; ++i) {
		if (DFS[i].freq >= theta) 
			islist.push_back(i);
	}

	for (int i = 0; i < DFS.size(); ++i) {
		DFS[i].ilist = islist;
		DFS[i].slist = islist;
	}
			
	while (!DFS.empty() && give_time(clock() - start_time) < time_limit) {
		if (DFS.back().freq >= theta)
			Extend_patt(DFS.back());
		else 
			DFS.pop_back();
	}


}

void Extend_patt(Pattern _patt) {

	DFS.pop_back();

	vector<bool> slist(L, 0);
	vector<bool> ilist(L, 0);
	
	for (vector<int>::iterator it = _patt.slist.begin(); it != _patt.slist.end(); ++it)
		slist[*it] = 1;
	for (vector<int>::iterator it = _patt.ilist.begin(); it != _patt.ilist.end(); ++it)
		ilist[*it] = 1;

	int itmset_size = 1;
	int last_neg = _patt.seq.size() - 1;
	while (_patt.seq[last_neg] > 0) {
		--last_neg;
		++itmset_size;
	}
		
	vector<Pattern> pot_patt(2 * L);		
	vector<int> DFS_patt_init;
	vector<int> DFS_patt;
	vector<int> DFS_numfound;
	vector<int> last_strpnt(L, 0);
	for (int pnt = 0; pnt < _patt.str_pnt.size(); ++pnt) {
		DFS_patt_init.push_back(_patt.str_pnt[pnt]);
		while(!DFS_patt_init.empty()) { 
			int cur_sibl = Tree[DFS_patt_init.back()].chld;
			DFS_patt_init.pop_back();
			while (cur_sibl != -1) {
				int cur_itm = Tree[cur_sibl].item;
				if (cur_itm < 0) {
					cur_itm = -cur_itm;
					if (slist[cur_itm - 1]) {
						pot_patt[cur_itm + L - 1].freq += Tree[cur_sibl].freq;
						pot_patt[cur_itm + L - 1].str_pnt.push_back(cur_sibl);	
					}
					if (Tree[cur_sibl].chld != -1) {
						DFS_patt.push_back(cur_sibl);
						if (!_patt.ilist.empty()) {
							if (cur_itm == -_patt.seq[last_neg])
								DFS_numfound.push_back(1);
							else
								DFS_numfound.push_back(0);
						}
					}
				}
				else {
					if (ilist[cur_itm - 1]) {
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != -1)
						DFS_patt_init.push_back(cur_sibl);
				}
				cur_sibl = Tree[cur_sibl].sibl;
			}
		}
		for (vector<int>::iterator it = _patt.ilist.begin(); it != _patt.ilist.end(); ++it)
			last_strpnt[*it] = pot_patt[*it].str_pnt.size() - 1;
		while(!DFS_patt.empty()) {
			int cur_sibl = Tree[DFS_patt.back()].chld;
			int num_found = DFS_numfound.back();
			DFS_patt.pop_back();
			DFS_numfound.pop_back();
			while (cur_sibl != -1) {
				int cur_itm = Tree[cur_sibl].item;
				if (cur_itm > 0) {
					if (num_found == itmset_size && ilist[cur_itm - 1] && !check_parent(cur_sibl, _patt.str_pnt[pnt], last_strpnt[cur_itm - 1], pot_patt[cur_itm - 1].str_pnt)) {
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);
					}
					if (slist[cur_itm - 1] && Tree[Tree[cur_sibl].anct].itmset <= Tree[_patt.str_pnt[pnt]].itmset) {
						pot_patt[cur_itm + L - 1].freq += Tree[cur_sibl].freq;
						pot_patt[cur_itm + L - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != -1) {
						DFS_patt.push_back(cur_sibl);
						if (!_patt.ilist.empty()) {
							if (num_found < itmset_size && cur_itm == abs(_patt.seq[last_neg + num_found])) 
								DFS_numfound.push_back(num_found + 1);
							else
								DFS_numfound.push_back(num_found);
						}
					}
				}
				else {
					cur_itm = -cur_itm;
					if (slist[cur_itm - 1] && Tree[Tree[cur_sibl].anct].itmset <= Tree[_patt.str_pnt[pnt]].itmset) {
						pot_patt[cur_itm + L - 1].freq += Tree[cur_sibl].freq;
						pot_patt[cur_itm + L - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != -1) {
						DFS_patt.push_back(cur_sibl);
						if (!_patt.ilist.empty()) {
							if (cur_itm == -_patt.seq[last_neg])
								DFS_numfound.push_back(1);
							else
								DFS_numfound.push_back(0);
						}
					}
				}
				cur_sibl = Tree[cur_sibl].sibl;
			}
		}
	}


	vector<int> slistp;
	vector<int> ilistp;

	for (vector<int>::iterator it = _patt.ilist.begin(); it != _patt.ilist.end(); ++it) {
		if (pot_patt[*it].freq >= theta) 
			ilistp.push_back(*it);
	}

	for (vector<int>::iterator it = _patt.slist.begin(); it != _patt.slist.end(); ++it) {
		if (pot_patt[(*it) + L].freq >= theta) 
			slistp.push_back(*it);
	}
	for (vector<int>::iterator it = ilistp.begin(); it != ilistp.end(); ++it) {
		pot_patt[*it].str_pnt.shrink_to_fit();
		DFS.push_back(pot_patt[*it]);
		DFS.back().seq = _patt.seq;
		DFS.back().seq.push_back((*it) + 1);
		DFS.back().seq.shrink_to_fit();
		DFS.back().slist = slistp;
		DFS.back().ilist = ilistp;
		if (b_disp || b_write) 
			Out_patt(DFS.back().seq, DFS.back().freq);
		++num_patt;
	}


	for (vector<int>::iterator it = slistp.begin(); it != slistp.end(); ++it) {
		pot_patt[(*it) + L].str_pnt.shrink_to_fit();
		DFS.push_back(pot_patt[(*it) + L]);
		DFS.back().seq = _patt.seq;
		DFS.back().seq.push_back(-(*it) - 1);
		DFS.back().seq.shrink_to_fit();
		DFS.back().slist = slistp;
		DFS.back().ilist = slistp;
		if (b_disp || b_write) 
			Out_patt(DFS.back().seq, DFS.back().freq);
		++num_patt;
	}


void Out_patt(vector<int>& seq, int freq) {

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





