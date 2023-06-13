#define MARLEYg4_cxx
#include "MARLEYg4.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <stdlib.h>

void MARLEYg4::Loop()
{
  double Nph=0;
  int pdg=0;
  double energy=0;
  double x=0;
  double y=0;
  double z=2;

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<1;jentry++) {
  //  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    Nph=0;
    x=0;
    y=0;
    z=2;

     for(int i=0;i<np;i++){

      pdg=pdgp[i];
      energy=KEp[i];
      // int status = system("./vdrift_build/g4workshop 0 0 2 $pdg $energy");
      int status = system(("./vdrift_build/g4workshop "+std::to_string(x)+" "+std::to_string(y)+" "+std::to_string(z)+" "+std::to_string(pdg)+" "+std::to_string(energy)).c_str());

      TFile f("arapuca.root");
      TH1D *hist=(TH1D*)f.Get("hv");
      Nph=Nph+hist->Integral(5, 164)+hist->Integral(165, 484)+hist->Integral(637, 676);
      std::cout<<"PDGcode vs Nph "<<pdgp[i]<<"  "<<Nph<<std::endl;
    }





  }

  std::cout<<"Congratulations!! Code finished running."<<std::endl;
}
