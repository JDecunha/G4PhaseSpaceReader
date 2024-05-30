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

DetectorConstruction::DetectorConstruction(G4String phantomType, G4double phantomHalfLength, G4double scoringResolution):G4VUserDetectorConstruction() , _phantomSetup(phantomType) , _phantomHalfLength(phantomHalfLength) , _scoringResolution(scoringResolution)
{

}  

DetectorConstruction::~DetectorConstruction()
{

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructDetector();
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{

  // std::cout << "DetectorConstruction: Phantom type is LuciteBlock" << std::endl;
  // std::cout << "Phantom half length is " << _phantomHalfLength/mm << " mm " << std::endl;
  // std::cout << "Scoring resolution is " << _scoringResolution/mm << " mm " << std::endl;

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


  
  if (_phantomSetup == "LuciteBlock" || _phantomSetup == "LuciteBlockHighRes")
  {
    //Create the phantom
    G4double xhalfsize = 8.0*cm; //16 cm x 12 cm x [User defined] cm thick
    G4double yhalfsize = 6.0*cm;
    G4double zhalfsize = _phantomHalfLength;
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

    G4VisAttributes* lucite_Vis = new G4VisAttributes();
    lucite_Vis->SetColor(1, 1, 1);
    lucite_Vis->SetVisibility(true);
    LucitePhantom_log->SetVisAttributes(lucite_Vis);

    G4VisAttributes* world_Vis = new G4VisAttributes();
    world_Vis->SetColor(1, 1, 1);
    world_Vis->SetVisibility(false);
    logicWorld->SetVisAttributes(world_Vis);
  }
  else if (_phantomSetup == "WellPlate")
  {
    //Create the phantom
    G4double xhalfsize = 8.0*cm; //16 cm x 12 cm x [User defined] cm thick
    G4double yhalfsize = 6.0*cm;
    G4double zhalfsize = _phantomHalfLength;
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

    G4VisAttributes* lucite_Vis = new G4VisAttributes();
    lucite_Vis->SetColor(1, 1, 1);
    lucite_Vis->SetVisibility(true);
    LucitePhantom_log->SetVisAttributes(lucite_Vis);

    G4VisAttributes* world_Vis = new G4VisAttributes();
    world_Vis->SetColor(1, 1, 1);
    world_Vis->SetVisibility(false);
    logicWorld->SetVisAttributes(world_Vis);

    Eppendorf_48WellPlate_Model model;
    model.Construct(logicWorld, _phantomHalfLength);
    model.ConstructLiquidandScorers(logicWorld, 2.5, _phantomHalfLength);
  }
  else if (_phantomSetup == "LateralScoring")
  {
    //Create the phantom
    G4double xhalfsize = 8.0*cm; //16 cm x 12 cm x [User defined] cm thick
    G4double yhalfsize = 6.0*cm;
    G4double zhalfsize = _phantomHalfLength;
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

    G4VisAttributes* lucite_Vis = new G4VisAttributes();
    lucite_Vis->SetColor(1, 1, 1);
    lucite_Vis->SetVisibility(true);
    LucitePhantom_log->SetVisAttributes(lucite_Vis);

    G4VisAttributes* world_Vis = new G4VisAttributes();
    world_Vis->SetColor(1, 1, 1);
    world_Vis->SetVisibility(false);
    logicWorld->SetVisAttributes(world_Vis);

    G4Box* LuciteLateralBox = new G4Box("LuciteLateralBox", xhalfsize, yhalfsize, 0.5*mm); //1 mm thick slab to sit on top
    G4LogicalVolume* LuciteLateralBox_log = new G4LogicalVolume(LuciteLateralBox, lucite,"LuciteLateralBox_log");
    G4ThreeVector slabOffset = G4ThreeVector(0, 0, (-zhalfsize+1.7*cm)-zhalfsize-(0.5*mm));
    G4VPhysicalVolume* LuciteLateralBox_physical = new G4PVPlacement(0, slabOffset, LuciteLateralBox_log, "LuciteLateralBox", logicWorld, false, 0, false);

    //Gotta do some replica stuff here huh

    //So the box is 16 x 12 cm. If I slice it up in 1 mm x 1 mm x 1mm cubes I will have
    //160 x 120 = 19200 voxels. Seems like a lot, I hope it doesn't destroy me

    G4double resolution = 1*mm;
    G4int xIncrements = 160; 
    G4int yIncrements = 120;
    G4int numVoxels = xIncrements*yIncrements;

    G4Box* ScoringXCuts = new G4Box("ScoringXCuts", resolution/2, yhalfsize, 0.5*mm); //1 mm thick slab to sit on top
    G4LogicalVolume* ScoringXCuts_logical = new G4LogicalVolume(ScoringXCuts, lucite,"ScoringXCuts_logical");

    G4VPhysicalVolume* ScoringXCuts_physical = new G4PVReplica("ScoringXCutsPhysical",
                                                                ScoringXCuts_logical,
                                                                LuciteLateralBox_physical,
                                                                kXAxis,
                                                                xIncrements,
                                                                resolution);

    G4Box* ScoringYCuts = new G4Box("ScoringYCuts", resolution/2, resolution/2, 0.5*mm); //1 mm thick slab to sit on top
    G4LogicalVolume* ScoringYCuts_logical = new G4LogicalVolume(ScoringYCuts, lucite,"ScoringYCuts_logical");

    G4VPhysicalVolume* ScoringYCuts_physical = new G4PVReplica("ScoringYCutsPhysical",
                                                              ScoringYCuts_logical,
                                                              ScoringXCuts_physical,
                                                              kYAxis,
                                                              yIncrements,
                                                              resolution);

    // I should visualize here
    G4VisAttributes* lateral_vis = new G4VisAttributes();
    lateral_vis->SetColor(0, 1, 0);
    lateral_vis->SetVisibility(true);
    ScoringYCuts_logical->SetVisAttributes(lateral_vis);


  }
  else if (_phantomSetup == "LateralScoringWide")
  {
    //Create the phantom
    G4double xhalfsize = 15.0*cm; //16 cm x 12 cm x [User defined] cm thick
    G4double yhalfsize = 15.0*cm;
    G4double zhalfsize = _phantomHalfLength;
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

    G4VisAttributes* lucite_Vis = new G4VisAttributes();
    lucite_Vis->SetColor(1, 1, 1);
    lucite_Vis->SetVisibility(true);
    LucitePhantom_log->SetVisAttributes(lucite_Vis);

    G4VisAttributes* world_Vis = new G4VisAttributes();
    world_Vis->SetColor(1, 1, 1);
    world_Vis->SetVisibility(false);
    logicWorld->SetVisAttributes(world_Vis);
    
    G4Box* LuciteLateralBox = new G4Box("LuciteLateralBox", xhalfsize, yhalfsize, 0.5*mm); //1 mm thick slab to sit on top
    G4LogicalVolume* LuciteLateralBox_log = new G4LogicalVolume(LuciteLateralBox, lucite,"LuciteLateralBox_log");
    G4ThreeVector slabOffset = G4ThreeVector(0, 0, (-zhalfsize+1.7*cm)-zhalfsize-(0.5*mm));
    G4VPhysicalVolume* LuciteLateralBox_physical = new G4PVPlacement(0, slabOffset, LuciteLateralBox_log, "LuciteLateralBox", logicWorld, false, 0, false);

    //Gotta do some replica stuff here huh

    //So the box is 16 x 12 cm. If I slice it up in 1 mm x 1 mm x 1mm cubes I will have
    //160 x 120 = 19200 voxels. Seems like a lot, I hope it doesn't destroy me

    G4double resolution = 5*mm;
    G4int xIncrements = 60; 
    G4int yIncrements = 60;
    G4int numVoxels = xIncrements*yIncrements;

    G4Box* ScoringXCuts = new G4Box("ScoringXCuts", resolution/2, yhalfsize, 0.5*mm); //1 mm thick slab to sit on top
    G4LogicalVolume* ScoringXCuts_logical = new G4LogicalVolume(ScoringXCuts, lucite,"ScoringXCuts_logical");

    G4VPhysicalVolume* ScoringXCuts_physical = new G4PVReplica("ScoringXCutsPhysical",
                                                                ScoringXCuts_logical,
                                                                LuciteLateralBox_physical,
                                                                kXAxis,
                                                                xIncrements,
                                                                resolution);

    G4Box* ScoringYCuts = new G4Box("ScoringYCuts", resolution/2, resolution/2, 0.5*mm); //1 mm thick slab to sit on top
    G4LogicalVolume* ScoringYCuts_logical = new G4LogicalVolume(ScoringYCuts, lucite,"ScoringYCuts_logical");

    G4VPhysicalVolume* ScoringYCuts_physical = new G4PVReplica("ScoringYCutsPhysical",
                                                              ScoringYCuts_logical,
                                                              ScoringXCuts_physical,
                                                              kYAxis,
                                                              yIncrements,
                                                              resolution);

    // I should visualize here
    G4VisAttributes* lateral_vis = new G4VisAttributes();
    lateral_vis->SetColor(0, 1, 0);
    lateral_vis->SetVisibility(true);
    ScoringYCuts_logical->SetVisAttributes(lateral_vis);


  }
  else
  {
    std::cout << "DetectorConstruction: Phantom type " << _phantomSetup << " is unknown." << std::endl;
  }
  
  return physiWorld;
}

void DetectorConstruction::ConstructSDandField() 
{
  if (_phantomSetup == "WellPlate")
  {
    // std::cout << "Physical world scoring activated" << std::endl;

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
  else if (_phantomSetup == "LateralScoring" || _phantomSetup == "LateralScoringWide")
  {
    // std::cout << "Physical world scoring activated" << std::endl;

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
    SetSensitiveDetector("ScoringYCuts_logical",MultiFuncDetector);
  }
}
