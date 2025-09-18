#include <iostream>
#include <time.h>
#include <string.h>
#include <string>
#include "load_inst.hpp"
#include "freq_miner.hpp"
#include "utility.hpp"

using namespace std;

string out_file;

bool  b_disp = 0, b_write = 0, use_dic = 0, use_list = 0, pre_pro = 0;

unsigned int time_limit = 10 * 3600;

clock_t start_time;

int main(int argc, char* argv[]) {

        double thresh = 0;
        string VV, folder;
        for (int i = 1; i < argc; ++i){
		if (argv[i][0] !='-')
			continue;
		else if (strcmp(argv[i], "-thr") == 0)
			thresh = stof(argv[i + 1]);
                else if (strcmp(argv[i], "-file") == 0)
                        VV = argv[i + 1];
		else if (strcmp(argv[i], "-folder") == 0)
                        folder = argv[i + 1];
		else if (strcmp(argv[i], "-time") == 0)
			time_limit = stoi(argv[i + 1]);
		else if (strcmp(argv[i], "-uselist") == 0)
			use_list = 1;
		else if (strcmp(argv[i], "-preproc") == 0)
			pre_pro = 1;
		else if (strcmp(argv[i], "-dic") == 0)
			use_dic = 1;
		else if (strcmp(argv[i], "-out") == 0){
			if (i + 1 == argc || argv[i + 1][0] == '-')
				b_disp = 1;
			else if (argv[i + 1][0] == '+') {
				b_disp = 1; 
				b_write = 1;
				out_file = argv[i + 1];
				out_file = out_file.substr(1,out_file.size()-1);
			}
			else {
				b_write = 1;
				out_file = argv[i + 1];
			}
		}
		else
			cout << "Command " << argv[i] << " not recognized and skipped.\n";
        }

	if (thresh == 0){	
		cout << "No Threshold given, using threshold deafult of 1%\n";
		thresh = 0.01;
	}
	if (folder.back() != '/'){
		folder += '/';
	}


   	cout << "\n********************** " << VV << "**********************\n";

	string item_file = folder + VV + ".txt";
	//out_file = folder + VV + "_result.txt";
	cout << "loading instances...\n";

	start_time = clock();

	if(!Load_instance(item_file, thresh)){
		cout << "Files invalid, exiting.\n";
		return 0;
	}

	cout << "Instances loaded\n";

	if (give_time(clock() - start_time) < time_limit) {
		cout << "\nRunning mining algorithm...\n";
		Freq_miner();
		if (give_time(clock() - start_time) >= time_limit)
			cout << "TIME LIMIT REACHED\n";
		cout << "Mining Complete\n\nFound a total of " << num_patt << " patterns\n";
	}

	cout << "Total CPU time is: ";
	cout << give_time(clock() - start_time) << "\n";

	if (b_write){
		ofstream file;
		file.open(out_file, std::ios::app);
		file << "\nMining completed in " << give_time(clock() - start_time) << " seconds\n";
		//file << "Found a total of " << num_max_patt << " maximal patterns\n";
		file.close();
	}


	return 0;
}
