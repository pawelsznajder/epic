#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

#include <TH1D.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TLorentzVector.h> 

#include "ref_partons/partons_dvcs.h"
#include "ref_partons/partons_tcs.h"
#include "ref_partons/partons_ddvcs.h"
#include "ref_partons/partons_dvmp.h"

double c_toleranceA = 0.01;	//to PARTONS
double c_toleranceB = 0.01; //to EpIC reference histograms
double c_toleranceC = 0.95; //to EpIC histograms

struct Event{
   std::vector<TLorentzVector> m_particles;
};

double getPhi(const TLorentzVector& q, 
                const TLorentzVector& p, const TLorentzVector& e, 
                const TLorentzVector& eS, const TLorentzVector& v) {

	TVector3 boost;
	TLorentzVector q_boosted, p_boosted, e_boosted, eS_boosted, v_boosted;
	double sinb2, cosb2;
	double sign;

	boost = (q+p).BoostVector();
	q_boosted = q;                q_boosted.Boost(-boost);
	p_boosted = p;                p_boosted.Boost(-boost);
	e_boosted = e;                e_boosted.Boost(-boost);
	eS_boosted = eS;              eS_boosted.Boost(-boost);
	v_boosted = v;                v_boosted.Boost(-boost);

	TVector3 a = e_boosted.Vect().Unit().Cross(eS_boosted.Vect().Unit());
	TVector3 b = q_boosted.Vect().Unit().Cross(v_boosted.Vect().Unit());

	sign = ( ((e_boosted.Vect()).Cross(eS_boosted.Vect())).Unit() ).Cross( ((q_boosted.Vect()).Cross(v_boosted.Vect())).Unit() ).Dot( (q_boosted.Vect()).Unit() );
	sign /= fabs(sign);

	sign = a.Cross(b).Dot((q_boosted.Vect()).Unit());
	sign /= fabs(sign);

	sinb2 = a.Cross(b).Mag();
	cosb2 = a.Dot(b);

	double phi = atan2(sign*sinb2, cosb2);
	if( phi < 0. ) phi = phi + 2.*TMath::Pi();

	return phi;
}

double getPhiL(const TLorentzVector& q, 
                const TLorentzVector& p, const TLorentzVector& pS, const TLorentzVector& e, 
                const TLorentzVector& eS) {

	TVector3 boost;
	TLorentzVector q_boosted, p_boosted, pS_boosted, e_boosted, eS_boosted;
	double sinb2, cosb2;
	double sign;

	boost = (q+pS).BoostVector();
	q_boosted = q;                q_boosted.Boost(-boost);
	p_boosted = p;                p_boosted.Boost(-boost);
	pS_boosted = pS;              pS_boosted.Boost(-boost);
	e_boosted = e;                e_boosted.Boost(-boost);
	eS_boosted = eS;              eS_boosted.Boost(-boost);

	TVector3 a = pS_boosted.Vect().Unit().Cross(p_boosted.Vect().Unit());
	TVector3 b = eS_boosted.Vect().Unit().Cross(e_boosted.Vect().Unit());

	sign = a.Cross(b).Dot((pS_boosted.Vect()).Unit());
	sign /= fabs(sign);

	sinb2 = a.Cross(b).Mag();
	cosb2 = a.Dot(b);

	double phi = atan2(sign*sinb2, cosb2);
	if( phi < 0. ) phi = phi + 2.*TMath::Pi();

	return phi;
}

double getThetaL(const TLorentzVector& q,  const TLorentzVector& pS, const TLorentzVector& e, 
                const TLorentzVector& eS) {

	TVector3 boost;
	TLorentzVector q_boosted, pS_boosted, e_boosted, eS_boosted;
	double sinb2, cosb2;
	double sign;

	boost = (q+pS).BoostVector();
	q_boosted = q;                q_boosted.Boost(-boost);
	pS_boosted = pS;              pS_boosted.Boost(-boost);
	e_boosted = e;                e_boosted.Boost(-boost);
	eS_boosted = eS;              eS_boosted.Boost(-boost);

	boost = (e_boosted+eS_boosted).BoostVector();

	pS_boosted.Boost(-boost);
	e_boosted.Boost(-boost);
	eS_boosted.Boost(-boost);

	return pS_boosted.Vect().Angle(e_boosted.Vect());
}

