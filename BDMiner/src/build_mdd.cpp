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

	if (item < 0)
		++itmset;

	int last_sibl = Tree[last_arc].chld;

	if (last_sibl == -1) {
		Tree.emplace_back(item, itmset, anct);
		last_sibl = Tree.size() - 1;
		Tree[last_arc].chld = last_sibl;
		if (anct == 0) 
			DFS[abs(item) - 1].str_pnt.push_back(last_sibl);

	}
	else {

		while (Tree[last_sibl].item != item) {
			if (Tree[last_sibl].sibl == -1) {
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

	ancest_map[abs(item)] = last_sibl;

	return last_sibl;

}
	







