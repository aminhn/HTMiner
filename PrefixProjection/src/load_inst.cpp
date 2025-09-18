#include<iostream>
#include <sstream>
#include <algorithm>
#include "load_inst.hpp"
#include "freq_miner.hpp"
#include "utility.hpp"
#include <math.h>

using namespace std;

unsigned int M = 0, L = 0;
unsigned long long int N = 0, E = 0, theta;

vector<vector<int>> items;
vector<Pattern> DFS;
vector<int> item_dic;

bool Load_items(string& inst);
void Load_items_pre(string& inst);
bool Preprocess(string& inst, double thresh);

bool Load_instance(string &items_file, double thresh) {

	clock_t kk = clock();
	if (pre_pro) {
		if(!Preprocess(items_file, thresh))
			return 0;

		cout << "\nPreprocess done in " << give_time(clock() - kk) << " seconds\n\n";

		DFS.reserve(L);
		for (int i = 0; i < L; ++i)
			DFS.emplace_back(-i - 1);
	

		kk = clock();
		
		Load_items_pre(items_file);
	
		N = items.size();

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


	return 1;
}

bool Preprocess(string &inst, double thresh) {

	ifstream file(inst);

	vector<unsigned int> freq(1000000);
	vector<unsigned int> counted(1000000, 0);

	if (file.good()) {
		string line;
		int ditem;
		while (getline(file, line) && give_time(clock() - start_time) < time_limit) {
			++N;
			istringstream word(line);
			string itm;
			while (word >> itm) {
				ditem = stoi(itm);
				if (L < abs(ditem))
					L = abs(ditem);
			
				if (freq.size() < L) {
					freq.reserve(L);
					counted.reserve(L);
					while (freq.size() < L) {
						freq.push_back(0);
						counted.push_back(0);
					}
				}
				
				if (counted[abs(ditem) - 1] != N) {
					++freq[abs(ditem) - 1];
					counted[abs(ditem) - 1] = N;
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


void Load_items_pre(string &inst) {

	ifstream file(inst);

	if (file.good()) {
		string line;
		int size_m;
		int ditem;
		bool empty_seq = 0;
		while (getline(file, line) && give_time(clock() - start_time) < time_limit) {
			vector<bool> counted(L, 0);
			istringstream word(line);
			if (!empty_seq) {
				vector<int> temp;
				items.push_back(temp);
			}
			string itm;
			size_m = 0;
			bool sgn = 0; 
			empty_seq = 1;
			while (word >> itm) {
				
				ditem = stoi(itm);

				if (item_dic[abs(ditem) - 1] == -1) {
					if (!sgn)
						sgn = ditem < 0;
					continue;
				}
				else {
					if (ditem > 0)
						ditem = item_dic[ditem - 1];
					else
						ditem = -item_dic[-ditem - 1];
				}

				empty_seq = 0;

				if (sgn) {
					if (ditem > 0)
						ditem = -ditem;
					sgn = 0;
				}
			
				items.back().push_back(ditem);

				if (!counted[abs(ditem) - 1]) {
					DFS[abs(ditem) - 1].seq_ID.push_back(items.size() - 1);
					DFS[abs(ditem) - 1].str_pnt.push_back(items.back().size() - 1);
					++DFS[abs(ditem) - 1].freq;
					counted[abs(ditem) - 1] = 1;
				}

				++size_m;
			}

			if (empty_seq)
				continue;

			++N;

			E += size_m;

			if (size_m > M)
				M = size_m;
			
		}
	}
}

bool Load_items(string &inst) {

	ifstream file(inst);

	if (file.good()) {
		string line;
		int size_m;
		int ditem;
		while (getline(file, line) && give_time(clock() - start_time) < time_limit) {
			++N;
			vector<bool> counted(L, 0);
			istringstream word(line);
			items.emplace_back();
			string itm;
			size_m = 0;
			while (word >> itm) {
				ditem = stoi(itm);
				if (L < abs(ditem)) {
					L = abs(ditem);
					while (DFS.size() < L) {
						DFS.emplace_back(-DFS.size() - 1);
						counted.push_back(0);
					}
				}
			
				items.back().push_back(ditem);

				if (!counted[abs(ditem) - 1]) {
					DFS[abs(ditem) - 1].seq_ID.push_back(items.size() - 1);
					DFS[abs(ditem) - 1].str_pnt.push_back(items.back().size() - 1);
					++DFS[abs(ditem) - 1].freq;
					counted[abs(ditem) - 1] = 1;
				}

				++size_m;
			}

			E += size_m;

			if (size_m > M)
				M = size_m;
	
		}
			}
	else {
		cout << "!!!!!! No such file exists: " << inst << " !!!!!!\n";
		return 0;
	}

	return 1;

}









