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
 * This program transfers MILOU MC generator tables of CFFs into ROOT files used by EpIC.
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
	double xB, Q2;
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

		//store
		if (values.size() == 1) {
			singleValues.push_back(values.at(0));
		} else if (values.size() == 3) {

			if (singleValues.size() != 0) {

				if (singleValues.size() == 1) {
					Q2 = singleValues.at(0);
				} else if (singleValues.size() == 2) {

					xB = singleValues.at(0);
					Q2 = singleValues.at(1);
				} else {

					std::cout << "error: " << __func__
							<< ": inconsistent file (wrong number of single elements): "
							<< path << ", is:" << singleValues.size()
							<< std::endl;
					exit(0);
				}
			}

			std::vector<double> thisResult(5);

			thisResult.at(0) = xB;
			thisResult.at(1) = values.at(0);
			thisResult.at(2) = Q2;

			thisResult.at(3) = values.at(1);
			thisResult.at(4) = values.at(2);

			singleValues.clear();

			result.push_back(thisResult);

		} else {

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

void createTree(const std::vector<std::vector<double> >& grid,
		const std::string& type, const std::string& flavor) {

	//log
	std::cout << "info: " << __func__ << ": working for CFF " << type << "^"
			<< flavor << std::endl;

	//id
	std::string id = type + "_" + flavor;

	//create
	TNtuple tree(id.c_str(), "", "xB:t:Q2:Re:Im");

	//loop
	std::vector<std::vector<double> >::const_iterator it;

	for (it = grid.begin(); it != grid.end(); it++) {
		tree.Fill(it->at(0), it->at(1), it->at(2), it->at(3), it->at(4));
	}

	//write
	tree.Write();
}

int main(int argc, char *argv[]) {

	//arguments
	if (argc != 3) {

		std::cout << "error: " << __func__ << ": usage: " << argv[0]
				<< " path_to_directory output_file.root" << std::endl;
		exit(0);
	}

	std::string dirPath(argv[1]);
	std::string outFile(argv[2]);

	std::cout << "info: " << __func__ << ": directory path is: " << dirPath
			<< std::endl;
	std::cout << "info: " << __func__ << ": output file is: " << outFile
			<< std::endl;

	//open file
	TFile* rootFile = new TFile(outFile.c_str(), "RECREATE");

	//flavors
	std::vector<std::string> flavors(4);

	flavors.at(0) = "g";
	flavors.at(1) = "u";
	flavors.at(2) = "d";
	flavors.at(3) = "s";

	//H
	for (size_t i = 0; i < 4; i++) {

		std::vector<std::vector<double> > grid = readGrid(
				dirPath + "/" + "l" + flavors.at(i) + "amp.dat");

		createTree(grid, "H", flavors.at(i));

	}

	//E
	for (size_t i = 0; i < 4; i++) {

		std::vector<std::vector<double> > grid = readGrid(
				dirPath + "/" + "l" + flavors.at(i) + "ampe.dat");

		createTree(grid, "E", flavors.at(i));
	}

	//Ht
	for (size_t i = 0; i < 4; i++) {

		std::vector<std::vector<double> > grid = readGrid(
				dirPath + "/" + "l" + flavors.at(i) + "amppol.dat");

		createTree(grid, "Ht", flavors.at(i));
	}

	//Et
	for (size_t i = 0; i < 4; i++) {

		std::vector<std::vector<double> > grid = readGrid(
				dirPath + "/" + "l" + flavors.at(i) + "amppole.dat");

		createTree(grid, "Et", flavors.at(i));
	}

	//close
	rootFile->Close();

	//return
	return 0;
}