bool readEvent(std::ifstream& file, size_t nParticles, Event& event) { 

	//clear
	event.m_particles.clear();

    //loop over lines
    std::string line;

	//check if any
	if(file.eof()) return false;

    while (getline(file, line)) {

        //check if contains 
        if(line.find("P") != 0) continue;

        //string stream
        std::stringstream ss;
        ss.str(line);

        //values
        std::string valueS;
        double valueD;
        std::vector<double> values(10);

        //set
        for (size_t j = 0; j < 10; j++) {

        	if(j == 0){

        		ss >> valueS;
	            values.at(j) = 0.;
        	}else{

	            ss >> valueD;
	            values.at(j) = valueD;
        	}
        }

        event.m_particles.push_back(TLorentzVector(values.at(4), values.at(5), values.at(6), values.at(7)));
    
        if(event.m_particles.size() == nParticles) {
            break;
        }
    }

    if(event.m_particles.size() != nParticles) return false;

   	return true;
}

std::pair<double, std::pair<double, double> > readNEventsAndLuminosity(std::ifstream& file) { 

	//result
	std::pair<double, std::pair<double, double> > result;

	//clear
	file.clear();
	file.seekg(0);

    //loop over lines
    std::string line;

    while (getline(file, line)) {

    	//switch
 		bool isLumi = false;
 		bool isLumiErr = false;
 		bool isNEvents = false;

        //check if contains 
        if(line.find("generated_events_number") != std::string::npos){
        	isNEvents = true;
        }
        else if(line.find("integrated_cross_section_value") != std::string::npos){
        	isLumi = true;
        }
        else if(line.find("integrated_cross_section_uncertainty") != std::string::npos){
        	isLumiErr = true;
        }
        else{
        	continue;
        }

        //string stream
        std::stringstream ss;
        ss.str(line);

        //values
        std::string valueS;
        double valueD;

        //set
        for (size_t j = 0; j < 3; j++) {

        	if(j == 0 || j == 1){

        		ss >> valueS;
        	}else{

	            ss >> valueD;

	            if(isNEvents){
	            	result.first = valueD;
	            }
	            if(isLumi){
	            	result.second.first = valueD;
	            }
	           	if(isLumiErr){
	            	result.second.second = valueD;
	            }
        	}
        }

        //check if any
		if(file.eof()) break;
    }

   	return result;
}

struct HistogramPack{

	std::string m_processName;
	std::string m_variableName;

	TH1D* m_hPARTONS;
	TH1D* m_hPARTONSEvents;
	std::vector<TH1D*> m_hEPICRef;
	std::vector<TH1D*> m_hEPIC;
	std::vector<TH1D*> m_hEPICEvents;

	bool m_isLogX;
	bool m_isLogY;
};

std::string makeName(const std::string& a, const std::string& b, const std::string& c, int i = -1){

	if(i == -1){
		return a + "_" + b + "_" + c;
	}else{

		std::stringstream ss;
		ss << i;
		return a + "_" + b + "_" + c + "_" + ss.str();
	}
}

