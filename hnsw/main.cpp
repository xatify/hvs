#include <string.h>
#include <stdlib.h>

#include <getopt.h>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>

void build_and_query(
	char* dataset, 
	char* queries,
	char* index_dir, 
	int dataset_size,
	int ts_length,
	int level,
	int queries_size,
	float delta,
	int efsearch
);

void help(void) {
	std::cerr <<
		"Usage:\n\
        \t--dataset XX \t\t\tThe path to the dataset file\n\
        \t--queries XX \t\t\tThe path to the queries file\n\
        \t--index-path XX \t\tThe path of the output folder\n\
        \t--dataset-size XX \t\tThe number of time series to load\n\
        \t--queries-size XX \t\tThe number of queries to run\n\
        \t--mode: 0=index, 1=query\t\t\n\
        \t--timeseries-size XX\t\tThe size of each time series\n\
        \t--level XX\t\t\tThe level\n\
		\t--delta XX\t\tdelta\n\
		\t--ef XX\t\t\tefConstruction\n\
		\t--help\t\tprint help\n";
}


bool directory_exist(const std::string& path) {
	struct stat sb;

	return (stat(path.c_str(), &sb) == 0);
}

int main(int argc, char **argv) {

	static char *dataset = "";
	static char *queries = "";
	static char *index_path = "";
	static unsigned int dataset_size = 0;
	static unsigned int queries_size = 0;
	static unsigned int ts_length;
	static unsigned int level = 1;
	static float delta = 0.5;
	static int ef = 1000;
	static int mode = 0;

	static const struct option long_options[] = {
		{"dataset", required_argument, 0, 'd'},
		{"queries", required_argument, 0, 'q'},
		{"index-path", required_argument, 0, 'p'},
		{"dataset-size", required_argument, 0, 'z'},
		{"queries-size", required_argument, 0, 'g'},
		{"mode", required_argument, 0, 'x'},
		{"timeseries-size", required_argument, 0, 't'},
		{"level", required_argument, 0, 'l'},
		{"delta", required_argument, 0, 'de'},
		{"ef", required_argument, 0, 'e'},
		{"help", no_argument, 0, 'ls'}
	};
	int option_index = 0;

	while(1) {
		int c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1) break;
		switch (c) {
			case 'd':
				dataset = optarg;
				break;
			case 'q':
				queries = optarg;
				break;
			case 'p':
				index_path = optarg;
				break;
			case 'z':
				dataset_size = atoi(optarg);
				if (dataset_size < 1) {
					std::cerr << "change dataset size to be greater than 1";
					exit(1);
				}
				break;
			case 'g':
				queries_size = atoi(optarg);
				if (queries_size < 1) {
					std::cerr << "change queries size to be greater than 1";
					exit(1);
				}
				break;
			case 'x':
				mode = atoi(optarg);
				break;
			case 't':
				ts_length = atoi(optarg);
				break;
			case 'l':
				level = atoi(optarg);
				break;
			case 'de':
				delta = atof(optarg);
				break;
			case 'e':
				ef = atoi(optarg);
				break;
			case '?':
			default:
				help();
				break;
		} 
	}

	if (mode == 0) {
		build_and_query(dataset, nullptr, index_path, dataset_size, ts_length, level, 0, delta, -1);
	}
	else if (mode == 1) {
		build_and_query(nullptr, queries, index_path, dataset_size, ts_length, level, queries_size, delta, ef);
	}
    return 0;
};
