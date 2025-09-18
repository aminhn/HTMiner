#include <vector>
#include <iostream>
#include "load_inst.hpp"
#include "build_mdd.hpp"
#include "freq_miner.hpp"
#include "utility.hpp"

int Add_arc(int item, unsigned int last_arc, int& itmset, vector<unsigned int>& ancest_map);
void Add_vec(vector<int>& items_lim, vector<unsigned int>& ancest_map, unsigned int last_arc, int itmset);

vector<Arc> Tree;
vector<VArc> VTree;
vector<CArc> CTree;

void Build_MDD(vector<int>& items, vector<int>& items_lim) {

	vector<unsigned int> ancest_map(L, 0);	

	unsigned int last_arc = 0;
	int itmset = 0;
	for (vector<int>::iterator it = items.begin(); it != items.end(); ++it) 
		last_arc = Add_arc(*it, last_arc, itmset, ancest_map);

	if (!items_lim.empty())  
		Add_vec(items_lim, ancest_map, last_arc, itmset);	
	

		
}



int Add_arc(int item, unsigned int last_arc, int& itmset, vector<unsigned int>& ancest_map) {

	unsigned int anct;
	anct = ancest_map[abs(item) - 1];

	if (item < 0)
		++itmset;

	unsigned int last_sibl = Tree[last_arc].chld;

	if (last_sibl == 0) {
		Tree.emplace_back(item, itmset, anct);
		last_sibl = Tree.size() - 1;
		Tree[last_arc].chld = last_sibl;
		if (anct == 0) 
			DFS[abs(item) - 1].str_pnt.push_back(last_sibl);

	}
	else {
		while (Tree[last_sibl].item != item) {
			if (Tree[last_sibl].sibl == 0) {
				Tree.emplace_back(item, itmset, anct);
				Tree[last_sibl].sibl = Tree.size() - 1;
				last_sibl = Tree.size() - 1;
				if (anct == 0) 
					DFS[abs(item) - 1].str_pnt.push_back(last_sibl);
				break;
			}
			last_sibl = Tree[last_sibl].sibl;
		}

	}

	if (anct == 0)
		++DFS[abs(item) - 1].freq;

	++Tree[last_sibl].freq;

	ancest_map[abs(item) - 1] = last_sibl;

	return last_sibl;

}

void Add_vec(vector<int>& items_lim, vector<unsigned int>& ancest, unsigned int last_arc, int itmset) {

	items_lim.shrink_to_fit();
	vector<bool> counted(L, 0);							
		
	if (Tree[last_arc].itmset > 0) {
		ancest.push_back(0);			//last element of ancest is Carc child
		ancest.shrink_to_fit();
		for (int i = 0; i < items_lim.size(); ++i) {
			int cur_itm = abs(items_lim[i]); 
			if (ancest[cur_itm - 1] == 0 && !counted[cur_itm - 1]) {
				if (i + 1 < items_lim.size()) {
					VDFS[cur_itm - 1].str_pnt.push_back(-i - 1);			//str_pnts of Ctree are indicated by negative values
					VDFS[cur_itm - 1].seq_ID.push_back(CTree.size());

				}			
				++DFS[cur_itm - 1].freq;
				counted[cur_itm - 1] = 1;
			}
		}
		CTree.emplace_back(ancest, items_lim);
		Tree[last_arc].chld = CTree.size() - 1;		
		Tree[last_arc].itmset = -itmset;		//Tree connection to CTree is indicated by negative itmset value
	}
	else {
		vector<unsigned int>& ancest = CTree[Tree[last_arc].chld].ancest;
		for (int i = 0; i < items_lim.size(); ++i) {
			int cur_itm = abs(items_lim[i]); 
			if (!counted[cur_itm - 1] && ancest[cur_itm - 1] == 0) {
				if (i + 1 < items_lim.size()) {
					VDFS[cur_itm - 1].str_pnt.push_back(i + 1);			
					VDFS[cur_itm - 1].seq_ID.push_back(VTree.size());
				}
				++DFS[cur_itm - 1].freq;
				counted[cur_itm - 1] = 1;
			}
		}
		VTree.emplace_back(items_lim, CTree[Tree[last_arc].chld].ancest.back());
		CTree[Tree[last_arc].chld].ancest.back() = VTree.size();		//Vtree siblings and Ctree children are +1 of their actual position to distinguish end of children/siblings
	}
}
	






