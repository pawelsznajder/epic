#include <TFile.h>
#include <TNtuple.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

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

    //loop over lines
    std::string line;

    while (getline(file, line)) {

        //set strings stream
        ss.str(std::string());
        ss.clear();
        ss.str(line);

        //values in this line
        std::string valueStr;
        std::vector<double> values;

        while (ss >> valueStr) {
            double value = std::atof(valueStr.c_str());
            values.push_back(value);
        }

        if (values.size() != 10) {
            std::cout << "error: " << __func__ << ": wrong number of elements"
                    << " " << values.size() << " " << result.size()
                    << std::endl;
            exit(0);
        }

        result.push_back(values);
    }

    //close
    file.close();

    //return
    return result;
}

void createTree(const std::vector<std::vector<double> >& grid) {

    //id
    std::string id = "pi0_cross_section";

    //create
    TNtuple tree(id.c_str(), "", "xB:t:Q2:Ee:Ep:phi:cs");

    //loop
    std::vector<std::vector<double> >::const_iterator it;

    for (it = grid.begin(); it != grid.end(); it++) {
        tree.Fill(it->at(0), it->at(1), it->at(2), it->at(3), it->at(4),
                it->at(5), it->at(9));
    }

    //write
    tree.Write();
}

int main(int argc, char *argv[]) {

    //arguments
    if (argc != 3) {

        std::cout << "error: " << __func__ << ": usage: " << argv[0]
                << " input.dat output_file.root" << std::endl;
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

    //read and create
    std::vector<std::vector<double> > grid = readGrid(inFile);
    createTree(grid);

    //close
    rootFile->Close();

    //return
    return 0;
}