HistogramPack makeHistogramPack(const std::string& processName, const std::string& variableName, const size_t& nPoints, const double points[][4], const std::vector<TFile*> rootFilesRef, const std::vector<TFile*> rootFiles, bool isLogY = false){

	//result
	HistogramPack result;

	//set
	result.m_processName = processName;
	result.m_variableName = variableName;

	//PARTONS

	//check if log
	result.m_isLogX = !(1. - fabs((points[0][1] - points[0][0])/(points[nPoints - 1][1] - points[nPoints - 1][0])) < 1.E-6);
	result.m_isLogY = isLogY;

	//check if linear
	if(result.m_isLogX){

		result.m_hPARTONS = new TH1D(makeName(processName, variableName, "PARTONS").c_str(), "", nPoints, log(points[0][0]), log(points[nPoints - 1][1]));
		result.m_hPARTONSEvents = new TH1D(makeName(processName, variableName, "PARTONSEvents").c_str(), "", nPoints, log(points[0][0]), log(points[nPoints - 1][1]));

		for(size_t i = 0; i < nPoints; i++){

			size_t iBin = result.m_hPARTONS->FindBin(log(points[i][0] + 0.5 * (points[i][1]) - points[i][0]));

			if(iBin == 0 || iBin == nPoints + 1){

				std::cerr << "error: " << __func__ << ": not able to find bin index" << std::endl;
				exit(1);
			}

			result.m_hPARTONS->SetBinContent(iBin, points[i][2]);
			result.m_hPARTONS->SetBinError(iBin, points[i][3]);
		}
	}else{

		result.m_hPARTONS = new TH1D(makeName(processName, variableName, "PARTONS").c_str(), "", nPoints, points[0][0], points[nPoints - 1][1]);
		result.m_hPARTONSEvents = new TH1D(makeName(processName, variableName, "PARTONSEvents").c_str(), "", nPoints, points[0][0], points[nPoints - 1][1]);

		for(size_t i = 0; i < nPoints; i++){

			size_t iBin = result.m_hPARTONS->FindBin(points[i][0] + 0.5 * (points[i][1] - points[i][0]));

			if(iBin == 0 || iBin == nPoints + 1){

				std::cerr << "error: " << __func__ << ": not able to find bin index" << std::endl;
				exit(1);
			}

			result.m_hPARTONS->SetBinContent(iBin, points[i][2]);
			result.m_hPARTONS->SetBinError(iBin, points[i][3]);
		}
	}

	result.m_hPARTONSEvents->Sumw2();

	//EPIC
	if(rootFilesRef.size() != rootFiles.size()){

		std::cerr << "error: " << __func__ << ": different number of files" << std::endl;
		exit(1);
	}

	for(size_t i = 0; i < rootFilesRef.size(); i++){

		std::string hName = "h_" + variableName;

		result.m_hEPICRef.push_back((TH1D*)rootFilesRef.at(i)->Get(hName.c_str()));
		result.m_hEPIC.push_back((TH1D*)rootFiles.at(i)->Get(hName.c_str()));

		if(result.m_hEPICRef.back() == nullptr || result.m_hEPIC.back() == nullptr){

			std::cerr << "error: " << __func__ << ": not able to read histogram, " << hName << std::endl;
			exit(1);
		}

		result.m_hEPICRef.back()->SetName(makeName(processName, variableName, "EPICRef", i).c_str());
		result.m_hEPIC.back()->SetName(makeName(processName, variableName, "EPIC", i).c_str());

		result.m_hEPICEvents.push_back((TH1D*)result.m_hEPIC.back()->Clone());
		result.m_hEPICEvents.back()->SetName(makeName(processName, variableName, "EPICEvents", i).c_str());
		result.m_hEPICEvents.back()->Reset();
	}

	//return
	return result;
}

