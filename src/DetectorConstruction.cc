//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
//This project: Scorers
#include "EdepScorer.hh"
#include "EdepSquaredEventbyEventScorer.hh"
#include "ProtonSpectrumScorer.hh"
#include "ProtonSlowingSpectrumScorer.hh"
//Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

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

  //Alright, let's make our scoring volume in the physical world
  G4Box* LucitePhantom = new G4Box("LucitePhantom", 1*cm, 1*cm, 5*cm);
  G4LogicalVolume* LucitePhantom_log = new G4LogicalVolume(LucitePhantom, lucite,"LucitePhantom_log");
  G4ThreeVector phantomOffset = G4ThreeVector(0, 0, ((-5*cm)+1.7*cm));
  G4VPhysicalVolume* LucitePhantom_physical = new G4PVPlacement(0, phantomOffset, LucitePhantom_log, "LucitePhantom", logicWorld, false, 0, false);

  //And make our replica volume
  double zAxisThickness = 0.1*cm;
  G4Box* LucitePhantomZCuts = new G4Box("LucitePhantomZCuts", 1*cm, 1*cm, (zAxisThickness/2.));
  G4LogicalVolume* LucitePhantomZCuts_log = new G4LogicalVolume(LucitePhantomZCuts, lucite,"LucitePhantomZCuts_log");


  //Slice it up
  G4VPhysicalVolume* LucitePhantomZCuts_physical = new G4PVReplica("LucitePhantomZCutsPhysical",
                                                              LucitePhantomZCuts_log,
                                                              LucitePhantom_physical,
                                                              kZAxis,
                                                              100,
                                                              zAxisThickness);

  //We want the upper surface of the phantom at 1.7 cm above isocenter. 
  //Because current experiments are made with snout 1.3 cm above isocenter
  //and true bottom of snout insert is ~3 cm below that (so 1.7 cm below isocenter)
  //However Uwe's phase space shoots DOWN from the top, rather than experiment shooting up from the bottom.
  //So we are modelling Madison/Fada's experiment but inverted. (i.e. protons impinging on top rather than bottom)
  //So IRL the phantom bottom surface will be 1.7 cm below isocenter, but in this simulation it will be 1.7 cm above isocenter.
  

  return physiWorld;
}

void DetectorConstruction::ConstructSDandField() 
{

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

  G4VPrimitiveScorer* protonSlowingSpectrumScorer;
  protonSlowingSpectrumScorer = new ProtonSlowingSpectrumScorer("protonSlowingSpectrum",0);
  MultiFuncDetector->RegisterPrimitive(protonSlowingSpectrumScorer);

  //Register sensitive detector with SDManager, and register SD with logical volume
  SDManager->AddNewDetector(MultiFuncDetector);
  SetSensitiveDetector("LucitePhantomZCuts_log",MultiFuncDetector);
}
