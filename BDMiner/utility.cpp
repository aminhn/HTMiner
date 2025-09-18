#include "utility.hpp"
#include "build_mdd.hpp"
#include "load_inst.hpp"
#include <iostream>

int find_ID(vector<int>& vec, int itm) {
	int plc = 0;
	while (plc < vec.size() && vec[plc] != itm)
		++plc;

	if (plc == vec.size())
		return -1;
	else
		return plc;
}

bool check_parent(int cur_arc, int str_pnt, unordered_set<int>& counted) {

	vector<int> ancestors;
	
	int cur_anct = Tree[cur_arc].anct;

	while (Tree[cur_anct].itmset > Tree[str_pnt].itmset) {
		if (Tree[cur_anct].item > 0)
			ancestors.push_back(cur_anct);
		cur_anct = Tree[cur_anct].anct;
	}

	if (Tree[cur_anct].itmset == Tree[str_pnt].itmset)
		return 1;
	else {
		for (vector<int>::reverse_iterator it = ancestors.rbegin(); it != ancestors.rend(); ++it) {
			if (counted.find(*it) != counted.end())
				return 1;
		}
	}

	return 0;

}



float give_time(clock_t kk) {
	float ll = ((float)kk) / CLOCKS_PER_SEC;
	return ll;
}