void drawHistogramPack(HistogramPack& histogramPack, bool isLast){

	static size_t s_canCount = 0;

	TCanvas* can = new TCanvas();
	can->Divide(1, 1 + histogramPack.m_hEPIC.size());

	can->cd(1);
	if(histogramPack.m_isLogY) can->cd(1)->SetLogy();
	histogramPack.m_hPARTONS->SetMarkerStyle(20);
	histogramPack.m_hPARTONS->SetMarkerColor(2);
	if(! histogramPack.m_isLogY) histogramPack.m_hPARTONS->SetMinimum(0.);
	histogramPack.m_hPARTONS->SetStats(0);
	histogramPack.m_hPARTONS->Draw("e");
	histogramPack.m_hPARTONSEvents->Draw("same hist");

	for(size_t i = 0; i < histogramPack.m_hEPIC.size(); i++){

		can->cd(2 + i);
		if(histogramPack.m_isLogY) can->cd(2 + i)->SetLogy();
		histogramPack.m_hEPIC.at(i)->SetMarkerStyle(20);
		histogramPack.m_hEPIC.at(i)->SetMarkerColor(2);
		histogramPack.m_hEPIC.at(i)->SetMinimum((histogramPack.m_isLogY)?(1.):(0.));
		histogramPack.m_hEPIC.at(i)->SetStats(0);
		histogramPack.m_hEPIC.at(i)->Draw("e");
		histogramPack.m_hEPICRef.at(i)->SetMarkerStyle(22);
		histogramPack.m_hEPICRef.at(i)->SetMarkerColor(4);
		histogramPack.m_hEPICRef.at(i)->Draw("same e");
		histogramPack.m_hEPICEvents.at(i)->Draw("same hist");
	}

	if(isLast){
		can->Print("make_comparison.pdf)", "pdf");
	}else{

		if(s_canCount == 0){
			can->Print("make_comparison.pdf(", "pdf");
		}else{
			can->Print("make_comparison.pdf", "pdf");
		}
	}

	s_canCount++;
}

void fillHistogramPack(HistogramPack& histogramPack, const Event& event, size_t iFile, size_t nEvents, double luminosity){

	double variable;

	if(histogramPack.m_processName == "dvcs"){
		
		if(histogramPack.m_variableName == "y"){
			variable = (event.m_particles.at(3) * event.m_particles.at(2)) / (event.m_particles.at(3) * event.m_particles.at(0));
		}
		else if(histogramPack.m_variableName == "t"){
			variable = (event.m_particles.at(5) - event.m_particles.at(3)).Mag2();
		}
		else if(histogramPack.m_variableName == "Q2"){
			variable = -1 * event.m_particles.at(2).Mag2();
		}
		else if(histogramPack.m_variableName == "phi"){
			variable = getPhi(event.m_particles.at(2), event.m_particles.at(3), event.m_particles.at(0), event.m_particles.at(1), event.m_particles.at(4));
		}else{
			std::cerr << "error: " << __func__ << ": unknown variable, " << histogramPack.m_variableName << std::endl;
			exit(1);	
		}

	}
	else if(histogramPack.m_processName == "tcs"){

		if(histogramPack.m_variableName == "t"){
			variable = (event.m_particles.at(5) - event.m_particles.at(3)).Mag2();
		}
		else if(histogramPack.m_variableName == "QPrim2"){
			variable = event.m_particles.at(4).Mag2();
		}
		else if(histogramPack.m_variableName == "phiL"){
			variable = getPhiL(event.m_particles.at(4), event.m_particles.at(3), event.m_particles.at(5), event.m_particles.at(6), event.m_particles.at(7));
		}
		else if(histogramPack.m_variableName == "thetaL"){
			variable = getThetaL(event.m_particles.at(4), event.m_particles.at(5), event.m_particles.at(6), event.m_particles.at(7));
		}
		else if(histogramPack.m_variableName == "y"){
			variable = (event.m_particles.at(3) * event.m_particles.at(2)) / (event.m_particles.at(3) * event.m_particles.at(0));
		}
		else if(histogramPack.m_variableName == "Q2"){
			variable = -1 * event.m_particles.at(2).Mag2();
		}
		else{
			std::cerr << "error: " << __func__ << ": unknown variable, " << histogramPack.m_variableName << std::endl;
			exit(1);	
		}
	}
	else if(histogramPack.m_processName == "ddvcs"){

		if(histogramPack.m_variableName == "y"){
			variable = (event.m_particles.at(3) * event.m_particles.at(2)) / (event.m_particles.at(3) * event.m_particles.at(0));
		}
		else if(histogramPack.m_variableName == "t"){
			variable = (event.m_particles.at(5) - event.m_particles.at(3)).Mag2();
		}
		else if(histogramPack.m_variableName == "Q2"){
			variable = -1 * event.m_particles.at(2).Mag2();
		}
		else if(histogramPack.m_variableName == "QPrim2"){
			variable = event.m_particles.at(4).Mag2();
		}
		else if(histogramPack.m_variableName == "phi"){
			variable = getPhi(event.m_particles.at(2), event.m_particles.at(3), event.m_particles.at(0), event.m_particles.at(1), event.m_particles.at(4));
		}
		else if(histogramPack.m_variableName == "phiL"){
			variable = getPhiL(event.m_particles.at(4), event.m_particles.at(3), event.m_particles.at(5), event.m_particles.at(6), event.m_particles.at(7));
		}
		else if(histogramPack.m_variableName == "thetaL"){
			variable = getThetaL(event.m_particles.at(4), event.m_particles.at(5), event.m_particles.at(6), event.m_particles.at(7));
		}
		else{
			std::cerr << "error: " << __func__ << ": unknown variable, " << histogramPack.m_variableName << std::endl;
			exit(1);	
		}
	}
	else if(histogramPack.m_processName == "dvmp"){
		
		if(histogramPack.m_variableName == "y"){
			variable = (event.m_particles.at(3) * event.m_particles.at(2)) / (event.m_particles.at(3) * event.m_particles.at(0));
		}
		else if(histogramPack.m_variableName == "t"){
			variable = (event.m_particles.at(5) - event.m_particles.at(3)).Mag2();
		}
		else if(histogramPack.m_variableName == "Q2"){
			variable = -1 * event.m_particles.at(2).Mag2();
		}
		else if(histogramPack.m_variableName == "phi"){
			variable = getPhi(event.m_particles.at(2), event.m_particles.at(3), event.m_particles.at(0), event.m_particles.at(1), event.m_particles.at(4));
		}else{
			std::cerr << "error: " << __func__ << ": unknown variable, " << histogramPack.m_variableName << std::endl;
			exit(1);	
		}
	}
	else{
		std::cerr << "error: " << __func__ << ": unknown process, " << histogramPack.m_processName << std::endl;
		exit(1);
	}

	histogramPack.m_hPARTONSEvents->Fill(histogramPack.m_isLogX ? log(variable) : variable, 0.5*luminosity/double(nEvents));
	histogramPack.m_hEPICEvents.at(iFile)->Fill(variable);
}

