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
 * This program transfers text tables of CFFs into ROOT files used by EpIC.
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

	//kinematic values
	std::vector<double> singleValues;

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

		if(values.size() != 12){

			std::cout << "error:" << __func__
					<< ": inconsistent file (wrong number of elements in single line): "
					<< path << ", is: " << values.size() << std::endl;
			exit(0);
		}

		result.push_back(values);
	}

	//close
	file.close();

	//return
	return result;
}

void createTree(const std::vector<std::vector<double> >& grid,
		const std::string& type) {

	//log
	std::cout << "info: " << __func__ << ": working for CFF " << type << std::endl;

	//id
	std::string id = type;

	//create
	TNtuple tree(id.c_str(), "", "xi:t:Q2:Q2Prim:Re:Im");

	//index
	int index = -1;

	if(type == "H") index = 4;
	if(type == "E") index = 6;
	if(type == "Ht") index = 8;
	if(type == "Et") index = 10;

	if(index == -1){

		std::cout << "error:" << __func__
			<< ": can't find index for type: " << type << std::endl;

		exit(0);
	}

	//loop
	std::vector<std::vector<double> >::const_iterator it;

	for (it = grid.begin(); it != grid.end(); it++) {
		tree.Fill(it->at(0), it->at(1), it->at(2), it->at(3), it->at(index), it->at(index + 1));
	}

	//write
	tree.Write();
}

int main(int argc, char *argv[]) {

	//arguments
	if (argc != 3) {

		std::cout << "error: " << __func__ << ": usage: " << argv[0]
				<< " path_to_file output_file.root" << std::endl;
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

	//read grid
	std::vector<std::vector<double> > grid = readGrid(
		inFile);

	//create tree
	createTree(grid, "H");
	createTree(grid, "E");
	createTree(grid, "Ht");
	createTree(grid, "Et");

	//close
	rootFile->Close();

	//return
	return 0;
}
