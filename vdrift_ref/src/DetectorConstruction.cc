// Light simulation in Iceberg                                                                                                                               
// Authors: L. Paulucci & F. Marinho                                                                                                                         
// Date: 28th September 2020                                                                                                                                 
//                                                                                                                                                           
// Added modifications should be reported in arapuca.cc header comments                                                                                      

#include "DetectorConstruction.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4NistManager.hh"

#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include <string>

DetectorConstruction::DetectorConstruction()
  :fDefaultMaterial(NULL),
   fPhysiWorld(NULL),fLogicWorld(NULL),fSolidWorld(NULL),
   fPhysiVol(NULL),fLogicVol(NULL),fSolidVol(NULL)
{
  fWorldSizeX=15.8; //in meters                                                                                                                              
  fWorldSizeY=10.0; //in meters                                                                                                                              
  fWorldSizeZ=65.0;

  fCryostat_x = 14.8; //in meters                                                                                                                            
  fCryostat_y = 6.5; //in meters                                                                                                                             
  fCryostat_z = 60.0; //in meters                                                                                                                             
  newfCryostat_x = 15.1; //in meters
                                           
  newfCryostat_y = 8.5; //in meters                    
  
  newfCryostat_z = 62.0; //in meters     

  fFC_x = 13.5; //in meters                                                                                                                                  
  fFC_y = fCryostat_y; //in meters                                                                                                                           
  fFC_z = fCryostat_z; //in meters 

  fCathode_x = 13.5; //Cathode size in m                                                                                                                     
  fCathode_z = 60.0; //Cathode size in m                                                         
  fLatY = 6.5; //APA internal size in cm                                                                                                                     
  fLatZ = 60.0;

  fthickness=0.10; //m
  fAPA_thickness = 0.005; //m -- Temporarily using for FC
  fwindow = 0.6; //Arapuca window size in m
    
  fFC_z = fCryostat_z - fthickness;
}

DetectorConstruction::DetectorConstruction(double size)
  :fDefaultMaterial(NULL),
   fPhysiWorld(NULL),fLogicWorld(NULL),fSolidWorld(NULL),
   fPhysiVol(NULL),fLogicVol(NULL),fSolidVol(NULL)
{//  fWorldSizeX=Y=fWorldSizeZ=0;
  //  fsize = size;
}

