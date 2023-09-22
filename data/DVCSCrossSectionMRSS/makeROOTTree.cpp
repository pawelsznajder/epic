#include <stddef.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <TFile.h>
#include <TNtuple.h>

/**
 * This program converts tables of CSs into ROOT files used by EpIC.
 *
 * compilation: g++ makeROOTTree.cpp `root-config --libs` -o makeROOTTree.exe
 */

std::vector<std::vector<double> > readGrid(const std::string& path) {

	//open file
	std::ifstream file;
	file.open(path.c_str());

	if (!file.is_open()) {

		std::cout << "error: " << __func__ << ": unable to open file: " << path
				<< std::endl;
		exit(0);
	}

	//result
	std::vector<std::vector<double> > result;

	//string stream
	std::stringstream ss;

	//loop over lines
	std::string line;

	while (getline(file, line)) {

		//set strings stream
		ss.str(std::string());
		ss.clear();
		ss.str(line);

		//values in this line
		double value;
		std::vector<double> values;

		while (ss >> value) {
			values.push_back(value);
		}

		//store
		if (values.size() == 7) {
			result.push_back(values);
		} 
		else {

			std::cout << "error:" << __func__
					<< ": inconsistent file (wrong number of elements in single line): "
					<< path << ", is: " << values.size() << std::endl;
			exit(0);
		}

	}

	//close
	file.close();

	//return
	return result;
}

int main(int argc, char *argv[]) {

	//arguments
	if (argc != 3) {

		std::cout << "error: " << __func__ << ": usage: " << argv[0]
				<< " input_file.txt output_file.root" << std::endl;
		exit(0);
	}

	std::string inFile(argv[1]);
	std::string outFile(argv[2]);

	std::cout << "info: " << __func__ << ": input file is: " << inFile 
			<< std::endl;
	std::cout << "info: " << __func__ << ": output file is: " << outFile
			<< std::endl;

	//open file
	TFile* rootFile = new TFile(outFile.c_str(), "RECREATE");
		
	//read
	std::vector<std::vector<double> > grid = readGrid(inFile); 

	//create
	TNtuple tree("xsections", "", "xB:t:Q2:xs0:xs1:xs2:xs3");

	//loop
	std::vector<std::vector<double> >::const_iterator it;

	for (it = grid.begin(); it != grid.end(); it++) {
		tree.Fill(it->at(1), -1*it->at(2), it->at(0), it->at(3), it->at(4), it->at(5), it->at(6));
	}

	//write
	tree.Write();

	//close
	rootFile->Close();

	//return
	return 0;
}
