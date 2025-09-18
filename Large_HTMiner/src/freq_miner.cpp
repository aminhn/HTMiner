#include <iostream>
#include <time.h>
#include "freq_miner.hpp"
#include "build_mdd.hpp"
#include "utility.hpp"


void Out_patt(vector<int>& seq, unsigned int freq);
void Extend_patt(Pattern& _patt);
void Mine_vec(unsigned long long int seq_ID, int pos, int num_found, vector<unsigned long long int>& ancest, vector<int>& items, unsigned long long int inod, int sgn);

unsigned long long int num_patt = 0;

vector<bool> ilist;
vector<bool> slist;

vector<Pattern> pot_patt;
vector<VPattern> pot_vpatt;
vector<unsigned long long int> last_strpnt;
vector<unsigned long long int> ancest_base;
vector<int> DFS_numfound;

Pattern _patt;
VPattern _vpatt;

int itmset_size;
int last_neg;

bool ilist_nempty;

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
		else {
			DFS.pop_back();
			if (!VDFS.empty() && VDFS.back().ass_patt == DFS.size())
				VDFS.pop_back();
		}
	}
}

void Extend_patt(Pattern& _pattern) {

	swap(_patt, _pattern);
	DFS.pop_back();

	slist = vector<bool>(L, 0);
	ilist_nempty = 0;

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
	
	last_neg = _patt.seq.size() - 1;
	while (_patt.seq[last_neg] > 0) 
		--last_neg;
	itmset_size = _patt.seq.size() - last_neg;
	
	pot_patt = vector<Pattern>(L + L * ilist_nempty);
	if (!CTree.empty()) 
		pot_vpatt = vector<VPattern>(L + L * ilist_nempty);

	last_strpnt = vector<unsigned long long int>(L, 0);

	if (!VDFS.empty() && VDFS.back().ass_patt == DFS.size()) {
		swap(_vpatt, VDFS.back());
		VDFS.pop_back();
		for (unsigned long long int pnt = 0; pnt < _vpatt.str_pnt.size(); ++pnt) {
			if (_vpatt.str_pnt[pnt] < 0)
				Mine_vec(_vpatt.seq_ID[pnt], -_vpatt.str_pnt[pnt], -1, ancest_base, CTree[_vpatt.seq_ID[pnt]].seq, 0, -1);	//starting search from vpatt should start from 1 position ahead of pointer
			else															//-1:no need to check ancest for remaining itemset items
				Mine_vec(_vpatt.seq_ID[pnt], _vpatt.str_pnt[pnt], -1, ancest_base, VTree[_vpatt.seq_ID[pnt]].seq, 0, 1);
		}
	} 

	vector<unsigned long long int> DFS_itm;			//for initial itemset extention
	vector<unsigned long long int> DFS_seq;			//for sequence extension
	if (ilist_nempty)
		DFS_numfound.clear();			//tracks whether the current itemset is found anywhere along the search path, by counting how many of the current items in the itemset are found
	for (unsigned long long int pnt = 0; pnt < _patt.str_pnt.size(); ++pnt) {
		DFS_itm.push_back(_patt.str_pnt[pnt]);
		while(!DFS_itm.empty()) { 
			unsigned long long int cur_sibl = DFS_itm.back();
			DFS_itm.pop_back();
			if (Tree[cur_sibl].itmset < 0) {
				unsigned long long int carc = Tree[cur_sibl].chld;
				Mine_vec(carc, 0, -1, CTree[carc].ancest, CTree[carc].seq, _patt.str_pnt[pnt], -1);
				cur_sibl = CTree[carc].ancest.back();
				while (cur_sibl != 0) {
					Mine_vec(cur_sibl - 1, 0, -1, CTree[carc].ancest, VTree[cur_sibl - 1].seq, _patt.str_pnt[pnt], 1);
					cur_sibl = VTree[cur_sibl - 1].sibl;
				}
				continue;
			}
			cur_sibl = Tree[cur_sibl].chld;
			while (cur_sibl != 0) {
				int cur_itm = Tree[cur_sibl].item;
				if (cur_itm < 0) {
					cur_itm = -cur_itm;
					if (slist[cur_itm - 1]) {
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0)
							pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);	
					}
					if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0) {
						DFS_seq.push_back(cur_sibl);
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
						if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0)
							pot_patt[cur_itm + L - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0)
						DFS_itm.push_back(cur_sibl);
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
		while(!DFS_seq.empty()) {
			unsigned long long int cur_sibl = DFS_seq.back();
			DFS_seq.pop_back();
			int num_found = 0;
			if (ilist_nempty) {
				num_found = DFS_numfound.back();
				DFS_numfound.pop_back();
			}
			if (Tree[cur_sibl].itmset < 0) {
				unsigned int carc = Tree[cur_sibl].chld;
				Mine_vec(carc, 0, num_found, CTree[carc].ancest, CTree[carc].seq, _patt.str_pnt[pnt], -1);
				cur_sibl = CTree[carc].ancest.back();
				while (cur_sibl != 0) {
					Mine_vec(cur_sibl - 1, 0, num_found, CTree[carc].ancest, VTree[cur_sibl - 1].seq, _patt.str_pnt[pnt], 1);
					cur_sibl = VTree[cur_sibl - 1].sibl;
				}
				continue;
			}
			cur_sibl = Tree[cur_sibl].chld;
			while (cur_sibl != 0) {
				int cur_itm = Tree[cur_sibl].item;
				if (cur_itm > 0) {
					if (num_found == itmset_size && ilist[cur_itm - 1] && (abs(Tree[Tree[cur_sibl].anct].itmset) < abs(Tree[_patt.str_pnt[pnt]].itmset) || !check_parent(Tree[cur_sibl].anct, _patt.str_pnt[pnt], last_strpnt[cur_itm - 1], pot_patt[cur_itm + L - 1].str_pnt))) {
						pot_patt[cur_itm + L - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0)
							pot_patt[cur_itm + L - 1].str_pnt.push_back(cur_sibl);
					}
					if (slist[cur_itm - 1] && abs(Tree[Tree[cur_sibl].anct].itmset) <= abs(Tree[_patt.str_pnt[pnt]].itmset)) {
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0)
							pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0) {
						DFS_seq.push_back(cur_sibl);
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
					if (slist[cur_itm - 1] && abs(Tree[Tree[cur_sibl].anct].itmset) <= abs(Tree[_patt.str_pnt[pnt]].itmset)) {
						pot_patt[cur_itm - 1].freq += Tree[cur_sibl].freq;
						if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0)
							pot_patt[cur_itm - 1].str_pnt.push_back(cur_sibl);
					}
					if (Tree[cur_sibl].chld != 0 || Tree[cur_sibl].itmset < 0) {
						DFS_seq.push_back(cur_sibl);
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
		pot_patt[p].str_pnt.shrink_to_fit();
		DFS.push_back(pot_patt[p]);
		DFS.back().seq = _patt.seq;
		DFS.back().seq.push_back(*it);
		if (*it < 0)
			DFS.back().list = slistp;
		else
			DFS.back().list = ilistp;
		if (!CTree.empty() && !pot_vpatt[p].str_pnt.empty()) {
			pot_vpatt[p].ass_patt = DFS.size() - 1;
			VDFS.push_back(pot_vpatt[p]);
		}
		if (b_disp || b_write) 
			Out_patt(DFS.back().seq, DFS.back().freq);
		++num_patt;
	}
}


void Mine_vec(unsigned long long int seq_ID, int pos, int num_found, vector<unsigned long long int>& ancest, vector<int>& items, unsigned long long int pnt, int sgn) {
	
	vector<bool> found(L + L * ilist_nempty, 0);

	if (num_found == -1) {
		while (pos < items.size() && items[pos] > 0) {
			int cur_itm = items[pos];
			if (ilist[cur_itm - 1] && !found[cur_itm + L - 1]) {
				if (pos + 1 < items.size()) {
					pot_vpatt[cur_itm + L - 1].seq_ID.push_back(seq_ID);
					pot_vpatt[cur_itm + L - 1].str_pnt.push_back(sgn * (pos + 1));
				}
				++pot_patt[cur_itm + L - 1].freq;
				found[cur_itm + L - 1] = 1;			
			}
			++pos;
		}
	}

	for (unsigned int k = pos; k < items.size(); ++k) {
		int cur_itm = abs(items[k]);
		if (items[k] < 0) 
			num_found = 0;
		if (slist[cur_itm - 1] && !found[cur_itm - 1]) {
			if (ancest.empty() || abs(Tree[ancest[cur_itm - 1]].itmset) <= abs(Tree[pnt].itmset)) {
				if (k + 1 < items.size()) {
					pot_vpatt[cur_itm - 1].seq_ID.push_back(seq_ID);
					pot_vpatt[cur_itm - 1].str_pnt.push_back(sgn * (k + 1));
				}
				++pot_patt[cur_itm - 1].freq;
			}
			found[cur_itm - 1] = 1;
		}
		if (num_found == itmset_size) {
			if (ilist[cur_itm - 1] && !found[cur_itm + L - 1]) {
				if (ancest.empty() || abs(Tree[ancest[cur_itm - 1]].itmset) < abs(Tree[pnt].itmset) || !check_parent(ancest[cur_itm - 1], pnt, last_strpnt[cur_itm - 1], pot_patt[cur_itm + L - 1].str_pnt)) {
					if (k + 1 < items.size()) {
						pot_vpatt[cur_itm + L - 1].seq_ID.push_back(seq_ID);
						pot_vpatt[cur_itm + L - 1].str_pnt.push_back(sgn * (k + 1));
					}
					++pot_patt[cur_itm + L - 1].freq;
				}
				found[cur_itm + L - 1] = 1;
			}
		}
		else if (cur_itm == abs(_patt.seq[last_neg + num_found])) 
			++num_found;
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