double processHistograms(TH1D* h1, TH1D* h2, bool isLog){

		static int nHistograms = 0;

		TH1D* a = (TH1D*)(h1->Clone());
		a->SetName(makeName("processHistograms", "", "a", nHistograms).c_str());
		for(size_t j = 0; j <= a->GetNbinsX(); j++){
 
 			if(a->GetBinContent(j) > 10.){

				a->SetBinContent(j, log10(a->GetBinContent(j)));
				a->SetBinError(j, log10(a->GetBinError(j)));
			}else{

				a->SetBinContent(j, 0.);
				a->SetBinError(j, 0.);
			}
		}	

		TH1D* b = (TH1D*)(h2->Clone());
		b->SetName(makeName("processHistograms", "", "b", nHistograms).c_str());
		for(size_t j = 0; j <= b->GetNbinsX(); j++){

			if(b->GetBinContent(j) > 10.){

				b->SetBinContent(j, log10(b->GetBinContent(j)));
				b->SetBinError(j, log10(b->GetBinError(j)));
			}else{

				b->SetBinContent(j, 0.);
				b->SetBinError(j, 0.);
			}
		}	

		double result;

		if(isLog){
			result = a->KolmogorovTest(b);
		}else{
			result = h1->KolmogorovTest(h2);
		}

		nHistograms++;	

		return result;
}

