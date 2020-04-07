#include <iostream>
#include "gp_config.h"
#include "gp_TH1.h"
#include "CMatrix.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TSystem.h"

using namespace std;



int main(int argc, char* argv[])
{


  gp_config * config = new gp_config();
  config->setkernel("rbf");

  for (auto kernel : config->kernelTypes){
    cout<<"kernel"<<kernel;
  }

  for (auto f :config->rbfInvWidths){
    cout<<"widhts: "<<f<<endl;
  }
  config->rbfInvWidths.at(0)=2.;

  for (auto f :config->rbfInvWidths){
    cout<<"widhts: "<<f<<endl;
  }

  //config->setkernel("white");
//TH1D* h1 = new TH1D("h1", "h1", 10, 0, 10);
  gp_TH1 * GP_TH1 = new gp_TH1(config);

  config->modelFileName="test.model";


  // -- first test config works, hello world
  //cout<<config->gettol()<<endl;
  //cout<<h1<<endl;
  //cout<<"hello world"<<endl;
  //
  // -- second test:  open root file and check the X and y matrix

TFile * f1 = TFile::Open("examples/landau_histogram.root");
//  TFile * f1 = TFile::Open("/eos/user/y/ywng/skimmed_output/output-2020-2-1-DL-OS/mc16a/zjets/tree_NoSys.root");

  //f1->ls();

  TH1D * hist_F = (TH1D*)f1->Get("hist");
  //TH1F * hist_F = (TH1F*)f1->Get("dimuM");
  TH1D * hist;
  TH1 * hist_1 = (TH1*)hist_F;

  hist= (TH1D*) hist_1;


  for (int i=0; i< hist->GetNbinsX(); i++){
    if (hist->GetBinContent(i)<0){
      cout<<"content<0!!!!"<<endl;
      return 1;
      
    }
  }
  TCanvas *c0= new TCanvas();
  //c0->cd();
  //
  hist->Draw();
  c0->SaveAs("histogram.pdf");

  GP_TH1->readFromTH1(hist, 35, 75);

  //GP_TH1->printXY();

  //cout<<"before learn"<<endl;
  //GP_TH1->pmodel->pkern->display(cout);
  GP_TH1->learn("testlalala");


  cout<<"after learn : "<<endl;
  //GP_TH1->pmodel->pkern->display(cout);

  TH1D* h1 = GP_TH1->fitAndOutput(true);

  cout<<"after fit and output : "<<endl;
  //GP_TH1->pmodel->pkern->display(cout);



  TFile * output_file =new TFile("output.root", "recreate");
  output_file->cd();
  hist->SetLineColor(kBlue);
  h1->SetLineColor(kRed);
  TCanvas *c1 = new TCanvas();

  c1->cd();
  c1->SetLogy();
  hist->Draw();
  h1->Draw("SAME");

  c1->SaveAs("gp_prediction.pdf");

  hist->Write();
  
  h1->Write();



  output_file->Write();
  cout<<"Number of optional param: "<<endl;
  cout<<GP_TH1->pmodel->getOptNumParams()<<endl;


  cout<<"Number of optional param: "<<endl;
  cout<<GP_TH1->pmodel->getOptNumParams()<<endl;

  //cout<<GP_TH1->pmodel->pkern->getNumParams()<<endl;

 //GP_TH1->pmodel->pkern->display(cout);

}
