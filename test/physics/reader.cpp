#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/Print.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"

#include <TH1.h>
#include <TROOT.h>
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>

#include "DVCSEvent.h"

#include <iostream>

using namespace HepMC3;

//main
int main(int argc, char **argv) {

    //input file
    if(argc != 2){

        std::cout << __func__ << ": error: usage should be: " << argv[0] << " inputFile" << std::endl;
        exit(0);
    }

    //histograms
    std::vector<TH1D*> h_Q2(10, nullptr);
    std::vector<TH1D*> h_t(10, nullptr);
    std::vector<TH1D*> h_xB(10, nullptr);
    std::vector<TH1D*> h_y(10, nullptr);
    std::vector<TH1D*> h_phi(10, nullptr);

    h_Q2[0] = new TH1D("h_Q2_00", "", 90, 1., 10.);
    h_t[0] = new TH1D("h_t_00", "", 50, 0.0, 1.0);
    h_xB[0] = new TH1D("h_xB_00", "", 50, 0.0, 1.0);
    h_y[0] = new TH1D("h_y_00", "", 50, 0.0, 1.0);
    h_phi[0] = new TH1D("h_phi_00", "", 100, 0.0, 6.2831);

//    h_Q2[0] = new TH1D("h_Q2_00", "", 90, 1., 10.);
//    h_t[0] = new TH1D("h_t_00", "", 100, 0.001, 1.0);
//    h_xB[0] = new TH1D("h_xB_00", "", 100, 0.001, 0.999);
//    h_y[0] = new TH1D("h_y_00", "", 100, 0.0, 1.0);
//    h_phi[0] = new TH1D("h_phi_00", "", 100, 0.001, 6.283);

    //open file
    ReaderAscii inputFile(argv[1]);

    //loop over event
    size_t iEvent = 0;

    while(! inputFile.failed()) {

        //event
        GenEvent evt(Units::GEV,Units::MM);

        //read event from input file
        inputFile.read_event(evt);

        //if reading failed - exit loop
        if(inputFile.failed() ) break;

        //DVCS event
        DVCSEvent dvcsEvent(evt);

        //fill
        h_Q2[0]->Fill(dvcsEvent.getQ2());
        h_t[0]->Fill(-1. * dvcsEvent.getT());
        h_xB[0]->Fill(dvcsEvent.getXb());
        h_y[0]->Fill(dvcsEvent.getY());
        h_phi[0]->Fill(dvcsEvent.getPhi());

        //id
        iEvent++;
    }

//    //run info
//    std::shared_ptr<HepMC3::GenRunInfo> runInfo = inputFile.run_info();
//    std::cout << runInfo->attributes().size() + 5 << std::endl;
//
//    //run info
//    std::shared_ptr<HepMC3::Attribute> att_csValue = runInfo->attributes().find("integrated_cross_section_value")->second;
//    double csValue = std::stod(att_csValue->unparsed_string());
//    std::cout << csValue << std::endl;
//
//    //run info
//    std::shared_ptr<HepMC3::Attribute> att_eventsNumber = runInfo->attributes().find("generated_events_number")->second;
//    double eventsNumber = std::stod(att_eventsNumber->unparsed_string());
//    std::cout << eventsNumber << std::endl;


    //close file
    inputFile.close();

//    double luminosity = eventsNumber * csValue;

//    std::cout << luminosity << std::endl;

    TGraph *gr_Q2 = new TGraph();
    gr_Q2 -> SetMarkerStyle(kFullCircle);

    fstream data;
    data.open("CS_Q2.txt",ios::in);

    double x,y;

    double normalization=0.0;

    double normalization2 = 100000 / 0.00363645;

    int i=0;

    //double normalization = (h_Q2[0]->GetBinContent(1));

    while(1)
    {
        i++;
        data >> x >> y;

        if(i==1)
            normalization=(h_Q2[0]->GetBinContent(1))/y;

        gr_Q2 -> SetPoint(gr_Q2->GetN(),x,y*normalization2);
        if(data.eof()) break;
    }

    data.close();



    TGraph *gr_t = new TGraph();
    gr_t -> SetMarkerStyle(kFullCircle);

//        fstream data;
    data.open("CS_t.txt",ios::in);

//        double x,y,normalization;

    i=0;

    //double normalization = (h_Q2[0]->GetBinContent(1));

    while(1)
    {
        i++;
        data >> x >> y;

        gr_t -> SetPoint(gr_t->GetN(),-1. * x, y*normalization2);
        if(data.eof()) break;
    }

    data.close();



    TGraph *gr_xB = new TGraph();
    gr_xB -> SetMarkerStyle(kFullCircle);

//        fstream data;
    data.open("CS_xB.txt",ios::in);

//        double x,y,normalization;

    i=0;

    //double normalization = (h_Q2[0]->GetBinContent(1));

    while(1)
    {
        i++;
        data >> x >> y;

        gr_xB -> SetPoint(gr_xB->GetN(),x,y*normalization2);
        if(data.eof()) break;
    }

    data.close();


    TGraph *gr_y = new TGraph();
    gr_y -> SetMarkerStyle(kFullCircle);

//        fstream data;
    data.open("CS_y.txt",ios::in);

//        double x,y,normalization;

    i=0;

    //double normalization = (h_Q2[0]->GetBinContent(1));

    while(1)
    {
        i++;
        data >> x >> y;

        gr_y -> SetPoint(gr_y->GetN(),x,y*normalization2);
        if(data.eof()) break;
    }

    data.close();


    TGraph *gr_phi = new TGraph();
    gr_phi -> SetMarkerStyle(kFullCircle);

//        fstream data;
    data.open("CS_phi.txt",ios::in);

//        double x,y,normalization;

    i=0;

    //double normalization = (h_Q2[0]->GetBinContent(1));

    while(1)
    {
        i++;
        data >> x >> y;

        gr_phi -> SetPoint(gr_phi->GetN(),x,y*normalization2);
        if(data.eof()) break;
    }

    data.close();


    //print
//  TCanvas* can_Q2 = new TCanvas("can_Q2");
//  can_Q2->Divide(2, 1);
//  can_Q2->cd(1);
//  can_Q2->cd(1)->SetLogy();
//  h_Q2[0]->SetMinimum(1);
//  h_Q2[0]->Draw();
//  can_Q2->cd(2);
//  can_Q2->cd(2)->SetLogy();
//  h_Q2[1]->SetMinimum(1);
//  h_Q2[1]->Draw();
//  can_Q2->Print("plots.pdf", "pdf");

    TCanvas* can_all = new TCanvas("can_all");
    can_all->Divide(3, 2);

    can_all->cd(1);
    can_all->cd(1)->SetLogy();
    h_Q2[0]->SetMinimum(1);
    h_Q2[0]->Draw();
    gr_Q2->Draw("L");

    can_all->cd(2);
    can_all->cd(2)->SetLogy();
    h_t[0]->SetMinimum(1);
    h_t[0]->Draw();
    gr_t->Draw("L");

    can_all->cd(3);
    can_all->cd(3)->SetLogx();
    can_all->cd(3)->SetLogy();
    h_xB[0]->SetMinimum(1);
    h_xB[0]->Draw();
    gr_xB->Draw("L");

    can_all->cd(4);
    can_all->cd(4)->SetLogy();
    h_y[0]->SetMinimum(1);
    h_y[0]->Draw();
    gr_y->Draw("L");

    can_all->cd(5);
    can_all->cd(5)->SetLogy();
    h_phi[0]->SetMinimum(1);
    h_phi[0]->Draw();
    gr_phi->Draw("L");


    can_all->Print("plots_all.pdf", "pdf");

    //return
    return 0;
}
