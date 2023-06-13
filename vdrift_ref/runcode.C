{
  double x=0;
  double y=0;
  double pdg=22;
  double energy=10;
  double z=2;
  int status = system(("./vdrift_build/g4workshop "+std::to_string(x)+" "+std::to_string(y)+" "+std::to_string(z)+" "+std::to_string(pdg)+" "+std::to_string(energy)).c_str());
  // system("mv arapuca.root arapuca_muon_10MeV.root");
}
