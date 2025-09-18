#include <iostream>
#include <time.h>
#include <string.h>
#include <string>
#include "load_inst.hpp"
#include "build_mdd.hpp"
#include "utility.hpp"
#include "freq_miner.hpp"

using namespace std;

string out_file;

bool  b_disp = 0, b_write = 0, use_dic = 0, just_build = 0, pre_pro = 1;

unsigned int time_limit = 10 * 3600;

clock_t start_time;

string folder;

int main(int argc, char* argv[]) {
		
	string VV, attr;

	double thresh = 0;
	for (int i = 1; i<argc; i++) {
		if (argv[i][0] != '-' || isdigit(argv[i][1]))
			continue;
		else if (strcmp(argv[i], "-thr") == 0)
			thresh = stod(argv[i + 1]);
		else if (strcmp(argv[i], "-file") == 0)
			VV = argv[i + 1];
		else if (strcmp(argv[i], "-time") == 0)
			time_limit = stoi(argv[i + 1]);
		else if (strcmp(argv[i], "-jbuild") == 0)
			just_build = 1;
		else if (strcmp(argv[i], "-folder") == 0)
			folder = argv[i + 1];
		else if (strcmp(argv[i], "-npre") == 0)
			pre_pro = 0;
		else if (strcmp(argv[i], "-dic") == 0)
			use_dic = 1;
		else if (strcmp(argv[i], "-out") == 0) {
			if (i + 1 == argc || argv[i + 1][0] == '-')
				b_disp = 1;
			else if (argv[i + 1][0] == '+') {
				b_disp = 1;
				b_write = 1;
				if (strlen(argv[i + 1]) > 1) {
					out_file = argv[i + 1];
					out_file = out_file.substr(1, out_file.size() - 1);
				}
				else
					out_file = VV;
			}
			else {
				b_write = 1;
				out_file = argv[i + 1];
			}
		}

		else
			cout << "Command " << argv[i] << " not recognized and skipped.\n";
	}



    cout << "\n********************** " << VV << "**********************\n";

	string item_file = folder + VV + ".txt";

	cout << "loading instances...\n";

	start_time = clock();

	if (!Load_instance(item_file, thresh)) {
		cout << "Files invalid, exiting.\n";
		cin.get();
		return 0;
	}

	//kk = clock();

	if (!just_build && give_time(clock() - start_time) < time_limit) {
		Freq_miner();
		if (give_time(clock() - start_time) >= time_limit)
			cout << "TIME LIMIT REACHED\n";
		cout << "Mining Complete\n\nFound a total of " << num_patt << " patterns\n";	
		cout << "\nTotal CPU time " << give_time(clock() - start_time) << " seconds\n\n";
	}


	return 0;
}