bool processHistogramPack(HistogramPack& histogramPack){

	bool result = true;

	double t1 = histogramPack.m_hPARTONSEvents->KolmogorovTest(histogramPack.m_hPARTONS);

	std::cout << "info: " << __func__ << ": process: " << histogramPack.m_processName << ", variable: " << histogramPack.m_variableName 
		<< ", test result (PARTONS/EpIC events): " << t1 << ((t1 >= c_toleranceA)?(" PASSED"):(" FAILED")) << std::endl;

	std::cout << "info: " << __func__ << ": process: " << histogramPack.m_processName << ", variable: " << histogramPack.m_variableName 
		<< ", integral: PARTONS: " << histogramPack.m_hPARTONS->Integral() << ", EpIC: " << histogramPack.m_hPARTONSEvents->Integral() << std::endl;

	for(size_t i = 0; i < histogramPack.m_hEPIC.size(); i++){

		histogramPack.m_hEPICRef.at(i)->Scale(histogramPack.m_hEPIC.at(i)->Integral() / histogramPack.m_hEPICRef.at(i)->Integral());

		double t2 = processHistograms(histogramPack.m_hEPICRef.at(i), histogramPack.m_hEPIC.at(i), histogramPack.m_isLogY);

		std::cout << "info: " << __func__ << ": process: " << histogramPack.m_processName << ", variable: " << histogramPack.m_variableName << ", file: " << i
			<< ", test result (EpIC/EpICRef): " << t2 << ((t2 >= c_toleranceB)?(" PASSED"):(" FAILED")) << std::endl;

		result &= (t2 >= c_toleranceB);

		double t3 = processHistograms(histogramPack.m_hEPICEvents.at(i), histogramPack.m_hEPIC.at(i), histogramPack.m_isLogY);

		std::cout << "info: " << __func__ << ": process: " << histogramPack.m_processName << ", variable: " << histogramPack.m_variableName << ", file: " << i
			<< ", test result (EpIC/EpIC events): " << t3 << ((t3 >= c_toleranceC)?(" PASSED"):(" FAILED")) << std::endl;
		
		result &= (t3 >= c_toleranceC);
	}

	return result;
}

