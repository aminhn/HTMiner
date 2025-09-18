#include "utility.hpp"
#include "build_mdd.hpp"
#include "load_inst.hpp"
#include <iostream>

bool check_parent(unsigned int cur_anct, unsigned int str_pnt, unsigned int start, vector<unsigned int>& strpnt_vec) {

	vector<unsigned int> ancestors;
	
	while (abs(Tree[cur_anct].itmset) > abs(Tree[str_pnt].itmset)) {
		if (Tree[cur_anct].item > 0)
			ancestors.push_back(cur_anct);
		cur_anct = Tree[cur_anct].anct;
	}

	if (abs(Tree[cur_anct].itmset) == abs(Tree[str_pnt].itmset))
		return 1;
	else {
		for (vector<unsigned int>::reverse_iterator it = ancestors.rbegin(); it != ancestors.rend(); ++it) {
			for (unsigned int i = start; i < strpnt_vec.size(); ++i) {
				if (strpnt_vec[i] == *it)
					return 1;
			}
		}
	}

	return 0;

}



float give_time(clock_t kk) {
	float ll = ((float)kk) / CLOCKS_PER_SEC;
	return ll;
}
