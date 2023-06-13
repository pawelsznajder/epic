/**
 * This macro transfers text tables of CFFs into ROOT files used by EpIC.
 *
 * compilation: g++ makeROOTTree.cpp `root-config --libs` -o makeROOTTree.exe
 */

std::vector<double> readData(const std::string& path, size_t column) {

    //result
    std::vector<double> result;

    //string stream
    std::stringstream ss;

    //open file
    std::ifstream file;
    file.open(path.c_str());

    if (!file.is_open()) {
        std::cout << __func__ << ": unable to read: " << path
                << std::endl;
        exit(0);
    }

    //loop over lines
    std::string line;

    while (getline(file, line)) {

        //data
        double data;

        //set strings stream
        ss.str(std::string());
        ss.clear();
        ss.str(line);

        //set
        for (size_t j = 0;; j++) {

            if (ss.eof()){

               std::cout << __func__ << ": wrong column: " << column << std::endl;
               file.close();
               exit(0);
            }

            ss >> data;

            if(j == column){

               result.push_back(data);
               break;
            }
        }
    }

    //close
    file.close();

    if(result.size() == 0){
        std::cout << __func__ << ": empty file: " << path << std::endl;
        exit(0);
    }

    //return
    return result;
}

void makeROOTTree(){

        std::vector<double> xi = readData("result", 0);
        std::cout << "xi done" << std::endl;
        std::vector<double> t = readData("result", 1);
        std::cout << "t done" << std::endl;
        std::vector<double> Q2 = readData("result", 2);
        std::cout << "Q2 done" << std::endl;
        std::vector<double> GPD = readData("result", 3);
        std::cout << "GPD done" << std::endl;
        std::vector<double> re = readData("result", 4);
        std::cout << "Re done" << std::endl;
        std::vector<double> im = readData("result", 5);
        std::cout << "Im done" << std::endl;

        TFile* rootFile = new TFile("pi0_table_GK.root", "RECREATE");

        for(size_t i = 0; i < 4; i++){

            std::cout << i << std::endl;

            double thisGPD;

            if(i == 0) thisGPD = 4;
            if(i == 1) thisGPD = 5;
            if(i == 2) thisGPD = 6;
            if(i == 3) thisGPD = 18;

            std::string name;

            if(i == 0) name = "Ht";
            if(i == 1) name = "Et";
            if(i == 2) name = "HTrans";
            if(i == 3) name = "EbarTrans";

            TNtuple* tree = new TNtuple(name.c_str(), "", "xi:t:Q2:Re:Im");

            for(size_t j = 0; j < xi.size(); j++){

                if(GPD.at(j) != thisGPD) continue;

                tree->Fill(xi.at(j), t.at(j), Q2.at(j), re.at(j) , im.at(j));
            }

            tree->Write();
        }

        rootFile->Close();
}
