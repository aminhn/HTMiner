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

unsigned int M = 0, E = 0, mlim;

unsigned long long int N = 0, L = 0, theta;

bool itmset_exists = 0;

vector<int> item_dic;
vector<Pattern> DFS;
vector<VPattern> VDFS;

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
		if (Tree.size() > 100000000) {
			Tree.shrink_to_fit();
			CTree.shrink_to_fit();
			VTree.shrink_to_fit();
		}
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

	cout << "Total Trie nodes: " << Tree.size() << " Total CTree nodes: " << CTree.size() << " Total VTree nodes: " << VTree.size() << endl;

	return 1;

}

bool Preprocess(string &inst, double thresh) {


	vector<unsigned long long int> MN(100, 0);
	vector<vector<bool>> ML(100, vector<bool>(1000000, 0));
	

	ifstream file(inst);

	vector<unsigned int> freq(1000000, 0);
	vector<unsigned long long int> counted(1000000, 0);

	if (file.good()) {
		string line;
		int ditem;
		while (getline(file, line) && give_time(clock() - start_time) < time_limit) {
			++N;
			
//if (N % 10000000 == 0)
//cout << "N: " << N << endl;
			istringstream word(line);
			string itm;
			int size_m = 0;
			while (word >> itm) {
				++size_m;
				ditem = stoi(itm);

				if (ditem > 0) 
					itmset_exists = 1;
				else 
					ditem *= -1;

				if (size_m < MN.size()) {
					++MN[size_m - 1];
					if (ML[size_m - 1].size() < ditem) {
						ML[size_m - 1].reserve(ditem);
						while (ML[size_m - 1].size() < ditem)
							ML[size_m - 1].push_back(0);
					}
					ML[size_m - 1][ditem - 1] = 1;
				}
	
				if (L < ditem)
					L = ditem;
				
				if (freq.size() < L) {
					freq.reserve(L);
					counted.reserve(L);
					while (freq.size() < L) { 
						freq.push_back(0);
						counted.push_back(0);
					}
				}

				if (counted[ditem - 1] != N) {
					++freq[ditem - 1];
					counted[ditem - 1] = N;
				}

			}
			if (size_m > M)
				M = size_m;
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
	vector<bool> item_in(L, 0);
	for (int i = 0; i < L; ++i) {
		if (freq[i] >= theta) {
			item_dic[i] = ++real_L;
			item_in[i] = 1;
		}
	}
	
	cout << "Original number of items: " << L << " Reduced to: " << real_L << endl;

	unsigned long long int LpM = 1;
	mlim = M;
	int orgmlim;
	int ulim = min(3 + real_L / 5, 10);
	unsigned long long int ml;
	int coef = 1 + 1 * itmset_exists;
	for (int i = 0; i + ulim < MN.size() && i + ulim < M; ++i) {
		ml = 0;
		for (int j = 0; j < L; ++j) {
			if (ML[i][j] && item_in[j]) 
				++ml;
		}
		LpM *= ml * coef;
		cout << ml << " " << LpM << " " << MN[i] << endl;
		if (LpM * ulim > MN[i]) {
		if (6 * (MN[i] - LpM) >= 5 * MN[i])
			orgmlim = i;
			while (i + ulim - 1 < MN.size() && i + ulim - 1 < M) {
				cout << MN[i - 1] - MN[i + ulim - 1] << " " << MN[i + ulim - 1] << endl;
				if (MN[i - 1] - MN[i + ulim - 1] < MN[i + ulim - 1] && MN[i + ulim - 1] < 600000000 ) {
					mlim = i - 1;
					break;
				}					
				i += 1;
			}
			break;
		}
	}

	cout << "M is: " << M << " Mlim is: " << mlim << " ulim is: " << ulim << " original mlim is: " << orgmlim << " guess is: " << round((log(N) - log(6)) / log(real_L)) << endl;


	if (mlim < M) {
		for (int i = 0; i < real_L; ++i) 
			VDFS.emplace_back(i);
		if (MN[mlim + ulim] > 100000000) {
			CTree.reserve(MN[mlim + ulim] / 2);
			VTree.reserve(MN[mlim + ulim] / 2);
			Tree.reserve((N - MN[mlim + ulim]) * 2);
		}
	}
	else if (N > 100000000) 
		Tree.reserve(500000000);


	L = real_L;
	N = 0;
	M = 0;

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
			vector<int> temp_lim;
			bool sgn = 0;
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

				if (sgn) {
					if (ditem > 0)
						ditem = -ditem;
					sgn = 0;
				}
				
				if (temp_vec.size() <= mlim)
					temp_vec.push_back(ditem);
				else
					temp_lim.push_back(ditem);
			}

			if (temp_vec.empty())
				continue;

			++N;
			//if (N % 1000000 == 0)
			//	cout << N << " " << Tree.size() << " " << CTree.size() << " " << VTree.size() << endl;

			if (temp_vec.size() + temp_lim.size() > M)
				M = temp_vec.size() + temp_lim.size();

			Build_MDD(temp_vec, temp_lim);
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
			vector<int> temp_lim;
			while (word >> itm) {
				ditem = stoi(itm);
				if (ditem > 0)
					itmset_exists = 1;
				if (L < abs(ditem)) {
					L = abs(ditem);
					while (DFS.size() < L) {
						DFS.reserve(L);
						DFS.emplace_back(-DFS.size() - 1);
					}
				}

				if (temp_vec.size() < mlim)
					temp_vec.push_back(ditem);
				else
					temp_lim.push_back(ditem);
			}

			if (temp_vec.size() + temp_lim.size() > M)
				M = temp_vec.size();

			Build_MDD(temp_vec, temp_lim);
		}
	}
	else {
		cout << "!!!!!! No such file exists: " << inst_name << " !!!!!!\n";
		return 0;
	}

	return 1;

}










