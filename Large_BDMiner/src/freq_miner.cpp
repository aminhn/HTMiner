#include <iostream>
#include <time.h>
#include "freq_miner.hpp"
#include "build_mdd.hpp"
#include "utility.hpp"


void Out_patt(vector<int>& seq, unsigned long long int freq);
void Extend_patt(Pattern& _patt);

unsigned long long int num_patt = 0;
Pattern _patt;

vector<bool> ilist;
vector<bool> slist;

vector<int> DFS_numfound;

void Freq_miner() {

	vector<int> list;

	for (int i = 0; i < L; ++i) {
		if (DFS[i].freq >= theta) {
			list.push_back(-i-1);
			if (itmset_exists)
				list.push_back(i+1);
		}
	}

	for (int i = 0; i < DFS.size(); ++i) 
		DFS[i].list = list;
			
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
	
	slist = vector<bool>(L, 0);
	bool ilist_nempty = 0;

	if (itmset_exists) {
		ilist = vector<bool>(L, 0);
		for (vector<int>::iterator it = _patt.list.begin(); it != _patt.list.end(); ++it) {
			if (*it < 0)
				slist[-(*it) - 1] = 1;
			else {
				ilist[(*it) - 1] = 1;
				ilist_nempty = 1;
			}
		}
	}
	else {
		for (vector<int>::iterator it = _patt.list.begin(); it != _patt.list.end(); ++it)
			slist[-(*it) - 1] = 1;
	}

	int itmset_size = 1;
	int last_neg = _patt.seq.size() - 1;
	while (_patt.seq[last_neg] > 0) {
		--last_neg;
		++itmset_size;
	}
		
	vector<Pattern> pot_patt(L + L * ilist_nempty);		
	vector<unsigned long long int> DFS_patt_init;
	vector<unsigned long long int> DFS_patt;
	if (ilist_nempty)
		DFS_numfound.clear();
	vector<unsigned long long int> last_strpnt(L, 0);
	for (unsigned long long int pnt = 0; pnt < _patt.str_pnt.size(); ++pnt) {
		DFS_patt_init.push_back(_patt.str_pnt[pnt]);
		while(!DFS_patt_init.empty()) { 
			unsigned long long int cur_sibl = Tree[DFS_patt_init.back()].chld;
			DFS_patt_init.pop_back();
			while (cur_sibl != 0) {
				int cur_itm = Tree[cur_sibl].item;
				if (cur_itm < 0) {
					cur_itm = -cur_itm;
					if (slist[cur_itm - 1]) {
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0)
							pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);	
					}
					if (Tree[cur_sibl].chld != -1) {
						DFS_patt.push_back(cur_sibl);
						if (ilist_nempty) {
							if (cur_itm == -_patt.seq[last_neg])
								DFS_numfound.push_back(1);
							else
								DFS_numfound.push_back(0);
						}
					}
				}
				else {
					if (ilist[cur_itm - 1]) {
						pot_patt[cur_itm + L - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0)
							pot_patt[cur_itm + L - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != -1)
						DFS_patt_init.push_back(cur_sibl);
				}
				cur_sibl = Tree[cur_sibl].sibl;
			}
		}
		if (ilist_nempty) {
			for (int i = 0; i < L; ++i) {
				if (ilist[i])
					last_strpnt[i] = pot_patt[i + L].str_pnt.size();
			}
		}
		while(!DFS_patt.empty()) {
			unsigned long long int cur_sibl = Tree[DFS_patt.back()].chld;
			DFS_patt.pop_back();
			int num_found = 0;
			if (ilist_nempty) {
				num_found = DFS_numfound.back();
				DFS_numfound.pop_back();
			}
			while (cur_sibl != 0) {
				int cur_itm = Tree[cur_sibl].item;
				if (cur_itm > 0) {
					if (num_found == itmset_size && ilist[cur_itm - 1] && (Tree[Tree[cur_sibl].anct].itmset < Tree[_patt.str_pnt[pnt]].itmset || !check_parent(cur_sibl, _patt.str_pnt[pnt], last_strpnt[cur_itm - 1], pot_patt[cur_itm + L - 1].str_pnt))) {
						pot_patt[cur_itm + L - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0)
							pot_patt[cur_itm + L - 1].str_pnt.push_back(cur_sibl);
					}
					if (slist[cur_itm - 1] && Tree[Tree[cur_sibl].anct].itmset <= Tree[_patt.str_pnt[pnt]].itmset) {
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0)
							pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != -1) {
						DFS_patt.push_back(cur_sibl);
						if (ilist_nempty) {
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
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0)
							pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != -1) {
						DFS_patt.push_back(cur_sibl);
						if (ilist_nempty) {
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


	vector<int> ilistp;
	vector<int> slistp;
	for (vector<int>::iterator it = _patt.list.begin(); it != _patt.list.end(); ++it) {
		if (*it > 0 && pot_patt[(*it) + L - 1].freq >= theta) 
			ilistp.push_back(*it);
		else if (*it < 0 && pot_patt[-(*it) - 1].freq >= theta) {
			if (itmset_exists) 
				slistp.push_back(-(*it));
			ilistp.push_back(*it);
			slistp.push_back(*it);
		}
	} 

	for (vector<int>::iterator it = ilistp.begin(); it != ilistp.end(); ++it) {
		int p;
		if (*it < 0) 
			p = -(*it) - 1;
		else
			p = (*it) - 1 + L;
		DFS.emplace_back();
		swap(DFS.back(), pot_patt[p]);
		DFS.back().seq = _patt.seq;
		DFS.back().seq.push_back(*it);
		if (*it < 0)
			DFS.back().list = slistp;
		else
			DFS.back().list = ilistp;
		if (b_disp || b_write) 
			Out_patt(DFS.back().seq, DFS.back().freq);
		++num_patt;
	}
	
}


void Out_patt(vector<int>& seq, unsigned long long int freq) {

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