int main(int argc, char* argv[]){

	//check arguments
	if(argc < 5 || bool((argc-2)%3)){

		std::cerr << "error: " << __func__ << ": wrong number of arguments, usage: " << argv[0] << " channel histogramRefFile.root eventFile.txt histogramFile.root" << std::endl;
		exit(1);
	}

	//open root files
	std::vector<TFile*> rootFilesRef;
	std::vector<TFile*> rootFiles;

	for(size_t i = 2; i < argc; i=i+3){

		rootFilesRef.push_back(new TFile(argv[i], "READ"));
		rootFiles.push_back(new TFile(argv[i+2], "READ"));

		if(rootFilesRef.back()->IsZombie()){

			std::cerr << "error: " << __func__ << ": not able to open root file, " << argv[i] << std::endl;
			exit(1);
		}

		if(rootFiles.back()->IsZombie()){

			std::cerr << "error: " << __func__ << ": not able to open root file, " << argv[i+2] << std::endl;
			exit(1);
		}
	}

	//create histograms and decide on number of particles in event
	std::vector<HistogramPack> hists;
	size_t nParticles;

	if(std::string(argv[1]) == "dvcs"){
		
		hists.push_back(makeHistogramPack("dvcs", "y", c_dvcs_y_n, c_dvcs_y, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("dvcs", "t", c_dvcs_t_n, c_dvcs_t, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("dvcs", "Q2", c_dvcs_Q2_n, c_dvcs_Q2, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("dvcs", "phi", c_dvcs_phi_n, c_dvcs_phi, rootFilesRef, rootFiles, 0));

		nParticles = 6;
	}
	else if(std::string(argv[1]) == "tcs"){

		hists.push_back(makeHistogramPack("tcs", "t", c_tcs_t_n, c_tcs_t, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("tcs", "QPrim2", c_tcs_QPrim2_n, c_tcs_QPrim2, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("tcs", "phiL", c_tcs_phiL_n, c_tcs_phiL, rootFilesRef, rootFiles, 0));
		hists.push_back(makeHistogramPack("tcs", "thetaL", c_tcs_thetaL_n, c_tcs_thetaL, rootFilesRef, rootFiles, 0));
		hists.push_back(makeHistogramPack("tcs", "y", c_tcs_y_n, c_tcs_y, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("tcs", "Q2", c_tcs_Q2_n, c_tcs_Q2, rootFilesRef, rootFiles, 1));

		nParticles = 8;
	}
	else if(std::string(argv[1]) == "ddvcs"){

		hists.push_back(makeHistogramPack("ddvcs", "y", c_ddvcs_y_n, c_ddvcs_y, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("ddvcs", "t", c_ddvcs_t_n, c_ddvcs_t, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("ddvcs", "Q2", c_ddvcs_Q2_n, c_ddvcs_Q2, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("ddvcs", "QPrim2", c_ddvcs_QPrim2_n, c_ddvcs_QPrim2, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("ddvcs", "phi", c_ddvcs_phi_n, c_ddvcs_phi, rootFilesRef, rootFiles, 0));
		hists.push_back(makeHistogramPack("ddvcs", "phiL", c_ddvcs_phiL_n, c_ddvcs_phiL, rootFilesRef, rootFiles, 0));
		hists.push_back(makeHistogramPack("ddvcs", "thetaL", c_ddvcs_thetaL_n, c_ddvcs_thetaL, rootFilesRef, rootFiles, 0));

		nParticles = 8;
	}
	else if(std::string(argv[1]) == "dvmp"){
		
		hists.push_back(makeHistogramPack("dvmp", "y", c_dvmp_y_n, c_dvmp_y, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("dvmp", "t", c_dvmp_t_n, c_dvmp_t, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("dvmp", "Q2", c_dvmp_Q2_n, c_dvmp_Q2, rootFilesRef, rootFiles, 1));
		hists.push_back(makeHistogramPack("dvmp", "phi", c_dvmp_phi_n, c_dvmp_phi, rootFilesRef, rootFiles, 0));

		nParticles = 8;
	}
	else{
		std::cerr << "error: " << __func__ << ": unknown process, " << argv[1] << std::endl;
		exit(1);
	}

	//open text file
    std::vector<std::ifstream> eventFiles;

    for(size_t i = 3; i < argc; i=i+3){

	    eventFiles.push_back(std::ifstream(argv[i]));

	    if (!eventFiles.back().is_open()) {
	        std::cerr << __func__ << ": unable to read: " << argv[i] << std::endl;
	        exit(1);
	    }
	}

    //fill histograms
    Event event;
    size_t nEvents = 0;

    for(size_t i = 0; i < eventFiles.size(); i++){

    	std::pair<double, std::pair<double, double> > thisFile = readNEventsAndLuminosity(eventFiles.at(i));
		std::cout << "info: " << __func__ << ": file: " << i << ", number of events: " << thisFile.first << 
			", luminosity: " << thisFile.second.first << " +/- " << thisFile.second.second << std::endl;

		//clear
		eventFiles.at(i).clear();
		eventFiles.at(i).seekg(0);

	    for(;;){

	    	if(! readEvent(eventFiles.at(i), nParticles, event)) break;

	    	for(size_t j = 0; j < hists.size(); j++){
				fillHistogramPack(hists.at(j), event, i, thisFile.first, thisFile.second.first);
			}

			nEvents++;
		}

		std::cout << "info: " << __func__ << ": number of events after reading " << i << " file: " << nEvents << std::endl;
	}

	//process
	bool result = true;

	for(size_t i = 0; i < hists.size(); i++){
			result &= processHistogramPack(hists.at(i));
	}

	//print
	for(size_t i = 0; i < hists.size(); i++){
			drawHistogramPack(hists.at(i), i == (hists.size() - 1));
	}

	//close
	for(size_t i = 0; i < rootFiles.size(); i++){
		rootFiles.at(i)->Close();
	}

	for(size_t i = 0; i < eventFiles.size(); i++){
		eventFiles.at(i).close();
	}

	//return 
	std::cout << "info: " << __func__ << ": Final result: " << ((result)?("PASSED"):("FAILED")) << std::endl;

	return ((result)?(0):(2));
}