DetectorConstruction::~DetectorConstruction()
{delete fDefaultMaterial;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{DefineMaterials();return ConstructLine();}

void DetectorConstruction::DefineMaterials()
{
  G4String name, symbol;
  G4double density;

  G4int natoms,nel;
  G4double z;

  // Define Elements
  G4Element*   H  = new G4Element ("Hydrogen","H",1.,1.01*g/mole);
  G4Element*   C = new G4Element ("Carbon","C",6.,12.01*g/mole);
  G4Element*   O = new G4Element ("Oxygen","O",8.,16.0*g/mole);
  G4Element*  Al = new G4Element(name="Aluminium",symbol="Al",z=13.,26.98*g/mole);
  G4Element*  Fe = new G4Element(name="Iron",symbol="Fe",z=26.,55.85*g/mole);
  G4Element*  Ni = new G4Element(name="Niquel",symbol="Ni",z=28.,58.6934*g/mole);
G4Element*  Si = new G4Element(name="Silicon",symbol="Si",z=14.,28.085*g/mole);
G4Element*  Cr = new G4Element(name="Chromium",symbol="Cr",z=24.,51.9961*g/mole);
G4NistManager * man = G4NistManager::Instance();

G4Material* StainlessSteel = new G4Material(name="StainlessSteel",7.93*g/cm3,nel=4);//STEEL_STAINLESS_Fe7Cr2Ni
StainlessSteel->AddElement(C, 0.0010);
StainlessSteel->AddElement(Cr, 0.1792);
StainlessSteel->AddElement(Fe, 0.7298);
StainlessSteel->AddElement(Ni, 0.0900);

G4Material* G10 = new G4Material(name="G10",1.7*g/cm3,nel=4);
G10->AddElement(Si, 0.2805);
G10->AddElement(O, 0.3954);
G10->AddElement(C, 0.2990);
G10->AddElement(H, 0.0251);

G4Material* Aluminium = new G4Material(name="Aluminium",z=13.,26.98*g/mole,2.7*g/cm3);
G4Material* base_mat = man->FindOrBuildMaterial("G4_TEFLON");
G4Material* env_mat = man->FindOrBuildMaterial("G4_lAr");

// const G4int nEntries = 6;
// G4double PhotonEnergy[nEntries] =
//   { 2.0*eV, 2.341*eV, 2.757*eV, 3.353*eV, 4.136*eV, 10.0*eV };
// G4double l_lAr[nEntries] =
//   {50*m,  50*m,  50*m,  50*m, 50*m, 50*m};

const G4int nEntries = 8;
//_________ RELEVANT ENERGY VALUES Xe 175nm -> 7.08eV; Ar 128 -> 9.69eV_________
G4double PhotonEnergy[nEntries] = {2.5*eV, 5.0*eV, 7.0*eV, 7.5*eV, 8.0*eV, 9.0*eV, 9.5*eV, 10.136*eV};
G4double l_lAr[nEntries] =        {80*m,     80*m,   80*m,   80*m,   20*m,   20*m,   20*m,      20*m};

//  G4double n_lAr[nEntries] = {1.3,1.3,1.3,1.3,1.3};
G4double n_lAr[72] = {1.2310665394518083, 1.2311200318147684, 1.2312727482978456, 1.2313496348295065, 1.2313506914097514, 1.2314023012909403, 1.2321538166889967, 1.231909947460871, 1.2321384938953646, 1.2323923169803301, 1.232443926861519, 1.2328668458034384, 1.233095392237932, 1.233053736213583, 1.233509772502325, 1.2335361057330418, 1.2334866090123424, 1.2338668153496684, 1.2343481282888824, 1.2344502914710154, 1.2346282846045646, 1.2350337675923626, 1.2354898038811046, 1.2358874461725522, 1.2361238333033957, 1.2364787629902494, 1.2371038951359457, 1.2374666655191495, 1.237720488604115, 1.2384292913975776, 1.2388853276863196, 1.2393413639750617, 1.2402271033218288, 1.2409106294648191, 1.2417458155106422, 1.2425810015564651, 1.2435678475051206, 1.2446558000556642, 1.245844859208096, 1.2472614082147766, 1.2488296171242892, 1.2505242092861624, 1.2525221212537003, 1.2548739063278473, 1.257579564508603, 1.2607654790483278, 1.2644822032479663, 1.269108886864599, 1.2747719131505861, 1.2816987719601767, 1.2910690403154002, 1.302368759656658, 1.316713517166515, 1.3329411664124329, 1.3493804523854165, 1.365926455672306, 1.3820018039345536, 1.4010549636291998, 1.423580491960391, 1.442600693158671, 1.46103487273757, 1.4894895758980782, 1.508978836972031, 1.5300372520419365, 1.5496431051408859, 1.5737334642006038, 1.5996151986767673, 1.6187496758472137, 1.6361783106957755, 1.6540241603412935, 1.672490509546729, 1.69156768392026};
G4double Energy_n_lar[72] = {1.88901692613609*eV, 1.915491549763*eV, 1.94434453495496*eV, 1.9740800365654*eV, 2.00473917327901*eV, 2.03636565851099*eV, 2.0690060083582*eV, 2.1027097698752*eV, 2.1375297720313*eV, 2.17352240202191*eV, 2.21074790997382*eV, 2.24927074550798*eV, 2.28915993011439*eV, 2.33048946986518*eV, 2.37333881365753*eV, 2.41779336295621*eV, 2.46394503991691*eV, 2.51189292184303*eV, 2.56174395119099*eV, 2.61361373183203*eV, 2.66762742404865*eV, 2.72392075285046*eV, 2.78264114670945*eV, 2.84394902682875*eV, 2.90801927068415*eV, 2.97504287795441*eV, 3.04522887226339*eV, 3.11880647861692*eV, 3.19602762431763*eV, 3.27716982084588*eV, 3.36253949617556*eV, 3.45247586185779*eV, 3.54735541774646*eV, 3.64759722049586*eV, 3.75366907130329*eV, 3.86609481562225*eV, 3.98546299517591*eV, 4.11243715385428*eV, 4.24776817847434*eV, 4.39230915909235*eV, 4.54703339014745*eV, 4.71305631519082*eV, 4.89166246132417*eV, 5.08433873911506*eV, 5.29281593505014*eV, 5.51912084854449*eV, 5.76564240171605*eV, 6.03521629502331*eV, 6.33123457629323*eV, 6.65778911807759*eV, 7.01986191167459*eV, 7.42358102512033*eV, 7.80768693883725*eV, 8.18166617653237*eV, 8.49578904787588*eV, 8.75179837892604*eV, 8.97145190403002*eV, 9.21112941189563*eV, 9.40734062594689*eV, 9.56190983757659*eV, 9.71915769371575*eV, 9.88023992289941*eV, 9.99798584772925*eV, 10.0930619775564*eV, 10.1644798618311*eV, 10.2587258039568*eV, 10.3321720936895*eV, 10.3923239488175*eV, 10.4560982471211*eV, 10.4827908882082*eV, 10.5288181909777*eV, 10.5783779318939*eV};
G4double ray_e_lAr[21] = { 1.18626*eV, 1.68626*eV, 2.18626*eV, 2.68626*eV, 3.18626*eV, 3.68626*eV, 4.18626*eV, 4.68626*eV, 5.18626*eV, 5.68626*eV, 6.18626*eV, 6.68626*eV, 7.18626*eV, 7.68626*eV, 8.18626*eV, 8.68626*eV, 9.18626*eV, 9.68626*eV, 10.1863*eV, 10.6863*eV, 11.1863*eV};
G4double ray_s_lAr[21] = { 1200800*cm, 390747*cm, 128633*cm, 54969.1*cm, 27191.8*cm, 14853.7*cm, 8716.9*cm, 5397.42*cm, 3481.37*cm, 2316.51*cm, 1577.63*cm, 1092.02*cm, 763.045*cm, 534.232*cm, 371.335*cm, 252.942*cm, 165.38*cm, 99.9003*cm, 51.2653*cm, 17.495*cm, 0.964341*cm };

G4MaterialPropertiesTable* lAr_pt = new G4MaterialPropertiesTable();
lAr_pt->AddProperty("RINDEX", Energy_n_lar, n_lAr, 72);
lAr_pt->AddProperty("RAYLEIGH", ray_e_lAr, ray_s_lAr, 21);
lAr_pt->AddProperty("ABSLENGTH", PhotonEnergy, l_lAr, nEntries);
env_mat->SetMaterialPropertiesTable(lAr_pt);

///Adding extra lines to include scintillation photons
 const G4int nFast = 1;
 G4double ScintEnergy[nFast] = {9.76253*eV};
 G4double ScintFast[nFast] = {1.0};

 lAr_pt->AddProperty("FASTCOMPONENT", ScintEnergy, ScintFast, nFast);

 lAr_pt->AddConstProperty("SCINTILLATIONYIELD", 24000./MeV);
 lAr_pt->AddConstProperty("RESOLUTIONSCALE", 1.);
 lAr_pt->AddConstProperty("FASTTIMECONSTANT", 20.*ns);
 lAr_pt->AddConstProperty("YIELDRATIO", 1.);

 /////////////////////////////////////////////////



const G4int num = 3;
G4double n_teflon[num] = {1.41, 1.41, 1.41};
G4double l_teflon[num] = {3448*m,  4082*m, 9174*m};
G4double pp[num] = {1.9587*eV, 7.5407*eV, 10.1328*eV};

G4MaterialPropertiesTable*  Teflon_pt = new G4MaterialPropertiesTable();
Teflon_pt->AddProperty("RINDEX", pp, n_teflon, num);
Teflon_pt->AddProperty("ABSLENGTH", pp, l_teflon, num);

G4double l_fc[num] = {6.5*fAPA_thickness*m, 6.5*fAPA_thickness*m, 6.5*fAPA_thickness*m};
G4MaterialPropertiesTable*  fc_pt = new G4MaterialPropertiesTable();
fc_pt->AddProperty("RINDEX", Energy_n_lar, n_lAr, 72);
fc_pt->AddProperty("ABSLENGTH", pp, l_fc, num);

//Aluminium->SetMaterialPropertiesTable(fc_pt);
base_mat->SetMaterialPropertiesTable(Teflon_pt);

G4Material* acrylic = new G4Material("acrylic", 1.18*g/cm3,3); //acrylic
acrylic->AddElement (C, 5);
acrylic->AddElement (O, 2);
acrylic->AddElement (H, 8);

fDefaultMaterial = env_mat;
fBase = base_mat;
facrylic = acrylic;
fSteel = StainlessSteel;
fAluminium = Aluminium;
fG10 = G10;
// DISPLAY MATERIALS
G4cout << G4endl << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* DetectorConstruction::ConstructLine()
{
// WORLD
//  fWorldSizeXY  = 2*m;
//  fWorldSizeZ   = 2*m;
//*************
// WORLD VOLUME
//*************
fSolidWorld = new G4Box("World",fWorldSizeX/2*m,fWorldSizeY/2*m,fWorldSizeZ/2*m);
//its name; its size
fLogicWorld = new G4LogicalVolume(fSolidWorld,fDefaultMaterial,"World");
//its solid; its material; its name
fPhysiWorld = new G4PVPlacement(0,G4ThreeVector(),"World",fLogicWorld,NULL,false,0);
//no rotation; (0,0,0); its name; its logical volume; its mother volume; no boolean operation; copy number

G4Box* fSolidCryostat = new G4Box("Cryostat",(newfCryostat_x/2)*m, (newfCryostat_y/2.0+0.1)*m,(newfCryostat_z/2)*m); //make it a little bigger to avoid overlaps
G4LogicalVolume* fLogicCryostat = new G4LogicalVolume(fSolidCryostat,fDefaultMaterial,"Cryostat");
G4VPhysicalVolume* fPhysCryostat = new G4PVPlacement(0,G4ThreeVector(0,0,0),"Cryostat",
  fLogicCryostat,     //its logical volume
  fPhysiWorld,    //its mother  volume
  false,//no boolean operation
  0,
  true); //check for overlaps

G4Box* ShellOut = new G4Box("ShellOut",(newfCryostat_x/2+0.3)*m, (newfCryostat_y/2.0+0.3)*m,(newfCryostat_z/2+0.3)*m);
G4SubtractionSolid* fShell = new G4SubtractionSolid("Shell", ShellOut, fSolidCryostat);
G4LogicalVolume* fLogicShell = new G4LogicalVolume(fShell,fSteel,"Sheel");
G4VPhysicalVolume* fPhysShell = new G4PVPlacement(0,G4ThreeVector(0,0,0),"Sheel",
  fLogicShell,     //its logical volume
  fPhysiWorld,    //its mother  volume
  false,//no boolean operation
  0);

G4Box* fSolidCathode = new G4Box("Cathode",(fCathode_x/2)*m,fthickness/2*m,(fCathode_z/2)*m);
G4LogicalVolume* fLogicCathode = new G4LogicalVolume(fSolidCathode,fSteel,"Cathode");
G4VPhysicalVolume* fPhysiCathode = new G4PVPlacement(0,G4ThreeVector(0,(-fCryostat_y/2.0-fthickness/2)*m,0),"Cathode",
  fLogicCathode,     //its logical volume
  fPhysCryostat,    //its mother  volume
  false,//no boolean operation
  0,
  true); //check for overlaps
G4LogicalVolume* fLogicAnode = new G4LogicalVolume(fSolidCathode,fSteel,"Anode");
G4VPhysicalVolume* fPhysiAnode = new G4PVPlacement(0,G4ThreeVector(0,(fCryostat_y/2.0+fthickness/2)*m,0),"Anode",
  fLogicAnode,     //its logical volume
  fPhysCryostat,    //its mother  volume
  false,//no boolean operation
  0,
  true); //check for overlaps

//FC Structure

//Longer lateral
G4double ypos=-fCryostat_y/2.0 + 0.04; //in m
G4EllipticalTube* FCAuxwide = new G4EllipticalTube("FCAuxwide",5.*mm, 23.*mm,fFC_z/2*m);
G4Box* FCAuxOut = new G4Box("FCAuxOut",5.*mm,23*mm,(fFC_z/2+1)*m);
G4SubtractionSolid* fSolidFCwide = new G4SubtractionSolid("FieldCageWide", FCAuxwide, FCAuxOut, 0, G4ThreeVector((2.+5/2)*mm,0.,0.));
G4LogicalVolume* fLogicFCwide = new G4LogicalVolume(fSolidFCwide,fAluminium,"FieldCageWide");
G4VPhysicalVolume* fPhysFCwide = new G4PVPlacement(0,G4ThreeVector(fFC_x/2.*m,ypos*m,0),"FieldCageWide",
  fLogicFCwide,     //its logical volume
  fPhysCryostat,    //its mother  volume
  false,//no boolean operation
  0, true);

G4RotationMatrix* r = new G4RotationMatrix();
G4ThreeVector* axis = new G4ThreeVector(0.0,0.0,1.0);
r->rotate(CLHEP::pi,axis);

G4PVPlacement* ph_cp2  = new G4PVPlacement(r,G4ThreeVector(-fFC_x/2.*m,ypos*m,0), "FieldCageWide", fLogicFCwide, fPhysCryostat, false, 1, true);

for(int i = 2; i<=50; i++){ //49 copies of FCwide (~ 2m)
ypos+=0.06;
G4PVPlacement* ph_cp  = new G4PVPlacement(0,G4ThreeVector(fFC_x/2.*m,ypos*m,0),
  "FieldCageWide", fLogicFCwide, fPhysCryostat, false, i, true);
ph_cp2  = new G4PVPlacement(r,G4ThreeVector(-fFC_x/2.*m,ypos*m,0),
  "FieldCageWide", fLogicFCwide, fPhysCryostat, false, i+107, true);
}

ypos+=0.06;
G4EllipticalTube* FCAuxSlim = new G4EllipticalTube("FCAuxSlim",5.*mm, 7.5*mm,fFC_z/2*m);
G4Box* FCAuxSlimOut = new G4Box("FCAuxSlimOut",5.*mm,7.5*mm,(fFC_z/2+1)*m);
G4SubtractionSolid* fSolidFCSlim = new G4SubtractionSolid("FieldCageSlim", FCAuxSlim, FCAuxSlimOut, 0, G4ThreeVector((2.+5/2)*mm,0.,0.));
G4LogicalVolume* fLogicFCSlim = new G4LogicalVolume(fSolidFCSlim,fAluminium,"FieldCageSlim");
G4VPhysicalVolume* fPhysFCSlim = new G4PVPlacement(0,G4ThreeVector(fFC_x/2.*m,ypos*m,0),"FieldCageSlim",
  fLogicFCSlim,     //its logical volume
  fPhysCryostat,    //its mother  volume
  false,//no boolean operation
  0, true);
ph_cp2  = new G4PVPlacement(r,G4ThreeVector(-fFC_x/2.*m,ypos*m,0), "FieldCageSlim", fLogicFCSlim, fPhysCryostat, false, 51, true);

for(int i = 52; i<=107; i++){ //63 copies of FCSlim
ypos+=0.06;
G4PVPlacement* ph_cp  = new G4PVPlacement(0,G4ThreeVector(fFC_x/2.*m,ypos*m,0),
  "FieldCageSlim", fLogicFCSlim, fPhysCryostat, false, i, true);
ph_cp2  = new G4PVPlacement(r,G4ThreeVector(-fFC_x/2.*m,ypos*m,0),
  "FieldCageSlim", fLogicFCSlim, fPhysCryostat, false, i+107, true);
}

//Shorter laterals
ypos=-fCryostat_y/2.0 + 0.04; //in m
G4RotationMatrix* rSh = new G4RotationMatrix();
G4ThreeVector* axisSh = new G4ThreeVector(0.0,1.0,0.0);
rSh->rotate(CLHEP::pi/2,axisSh);
G4RotationMatrix* rSh2 = new G4RotationMatrix();
rSh2->rotate(-1.*CLHEP::pi/2,axisSh);

G4EllipticalTube* FCAuxShort = new G4EllipticalTube("FCAuxShort",5.*mm, 23.*mm,(fFC_x/2.-0.005)*m);
G4Box* FCAuxShOut = new G4Box("FCAuxShOut",5.*mm,23.*mm,(fFC_x/2.+1.)*m);
G4SubtractionSolid* fSolidFCShort = new G4SubtractionSolid("FieldCageShort", FCAuxShort, FCAuxShOut, 0, G4ThreeVector((2.+5./2.)*mm,0.,0.));
G4LogicalVolume* fLogicFCShort = new G4LogicalVolume(fSolidFCShort,fAluminium,"FieldCageShort");
G4VPhysicalVolume* fPhysFCShort = new G4PVPlacement(rSh,G4ThreeVector(0,ypos*m,fFC_z/2.*m),"FieldCageShort",
						    fLogicFCShort,     //its logical volume
						    fPhysCryostat,    //its mother  volume
						    false,//no boolean operation
						    0, true);

G4PVPlacement* sh_cp2  = new G4PVPlacement(rSh2,G4ThreeVector(0,ypos*m,-fFC_z/2*m),"FieldCageShort", fLogicFCShort, fPhysCryostat, false, 1, true);
    
for(int i = 2; i<=50; i++){ //49 copies of FCShort
  ypos+=0.06;
  G4PVPlacement* sh_cp  = new G4PVPlacement(rSh,G4ThreeVector(0,ypos*m,fFC_z/2*m),
					    "FieldCageShort", fLogicFCShort, fPhysCryostat, false, i, true);
  sh_cp2  = new G4PVPlacement(rSh2,G4ThreeVector(0,ypos*m,-fFC_z/2*m),
			      "FieldCageShort", fLogicFCShort, fPhysCryostat, false, i+107, true);
 }

ypos+=0.06;
G4EllipticalTube* FCAuxShortSlim = new G4EllipticalTube("FCAuxShortSlim",5.*mm, 7.5*mm,(fFC_x/2.-3.4)*m);
G4Box* FCAuxShortSlimOut = new G4Box("FCAuxShortSlimOut",5.*mm,7.5*mm,(fFC_x/2.-3.4 + (fFC_x/2.-3.4)*2./fFC_x)*m);
G4SubtractionSolid* fSolidFCShortSlim = new G4SubtractionSolid("FieldCageShortSlim", FCAuxShortSlim, FCAuxShortSlimOut, 0, G4ThreeVector((2.+5/2)*mm,0.,0.));
G4LogicalVolume* fLogicFCShortSlim = new G4LogicalVolume(fSolidFCShortSlim,fAluminium,"FieldCageShortSlim");
G4VPhysicalVolume* fPhysFCShortSlim = new G4PVPlacement(rSh,G4ThreeVector(0,ypos*m,fFC_z/2.*m),"FieldCageShortSlim",
							fLogicFCShortSlim,     //its logical volume
							fPhysCryostat,    //its mother  volume
							false,//no boolean operation
							0, true);
sh_cp2  = new G4PVPlacement(rSh2,G4ThreeVector(0,ypos*m,-fFC_z/2*m), "FieldCageShortSlim", fLogicFCShortSlim, fPhysCryostat, false, 51, true);
  
G4EllipticalTube* FCAuxShortaux = new G4EllipticalTube("FCAuxShortaux",5.*mm, 23.*mm,(3.4/2-0.005)*m);
G4Box* FCAuxShOutaux = new G4Box("FCAuxShOutaux",5.*mm,23.*mm,(3.4/2 + 3.4/fFC_x)*m);
G4SubtractionSolid* fSolidFCShortaux = new G4SubtractionSolid("FieldCageShortaux", FCAuxShortaux, FCAuxShOutaux, 0, G4ThreeVector((2.+5./2.)*mm,0.,0.));
G4LogicalVolume* fLogicFCShortaux = new G4LogicalVolume(fSolidFCShortaux,fAluminium,"FieldCageShortaux");
G4VPhysicalVolume* fPhysFCShortaux = new G4PVPlacement(rSh,G4ThreeVector(((fFC_x/2.-3.4)+3.4/2)*m,ypos*m,fFC_z/2.*m),"FieldCageShortaux",
						       fLogicFCShortaux,     //its logical volume
						       fPhysCryostat,    //its mother  volume
						       false,//no boolean operation
						       0, true);
    
G4PVPlacement* sh_cp4  = new G4PVPlacement(rSh2,G4ThreeVector(((fFC_x/2.-3.4)+3.4/2)*m,ypos*m,-fFC_z/2*m),"FieldCageShortaux", fLogicFCShortaux, fPhysCryostat, false, 215, true);
    
G4EllipticalTube* FCAuxShortaux2 = new G4EllipticalTube("FCAuxShortaux2",5.*mm, 23.*mm,(3.4/2-0.005)*m);
G4Box* FCAuxShOutaux2 = new G4Box("FCAuxShOutaux2",5.*mm,23.*mm,(3.4/2 + 3.4/fFC_x)*m);
G4SubtractionSolid* fSolidFCShortaux2 = new G4SubtractionSolid("FieldCageShortaux2", FCAuxShortaux2, FCAuxShOutaux2, 0, G4ThreeVector((2.+5./2.)*mm,0.,0.));
G4LogicalVolume* fLogicFCShortaux2 = new G4LogicalVolume(fSolidFCShortaux2,fAluminium,"FieldCageShortaux2");
G4VPhysicalVolume* fPhysFCShortaux2 = new G4PVPlacement(rSh,G4ThreeVector(((-fFC_x/2.+3.4)-3.4/2)*m,ypos*m,fFC_z/2.*m),"FieldCageShortaux2",
							fLogicFCShortaux2,     //its logical volume
							fPhysCryostat,    //its mother  volume
							false,//no boolean operation
							0, true);
      
G4PVPlacement* sh_cp6  = new G4PVPlacement(rSh2,G4ThreeVector(((-fFC_x/2.+3.4)-3.4/2)*m,ypos*m,-fFC_z/2*m),"FieldCageShortaux2", fLogicFCShortaux2, fPhysCryostat, false, 328, true);
  
for(int i = 52; i<=107; i++){ //56 copies of FCShortSlim
  ypos+=0.06;
  G4PVPlacement* sh_cp  = new G4PVPlacement(rSh,G4ThreeVector(0,ypos*m,fFC_z/2.*m),
					    "FieldCageShortSlim", fLogicFCShortSlim, fPhysCryostat, false, i, true);
  sh_cp2  = new G4PVPlacement(rSh2,G4ThreeVector(0,ypos*m,-fFC_z/2.*m),
			      "FieldCageShortSlim", fLogicFCShortSlim, fPhysCryostat, false, i+107, true);
      
  G4PVPlacement* sh_cp3  = new G4PVPlacement(rSh,G4ThreeVector(((fFC_x/2.-3.4)+3.4/2)*m,ypos*m,fFC_z/2*m),
					     "FieldCageShortaux", fLogicFCShortaux, fPhysCryostat, false, i+215-51, true);
  sh_cp4  = new G4PVPlacement(rSh2,G4ThreeVector(((fFC_x/2.-3.4)+3.4/2)*m,ypos*m,-fFC_z/2*m),
			      "FieldCageShortaux", fLogicFCShortaux, fPhysCryostat, false, i+56+215-51, true);
      
  G4PVPlacement* sh_cp5  = new G4PVPlacement(rSh,G4ThreeVector(((-fFC_x/2.+3.4)-3.4/2)*m,ypos*m,fFC_z/2*m),
					     "FieldCageShortaux2", fLogicFCShortaux2, fPhysCryostat, false, i+328-51, true);
  sh_cp6  = new G4PVPlacement(rSh2,G4ThreeVector(((-fFC_x/2.+3.4)-3.4/2)*m,ypos*m,-fFC_z/2*m),
			      "FieldCageShortaux2", fLogicFCShortaux2, fPhysCryostat, false, i+56+328-51, true);
 }


    
//________________________________MEMBRANE ARAPUCAS SHIELDS____________________________________________//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
G4double ArapucaOut_y = 0.65;
G4double ArapucaOut_x = 0.025;
G4double ArapucaOut_z = 0.65;
G4double ArapucaAcceptanceWindow_x = 0.01;//m
G4double DistFromCryoWall = 0.1; //m
int ncol=20, nrows=4;
double dz=60.0/ncol;
double zpos, xpos, yposBot;
  
G4Box* ArapucaOut = new G4Box("ArapucaOut",ArapucaOut_x/2*m,ArapucaOut_y/2*m,ArapucaOut_z/2*m);
G4Box* ArapucaIn = new G4Box("ArapucaIn",ArapucaOut_x/2*m,fwindow/2*m,fwindow/2*m);
G4SubtractionSolid* ArapucaWalls = new G4SubtractionSolid("ArapucaWalls", ArapucaOut, ArapucaIn, 0, G4ThreeVector(ArapucaOut_x/2*m,0.,0.));
G4LogicalVolume* fLogicAraWalls = new G4LogicalVolume(ArapucaWalls,facrylic,"ArapucaWalls");
G4RotationMatrix* rWall = new G4RotationMatrix();
G4ThreeVector* axisWall = new G4ThreeVector(0.0,1.0,0.0);
rWall->rotate(CLHEP::pi,axisWall);
xpos=newfCryostat_x/2.0-DistFromCryoWall-ArapucaOut_x/2.0;
    
G4VPhysicalVolume* physAraWall = new G4PVPlacement(rWall,G4ThreeVector(xpos*m,(fCryostat_y/2-0.5)*m,(-fCryostat_z/2+dz/2.0)*m),"ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,0, true);
G4VPhysicalVolume* physAraWall2 = new G4PVPlacement(0,G4ThreeVector(-xpos*m,(fCryostat_y/2-0.5)*m,(-fCryostat_z/2+dz/2.0)*m),"ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,0, true);

for(int i=1; i<ncol; i++){ //copies of ArapucaWalls on each side (first row)
  G4PVPlacement* arawall_cp  = new G4PVPlacement(rWall,G4ThreeVector(xpos*m,(fCryostat_y/2-0.5)*m,(-fCryostat_z/2+dz/2.0+i*dz)*m), "ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,i, true);
  G4PVPlacement* arawall_cp2  = new G4PVPlacement(0,G4ThreeVector(-xpos*m,(fCryostat_y/2-0.5)*m,(-fCryostat_z/2+dz/2.0+i*dz)*m),"ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,i, true);
 }
G4int ct=ncol; //count of copies for arapucawalls
for(int i=1; i<ncol; i++){ //19 copies of ArapucaWalls on each side (first row)
  G4PVPlacement* arawall_cp  = new G4PVPlacement(rWall,G4ThreeVector(xpos*m,(fCryostat_y/2-0.5)*m,
								     (-fCryostat_z/2+1.5+i*3.0)*m), "ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,i);//, true);
  G4PVPlacement* arawall_cp2  = new G4PVPlacement(0,G4ThreeVector(-xpos*m,(fCryostat_y/2-0.5)*m,
								  (-fCryostat_z/2+1.5+i*3.0)*m),  "ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,i);//, true);
 }
ct=20; //count of copies for arapucawalls
for(int i=1; i<=3; i++){ //other copies of ArapucaWalls on each side (2-4 rows)
  for(int j=0; j<20;j++){
    G4PVPlacement* arawallextra_cp  = new G4PVPlacement(rWall,G4ThreeVector(xpos*m,(fCryostat_y/2-0.5-0.8*i)*m,
									    (-fCryostat_z/2+1.5+j*3.0)*m), "ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,ct);//, true);
    G4PVPlacement* arawallextra_cp2  = new G4PVPlacement(0,G4ThreeVector(-xpos*m,(fCryostat_y/2-0.5-0.8*i)*m,
									 (-fCryostat_z/2+1.5+j*3.0)*m), "ArapucaWalls", fLogicAraWalls, fPhysCryostat, false,ct);//, true);
    ct++;
  }
 }

//ARAPUCAs
G4Box* AraWindowLat = new G4Box("ArapucaWindow",ArapucaAcceptanceWindow_x/2*m,fwindow/2*m,fwindow/2*m);
G4LogicalVolume* fLogicAraWindowLat = new G4LogicalVolume(AraWindowLat,facrylic,"ArapucaWindow");

ncol=20, nrows=4;
std::string name, physname, name2, physname2;
xpos=newfCryostat_x/2.0-DistFromCryoWall-ArapucaOut_x+ArapucaAcceptanceWindow_x/2.0+0.001;
for(int i=0; i<nrows; i++){
  for(int j=0; j<ncol;j++){
    name = "ArapucaWindowLat"; name.append(std::to_string(i+1)); name.append(std::to_string(j+1));
    physname = "fPhysAraWindowLat"; physname.append(std::to_string(i+1)); physname.append(std::to_string(j+1));
    G4VPhysicalVolume* physname = new G4PVPlacement(0,G4ThreeVector(xpos*m,(fCryostat_y/2-0.5-0.8*i)*m,
								    (-fCryostat_z/2+1.5+j*3.0)*m),name.c_str(), fLogicAraWindowLat, fPhysCryostat, false,0, true);
    name2 = "ArapucaWindowLlat"; name2.append(std::to_string(i+1)); name2.append(std::to_string(j+1));
    physname2 = "fPhysAraWindowLlat"; physname2.append(std::to_string(i+1)); physname2.append(std::to_string(j+1));
    G4VPhysicalVolume* physname2 = new G4PVPlacement(0,G4ThreeVector(-xpos*m,(fCryostat_y/2-0.5-0.8*i)*m,
								     (-fCryostat_z/2+1.5+j*3.0)*m),name2.c_str(), fLogicAraWindowLat, fPhysCryostat, false,0, true);
std::cout << name << " " << xpos << " " << (fCryostat_y/2-0.5-0.8*i) << " " << (fCryostat_y/2-0.5-0.8*i) << std::endl;
std::cout << name2 << " " << -xpos << " " << (fCryostat_y/2-0.5-0.8*i) << " " << (-fCryostat_z/2+1.5+j*3.0) << std::endl;
  }
 }

//CATHODE
double cathode[16];
cathode[0]=-5.5*0.84375;
cathode[1]=-1.5*0.84375;
cathode[2]=2.5*0.84375;
cathode[3]=6.5*0.84375;
cathode[4]=-7.5*0.84375;
cathode[5]=-3.5*0.84375;
cathode[6]=0.5*0.84375;
cathode[7]=4.5*0.84375;
cathode[8]=-4.5*0.84375;
cathode[9]=-0.5*0.84375;
cathode[10]=3.5*0.84375;
cathode[11]=7.5*0.84375;
cathode[12]=-6.5*0.84375;
cathode[13]=-2.5*0.84375;
cathode[14]=1.5*0.84375;
cathode[15]=5.5*0.84375;
    
//________________________________CATHODE ARAPUCAS SHIELDS____________________________________________//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
ArapucaOut_x = 0.65;
ArapucaOut_y = 0.025;
ArapucaOut_z = 0.65;
G4double ArapucaAcceptanceWindow_y = 0.01;//m
ncol=40;
int ncat=8, aux=0, auxcat=0;
G4String shield_namecat, shield_physnamecat;
  
G4Box* ArapucaOutBot = new G4Box("ArapucaOutBot",ArapucaOut_x/2*m,ArapucaOut_y/2*m,ArapucaOut_z/2*m);
G4Box* ArapucaInBot = new G4Box("ArapucaInBot",fwindow/2*m, ArapucaOut_y/2*m,fwindow/2*m);
G4SubtractionSolid* ArapucaBot = new G4SubtractionSolid("ArapucaBot", ArapucaOutBot, ArapucaInBot, 0, G4ThreeVector(0.,ArapucaOut_y/2*m,0.));
G4LogicalVolume* fLogicAraBot = new G4LogicalVolume(ArapucaBot,facrylic,"ArapucaBot");
  
yposBot=-fCryostat_y/2.0+ArapucaOut_y/2.0;
  
    
for(int i=0; i<ncol; i++){
  for(int j=0; j<ncat;j++){
    shield_namecat = "ArapucaBot"; shield_namecat.append(std::to_string(i+1)); shield_namecat.append(std::to_string(j+1));
    shield_physnamecat = "fPhysAraBot"; shield_physnamecat.append(std::to_string(i+1)); shield_physnamecat.append(std::to_string(j+1));
    G4VPhysicalVolume* shield_physnamecat = new G4PVPlacement(0,G4ThreeVector((cathode[auxcat])*m,yposBot*m,
									      (-fCryostat_z/2+(0.5+i+aux)*0.75)*m),shield_namecat.c_str(), fLogicAraBot, fPhysCryostat, false,0, true);
    if(j==3) aux++;
    if(auxcat==15) auxcat=0;
    else auxcat++;
  }
 }
    
G4Box* AraWindowBot = new G4Box("ArapucaWindowBot",fwindow/2*m,ArapucaAcceptanceWindow_y/2*m,fwindow/2*m);
G4LogicalVolume* fLogicAraWindowBot = new G4LogicalVolume(AraWindowBot,facrylic,"ArapucaWindowBot");

ncol=40;
ncat=8, aux=0, auxcat=0;
std::string namecat, physnamecat;
yposBot=-fCryostat_y/2.0+ArapucaOut_y-ArapucaAcceptanceWindow_y/2.0-0.001;
for(int i=0; i<ncol; i++){
  for(int j=0; j<ncat;j++){
    namecat = "ArapucaWindowBot"; namecat.append(std::to_string(i+1)); namecat.append(std::to_string(j+1));
    physnamecat = "fPhysAraWindowBot"; physnamecat.append(std::to_string(i+1)); physnamecat.append(std::to_string(j+1));
    G4VPhysicalVolume* physnamecat = new G4PVPlacement(0,G4ThreeVector((cathode[auxcat])*m,yposBot*m,
								       (-fCryostat_z/2+(0.5+i+aux)*0.75)*m),namecat.c_str(), fLogicAraWindowBot, fPhysCryostat, false,0, true);
    std::cout << namecat << " " << cathode[auxcat] << " " << yposBot << " " << (-fCryostat_z/2+(0.5+i+aux)*0.75) << std::endl;
    if(j==3) aux++;
    if(auxcat==15) auxcat=0;
    else auxcat++;
  }
 }
    
//________________________________ EXTRA MEMBRANE ARAPUCAS SHIELDS____________________________________________//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
ArapucaOut_y = 0.65;
ArapucaOut_z = 0.025;
ArapucaOut_x = 0.65;
G4double ArapucaAcceptanceWindow_z = 0.01;//m
DistFromCryoWall = 0.1; //m
ncol=2, nrows=4;
 
G4Box* ShortArapucaOut = new G4Box("ShortArapucaOut",ArapucaOut_x/2*m,ArapucaOut_y/2*m,ArapucaOut_z/2*m);
G4Box* ShortArapucaIn = new G4Box("ShortArapucaIn",fwindow/2*m,fwindow/2*m,ArapucaOut_z/2*m);
G4SubtractionSolid* ShortArapucaWalls = new G4SubtractionSolid("ShortArapucaWalls", ShortArapucaOut, ShortArapucaIn, 0, G4ThreeVector(0.,0.,ArapucaOut_z/2*m));
G4LogicalVolume* fLogicShortAraWalls = new G4LogicalVolume(ShortArapucaWalls,facrylic,"ShortArapucaWalls");

zpos=newfCryostat_z/2.0-DistFromCryoWall-ArapucaOut_z/2.0+0.001;
 
G4VPhysicalVolume* physShortAraWall = new G4PVPlacement(rWall,G4ThreeVector((-fCryostat_x/2+5.20)*m,(fCryostat_y/2-0.5)*m,zpos*m),"ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,0, true);
G4VPhysicalVolume* physShortAraWall2 = new G4PVPlacement(0,G4ThreeVector((-fCryostat_x/2+5.20)*m,(fCryostat_y/2-0.5)*m,-zpos*m),"ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,0, true);

for(int i=1; i<ncol; i++){ //copies of ArapucaWalls on each side (first row)
  G4PVPlacement* Shortarawall_cp  = new G4PVPlacement(rWall,G4ThreeVector((-fCryostat_x/2+5.20+i*4.4)*m,(fCryostat_y/2-0.5)*m,zpos*m), "ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,i, true);
  G4PVPlacement* Shortarawall_cp2  = new G4PVPlacement(0,G4ThreeVector((-fCryostat_x/2+5.20+i*4.4)*m,(fCryostat_y/2-0.5)*m,-zpos*m),"ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,i, true);
 }
G4int shortct=ncol; //count of copies for arapucawalls
for(int i=1; i<ncol; i++){ //1 copy of ArapucaWalls on each side (first row)
  G4PVPlacement* Shortarawall_cp  = new G4PVPlacement(rWall,G4ThreeVector((-fCryostat_x/2+5.20+i*4.4)*m,(fCryostat_y/2-0.5)*m,
									  zpos*m), "ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,i);//, true);
  G4PVPlacement* Shortarawall_cp2  = new G4PVPlacement(0,G4ThreeVector((-fCryostat_x/2+5.20+i*4.4)*m,(fCryostat_y/2-0.5)*m,
								       -zpos*m),  "ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,i);//, true);
 }
shortct=2; //count of copies for arapucawalls
for(int i=1; i<=3; i++){ //other copies of ArapucaWalls on each side (2-4 rows)
  for(int j=0; j<2;j++){
    G4PVPlacement* Shortarawallextra_cp  = new G4PVPlacement(rWall,G4ThreeVector((-fCryostat_x/2+5.20+j*4.4)*m,(fCryostat_y/2-0.5-0.8*i)*m,
										 zpos*m), "ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,shortct);//, true);
    G4PVPlacement* Shortarawallextra_cp2  = new G4PVPlacement(0,G4ThreeVector((-fCryostat_x/2+5.20+j*4.4)*m,(fCryostat_y/2-0.5-0.8*i)*m,
									      -zpos*m), "ShortArapucaWalls", fLogicShortAraWalls, fPhysCryostat, false,shortct);//, true);
    shortct++;
  }
 }

//Extra PDs on short laterals

G4Box* AraWindowShortLat = new G4Box("ArapucaWindowShort",fwindow/2*m,fwindow/2*m,ArapucaAcceptanceWindow_z/2*m);
G4LogicalVolume* fLogicAraWindowShortLat = new G4LogicalVolume(AraWindowShortLat,facrylic,"ArapucaWindowShort");

ncol=2, nrows=4;
std::string nameshort, physnameshort, nameshort2, physnameshort2;
zpos=newfCryostat_z/2.0-DistFromCryoWall-ArapucaOut_z+ArapucaAcceptanceWindow_z/2.0+0.001;
for(int i=0; i<nrows; i++){
  for(int j=0; j<ncol;j++){
    nameshort = "ArapucaWindowShortLat"; nameshort.append(std::to_string(i+1)); nameshort.append(std::to_string(j+1));
    physnameshort = "fPhysAraWindowShortLat"; physnameshort.append(std::to_string(i+1)); physnameshort.append(std::to_string(j+1));
    G4VPhysicalVolume* physname = new G4PVPlacement(0,G4ThreeVector((-fCryostat_x/2+5.20+j*4.4)*m,(fCryostat_y/2-0.5-0.8*i)*m,
								    zpos*m),nameshort.c_str(), fLogicAraWindowShortLat, fPhysCryostat, false,0, true);

    nameshort2 = "ArapucaWindowLShortlat"; nameshort2.append(std::to_string(i+1)); nameshort2.append(std::to_string(j+1));
    physnameshort2 = "fPhysAraWindowLShortlat"; physnameshort2.append(std::to_string(i+1)); physnameshort2.append(std::to_string(j+1));
    G4VPhysicalVolume* physname2 = new G4PVPlacement(0,G4ThreeVector((-fCryostat_x/2+5.20+j*4.4)*m,(fCryostat_y/2-0.5-0.8*i)*m,
								     -zpos*m),nameshort2.c_str(), fLogicAraWindowShortLat, fPhysCryostat, false,0, true);
    std::cout << nameshort << " " << (-fCryostat_x/2+5.20+j*4.4) << " " << (fCryostat_y/2-0.5-0.8*i) << " " << zpos << std::endl;
    std::cout << nameshort2 << " " << (-fCryostat_x/2+5.20+j*4.4) << " " << (fCryostat_y/2-0.5-0.8*i) << " " << -zpos << std::endl;
  }
 }


    

const G4int nEntries = 8;
//_________ RELEVANT ENERGY VALUES Xe 175nm -> 7.08eV; Ar 128 -> 9.69eV_________
    G4double PhotonEnergy[nEntries] =
      { 2.5*eV, 5.0*eV, 7.0*eV, 7.5*eV, 8.0*eV, 9.0*eV, 9.5*eV, 10.136*eV};

//Surfaces setup; lAr-Anode interface
G4OpticalSurface* AnodeSurface = new G4OpticalSurface("AnodeSurface");
AnodeSurface->SetType(dielectric_metal);
AnodeSurface->SetModel(unified);
AnodeSurface->SetFinish(ground);
AnodeSurface->SetSigmaAlpha(0.0*deg); // for vikuit

//_______ANODE REFLECTIVITY CHANGE (WITH 40% SOLID AREA) 0.2 -> 0.3*0.4 and 0.0 -> 0.15*0.4______________
/////////////////////////////////////////////////////////////////////////////////////////////////////////
G4double Anode_r[nEntries] = {0.12, 0.12, 0.12, 0.12, 0.06, 0.06, 0.06, 0.06}; //reflection coef for base
G4double Anode_e[nEntries] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; //absorption coefficient
// G4double Anode_r[nEntries] = {0.12, 0.12, 0.12, 0.12, 0.06, 0.06, 0.06, 0.06}; //reflection coef for base
/////////////////////////////////////////////////////////////////////////////////////////////////////////

G4MaterialPropertiesTable* AnodeSurface_pt = new G4MaterialPropertiesTable();

AnodeSurface_pt->AddProperty("REFLECTIVITY", PhotonEnergy, Anode_r, nEntries);
AnodeSurface_pt->AddProperty("EFFICIENCY", PhotonEnergy, Anode_e, nEntries);

AnodeSurface->SetMaterialPropertiesTable(AnodeSurface_pt);
new G4LogicalSkinSurface("AnodeSurface", fLogicAnode, AnodeSurface);

G4OpticalSurface* FCSurface = new G4OpticalSurface("FCSurface");
FCSurface->SetType(dielectric_metal);
FCSurface->SetModel(unified);
FCSurface->SetFinish(ground);
FCSurface->SetSigmaAlpha(0.0*deg); // for vikuit

//_____________________________FC REFLECTIVITY CHANGE 0.2 -> 0.7 ______________________________
G4double FC_r[nEntries] = {0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,}; //reflection coef for base
G4double FC_e[nEntries] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; //absorption coefficient
//G4double FC_r[nEntries] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //reflection coef for base

G4MaterialPropertiesTable* FCSurface_pt = new G4MaterialPropertiesTable();

FCSurface_pt->AddProperty("REFLECTIVITY", PhotonEnergy, FC_r, nEntries);
FCSurface_pt->AddProperty("EFFICIENCY", PhotonEnergy, FC_e, nEntries);

FCSurface->SetMaterialPropertiesTable(FCSurface_pt);
new G4LogicalSkinSurface("FCSurfaceShort", fLogicFCShort, FCSurface);
new G4LogicalSkinSurface("FCSurfaceShortSlim", fLogicFCShortSlim, FCSurface);
new G4LogicalSkinSurface("FCSurfaceSlim", fLogicFCSlim, FCSurface);
new G4LogicalSkinSurface("FCSurfaceWide", fLogicFCwide, FCSurface);

// _________________ CHANGE NEW CRYOSTAT PROPRTIES _____________________________________
G4OpticalSurface* CryostatSurface = new G4OpticalSurface("CryostatSurface");
CryostatSurface->SetType(dielectric_metal);
CryostatSurface->SetModel(unified);
CryostatSurface->SetFinish(ground);
CryostatSurface->SetSigmaAlpha(0.0*deg); // for vikuit

G4double Cryo_r[nEntries] = {0.4, 0.4, 0.4, 0.4, 0.3, 0.3, 0.3, 0.3}; //reflection coef for base
G4double Cryo_e[nEntries] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; //absorption coefficient

G4MaterialPropertiesTable* CryostatSurface_pt = new G4MaterialPropertiesTable();

CryostatSurface_pt->AddProperty("REFLECTIVITY", PhotonEnergy, Cryo_r, nEntries);
CryostatSurface_pt->AddProperty("EFFICIENCY", PhotonEnergy, Cryo_e, nEntries);

CryostatSurface->SetMaterialPropertiesTable(CryostatSurface_pt);
new G4LogicalSkinSurface("CryostatSurfaceShort", fLogicCryostat, CryostatSurface);

G4VisAttributes* simpleWorldVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //White
simpleWorldVisAtt->SetVisibility(true);

//G4VisAttributes* simplePlain= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.4));
G4VisAttributes* simplePlain= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.5)); //White
simplePlain->SetVisibility(true);
simplePlain->SetForceSolid(true);
simplePlain->SetForceAuxEdgeVisible(true);

//G4VisAttributes* simpleBoxAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0,0.3));
G4VisAttributes* simpleBoxAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0,0.5));
simpleBoxAtt->SetDaughtersInvisible(true);
simpleBoxAtt->SetForceSolid(true);
simpleBoxAtt->SetForceAuxEdgeVisible(true);

G4VisAttributes* simpleBoxAttKGM= new G4VisAttributes(G4Colour(0.0,0.0,1.0,1.0));
simpleBoxAttKGM->SetVisibility(true);
simpleBoxAttKGM->SetForceWireframe(true);
simpleBoxAttKGM->SetDaughtersInvisible(false);
simpleBoxAttKGM->SetForceSolid(true);

G4VisAttributes* BoxAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.0));
BoxAtt->SetDaughtersInvisible(true);
BoxAtt->SetForceSolid(true);
BoxAtt->SetForceAuxEdgeVisible(true);

//  fLogicCathode->SetVisAttributes(simpleBoxAtt);
fLogicAraWindowLat->SetVisAttributes(simpleBoxAttKGM);
//  fLogicAraWindowTop->SetVisAttributes(simpleBoxAttKGM);
fLogicAraWindowBot->SetVisAttributes(simpleBoxAttKGM);
fLogicAraWindowShortLat->SetVisAttributes(simpleBoxAttKGM);
fLogicFCwide->SetVisAttributes(simplePlain);
fLogicFCShortaux->SetVisAttributes(simpleBoxAtt);
fLogicFCShortaux2->SetVisAttributes(simpleBoxAtt);
fLogicFCShort->SetVisAttributes(simpleBoxAtt);
fLogicCathode->SetVisAttributes(simplePlain);
fLogicAraWalls->SetVisAttributes(BoxAtt);
fLogicShortAraWalls->SetVisAttributes(BoxAtt);
fLogicAraBot->SetVisAttributes(BoxAtt);
return fPhysiWorld;
}
