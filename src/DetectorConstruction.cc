//This project
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
#include "Eppendorf_48WellPlate_Model.hh"
#include "RunAction.hh"
//This project: Scorers
#include "EdepScorer.hh"
#include "EdepSquaredEventbyEventScorer.hh"
#include "ProtonSpectrumScorer.hh"
#include "ProtonSlowingSpectrumScorer.hh"
#include "LETSpectrumScorer.hh"
//Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4RunManager.hh"

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction()
{
  pMessenger = new DetectorConstructionMessenger(this);
}  

DetectorConstruction::~DetectorConstruction()
{
  delete pMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructDetector();
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
  //Define materials
  G4NistManager * man = G4NistManager::Instance();
  G4Material* air = man->FindOrBuildMaterial("G4_AIR");
  G4Material* lucite = man->FindOrBuildMaterial("G4_LUCITE");

  //Create the world volume
  double sideLength = 100*cm; //The phase space is 48 cm from ISO  
  G4VSolid* solidWorld = new G4Box("World", sideLength/2,sideLength/2,sideLength);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,  //its solid
                                    air,  //its material
                                    "World");    //its name

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(),  //at (0,0,0), a requirement of the mother volume
                                  "World",    //its name
                                  logicWorld,    //its logical volume
                                  0,      //its mother  volume
                                  false,      //no boolean operation
                                  0);      //copy number

  //Create the phantom
  G4double xhalfsize = 8.0*cm; //16 cm x 12 cm x 10 cm thick
  G4double yhalfsize = 6.0*cm;
  G4double zhalfsize = 5.0*cm;
  G4Box* LucitePhantom = new G4Box("LucitePhantom", xhalfsize, yhalfsize, zhalfsize); //Initial phase space covers 13 cm x 9 cm (i.e 6.5 x 4.5 half length)
  G4LogicalVolume* LucitePhantom_log = new G4LogicalVolume(LucitePhantom, lucite,"LucitePhantom_log");

  //We want the upper surface of the phantom at 1.7 cm above isocenter. 
  //Because current experiments are made with snout 1.3 cm above isocenter
  //and true bottom of snout insert is ~3 cm below that (so 1.7 cm below isocenter)
  //However Uwe's phase space shoots DOWN from the top, rather than experiment shooting up from the bottom.
  //So we are modelling Madison/Fada's experiment but inverted. (i.e. protons impinging on top rather than bottom)
  //So IRL the phantom bottom surface will be 1.7 cm below isocenter, but in this simulation it will be 1.7 cm above isocenter.
  G4ThreeVector phantomOffset = G4ThreeVector(0, 0, (-zhalfsize+1.7*cm));

  G4VPhysicalVolume* LucitePhantom_physical = new G4PVPlacement(0, phantomOffset, LucitePhantom_log, "LucitePhantom", logicWorld, false, 0, false);

  Eppendorf_48WellPlate_Model model;
  model.Construct(logicWorld);
  model.ConstructLiquidandScorers(logicWorld,2.5);

  G4VisAttributes* lucite_Vis = new G4VisAttributes();
  lucite_Vis->SetColor(1, 1, 1);
  lucite_Vis->SetVisibility(true);
  LucitePhantom_log->SetVisAttributes(lucite_Vis);

  G4VisAttributes* world_Vis = new G4VisAttributes();
  world_Vis->SetColor(1, 1, 1);
  world_Vis->SetVisibility(false);
  logicWorld->SetVisAttributes(world_Vis);

  return physiWorld;
}

void DetectorConstruction::ConstructSDandField() 
{
  //To pull the number of bins
  RunAction* pRunAction = ((RunAction*)G4RunManager::GetRunManager()->GetUserRunAction());

  //Set up the scoring
  G4SDManager* SDManager = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* MultiFuncDetector = new G4MultiFunctionalDetector("multifuncdetector1");

  //Make the scorers
  G4VPrimitiveScorer* edepScorer;
  edepScorer = new EdepScorer("edep",0);
  MultiFuncDetector->RegisterPrimitive(edepScorer);

  G4VPrimitiveScorer* edepSquaredScorer;
  edepSquaredScorer = new EdepSquaredEventbyEventScorer("edepSquared",0);
  MultiFuncDetector->RegisterPrimitive(edepSquaredScorer);

  G4VPrimitiveScorer* protonEnergySpectrumScorer;
  protonEnergySpectrumScorer = new ProtonSpectrumScorer("protonSpectrum",0);
  MultiFuncDetector->RegisterPrimitive(protonEnergySpectrumScorer);

  G4VPrimitiveScorer* letSpectrumScorer;
  letSpectrumScorer = new LETSpectrumScorer("LETSpectrum",0);
  MultiFuncDetector->RegisterPrimitive(letSpectrumScorer);

  // G4VPrimitiveScorer* protonSlowingSpectrumScorer;
  // protonSlowingSpectrumScorer = new ProtonSlowingSpectrumScorer("protonSlowingSpectrum",0);
  // MultiFuncDetector->RegisterPrimitive(protonSlowingSpectrumScorer);

  //Register sensitive detector with SDManager, and register SD with logical volume
  SDManager->AddNewDetector(MultiFuncDetector);
  SetSensitiveDetector("Scorer_Logical",MultiFuncDetector);
}
