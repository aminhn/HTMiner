#include<iostream>
#include <sstream>
#include <algorithm>
#include "load_inst.hpp"
#include <time.h>
#include "utility.hpp"
#include "build_mdd.hpp"
#include <math.h>
#include "freq_miner.hpp"

using namespace std;

int M = 0, L = 0;
unsigned long long int N = 0, num_nodes = 0, theta, E = 0;

bool itmset_exists = 0;

vector<int> item_dic;
vector<Pattern> DFS;

void Load_items_pre(string &inst_name);
bool Load_items(string &inst_name);
bool Preprocess(string& inst, double thresh);

bool Load_instance(string& items_file, double thresh) {
	
	clock_t kk = clock();

	Tree.emplace_back(0, 0, 0);

	if (pre_pro) {
		if(!Preprocess(items_file, thresh))
			return 0;

		cout << "\nPreprocess done in " << give_time(clock() - kk) << " seconds\n\n";

		DFS.reserve(L);
		for (int i = 0; i < L; ++i)
			DFS.emplace_back(-i - 1);

		kk = clock();

		Load_items_pre(items_file);

	}
	else if (!Load_items(items_file))
		return 0;
	else {
		if (thresh < 1)
			theta = ceil(thresh * N);
		else
			theta = thresh;
	}
	
	cout << "\nMDD Database built in " << give_time(clock() - kk) << " seconds\n\n";

	cout << "Found " << N << " sequence, with max line len " << M << ", and " << L << " items, and " << E << " enteries\n";

	cout << "Total MDD nodes: " << Tree.size() << endl;

	return 1;

}

bool Preprocess(string &inst, double thresh) {

	ifstream file(inst);

	vector<unsigned long long int> freq(1000000);
	vector<unsigned long long int> counted(1000000, 0);

	if (file.good()) {
		string line;
		int size_m;
		int ditem;
		while (getline(file, line) && give_time(clock() - start_time) < time_limit) {
			++N;
//if (N % 1000000 == 0)
//cout << "N: " << N << endl;
			istringstream word(line);
			string itm;
			while (word >> itm) {
				ditem = abs(stoi(itm));
				if (L < ditem)
					L = ditem;
			
				while (freq.size() < L) {
					freq.push_back(0);
					counted.push_back(0);
				}
				
				if (counted[ditem - 1] != N) {
					++freq[ditem - 1];
					counted[ditem - 1] = N;
				}
			}
		}
	}
	else {
		cout << "!!!!!! No such file exists: " << inst << " !!!!!!\n";
		return 0;
	}

	if (thresh < 1)
		theta = ceil(thresh * N);
	else
		theta = thresh;

	int real_L = 0;
	item_dic = vector<int>(L, -1);
	for (int i = 0; i < L; ++i) {
		if (freq[i] >= theta) 
			item_dic[i] = ++real_L;
	}
	
	cout << "Original number of items: " << L << " Reduced to: " << real_L << endl;

	L = real_L;
	N = 0;


	return 1;
}


void Load_items_pre(string &inst_name) {

	ifstream file(inst_name);

	if (file.good()) {
		string line;
		int ditem;
		while (getline(file, line) && give_time(clock() - start_time) < time_limit) {
			istringstream word(line);
			string itm;
			vector<int> temp_vec;
			bool sgn = 0;
			while (word >> itm) {
				
				ditem = stoi(itm);
				
				if (item_dic[abs(ditem) - 1] == -1) {
					if (!sgn)
						sgn = ditem < 0;
					continue;
				}
				else {
					if (ditem > 0) {
						ditem = item_dic[ditem - 1];
						itmset_exists = 1;
					}
					else
						ditem = -item_dic[-ditem - 1];
				}

				if (sgn) {
					if (ditem > 0)
						ditem = -ditem;
					sgn = 0;
				}

				temp_vec.push_back(ditem);
			}

			if (temp_vec.empty())
				continue;

			++N;
			if (N % 1000000 == 0)
				cout << N << endl;

			if (temp_vec.size() > M)
				M = temp_vec.size();

			Build_MDD(temp_vec);
		}
	}

}

bool Load_items(string &inst_name) {

	ifstream file(inst_name);

	if (file.good()) {
		string line;
		int ditem;
		while (getline(file, line) && give_time(clock() - start_time) < time_limit) {
			++N;
if (N % 1000000 == 0)
	cout << "Found " << N << " sequence, with max line len " << M << ", and " << L << " items, and " << E << " enteries\n";
			istringstream word(line);
			string itm;
			vector<int> temp_vec;
			while (word >> itm) {
				ditem = stoi(itm);
				if (ditem > 0)
					itmset_exists = 1;
				if (L < abs(ditem)) {
					L = abs(ditem);
					while (DFS.size() < L && !just_build) {
						DFS.reserve(L);
						DFS.emplace_back(-DFS.size() - 1);
					}
				}

				temp_vec.push_back(ditem);
			}

			if (temp_vec.size() > M)
				M = temp_vec.size();

			Build_MDD(temp_vec);
		}
	}
	else {
		cout << "!!!!!! No such file exists: " << inst_name << " !!!!!!\n";
		return 0;
	}

	return 1;

}










