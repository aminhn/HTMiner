#include <vector>
#include <iostream>
#include "load_inst.hpp"
#include "build_mdd.hpp"
#include "freq_miner.hpp"
#include "utility.hpp"

int Add_arc(int item, int last_arc, int& itmset, unordered_map<int, int>& ancest_map);
vector<Arc> Tree;

void Build_MDD(vector<int>& items) {

	unordered_map<int, int> ancest_map;	

	int last_arc = 0, itmset = 0;
	for (vector<int>::iterator it = items.begin(); it != items.end(); ++it) 
		last_arc = Add_arc(*it, last_arc, itmset, ancest_map);
		
}



int Add_arc(int item, int last_arc, int& itmset, unordered_map<int, int>& ancest_map) {
	
	int anct;
	unordered_map<int, int>::iterator p = ancest_map.find(abs(item));
	if (p == ancest_map.end())
		anct = 0;
	else
		anct = p->second;

	int plc = -1;
	for (int i = 0; i < Tree[last_arc].chld.size(); ++i) {
		if (Tree[Tree[last_arc].chld[i]].item == item) {
			plc = Tree[last_arc].chld[i];
			break;
		}	
	}

	if (item < 0)
		++itmset;

	if (plc == -1) {
		Tree.emplace_back(item, itmset, anct);
		plc = Tree.size() - 1;
		Tree[last_arc].chld.emplace_back(plc);
		if (anct == 0) 
			DFS[abs(item) - 1].str_pnt.push_back(plc);

	}

	if (anct == 0)
		++DFS[abs(item) - 1].freq;
	
	++Tree[plc].freq;
	
	ancest_map[abs(item)] = plc;

	return plc;

}

			







