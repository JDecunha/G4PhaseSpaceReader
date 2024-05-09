//Geant4
#include "G4SystemOfUnits.hh"
//This project
#include "DetectorConstruction.hh"
#include "ParallelWorldConstruction.hh"
#include "StraightLineParameterisation.hh"
#include "RunAction.hh"
//This project: Scorers
#include "EdepScorer.hh"
#include "EdepSquaredEventbyEventScorer.hh"
#include "ProtonSpectrumScorer.hh"
#include "ProtonSlowingSpectrumScorer.hh"
#include "LETSpectrumScorer.hh"
//Geant4
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif
#include "G4VisAttributes.hh"

ParallelWorldConstruction::ParallelWorldConstruction(const G4String& parallelWorldName): G4VUserParallelWorld(parallelWorldName) { }

void ParallelWorldConstruction::Construct() 
{
  //Get the DetectorConstruction pointer
  DetectorConstruction* pDetectorConstruction = ((DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double phantomHalfSize = pDetectorConstruction->GetPhantomHalfLength();
  G4double scoringResolution = pDetectorConstruction->GetScoringResolution();
  G4String _phantomType = pDetectorConstruction->GetPhantomType();

  //Get the world pointers (required for parallel worlds)
  G4VPhysicalVolume* motherWorldPointer = GetWorld();
  G4LogicalVolume* motherWorldLogical = motherWorldPointer->GetLogicalVolume();

  //Define G4Box that the parameterisation will place many copies of
  G4double xHalfSize = 2*cm; 
  G4double yHalfSize = 2*cm;
  G4double zHalfSize = phantomHalfSize;

  if (_phantomType == "LuciteBlock")
  {
    //
    // Normal scoring box
    //

    G4Box* scoringBox = new G4Box("scoringBox", xHalfSize, yHalfSize, zHalfSize);
    G4LogicalVolume * scoringBox_logical = new G4LogicalVolume(scoringBox, 0, "scoringBox_logical", 0, 0, 0);
    G4ThreeVector phantomOffset = G4ThreeVector(0, 0, ((-5*cm)+1.7*cm));
    G4VPhysicalVolume* scoringBox_physical = new G4PVPlacement(0, phantomOffset, scoringBox_logical, "scoringBox_logical", motherWorldLogical, false, 0, false);

    //Visualize scoring box
    G4VisAttributes* scoring_vis = new G4VisAttributes();
    scoring_vis->SetColor(1, 0, 0);
    scoring_vis->SetVisibility(true);
    scoringBox_logical->SetVisAttributes(scoring_vis);

    //Slice up the scoring box
    G4double zAxisResolution = pDetectorConstruction->GetScoringResolution();
    G4int zIncrements = (zHalfSize*2.)/zAxisResolution; //Set zIncrements according to desired resolution 
    G4int xIncrements = 1; G4int yIncrements = 1;
    G4int numVoxels = xIncrements*yIncrements*zIncrements;

    G4Box* ScoringZCuts = new G4Box("ScoringZCuts", 1*cm, 1*cm, (zAxisResolution/2.));
    G4LogicalVolume* ScoringZCuts_log = new G4LogicalVolume(ScoringZCuts, 0,"ScoringZCutsLog");
    G4VPhysicalVolume* ScoringZCuts_physical = new G4PVReplica("ScoringZCutsPhysical",
                                                                ScoringZCuts_log,
                                                                scoringBox_physical,
                                                                kZAxis,
                                                                zIncrements,
                                                                zAxisResolution);

    //Explanation for magic numbers contained in: phantomOffset
    //We want the upper surface of the phantom at 1.7 cm above isocenter. 
    //Because current experiments are made with snout 1.3 cm above isocenter
    //and, the true bottom of snout insert is ~3 cm below that (so 1.7 cm below isocenter)
    //However Uwe's phase space shoots DOWN from the top, rather than experiment shooting up from the bottom.
    //So we are modelling Madison/Fada's experiment but inverted. (i.e. protons impinging on top rather than bottom)
    //So IRL the phantom bottom surface will be 1.7 cm below isocenter, but in this simulation it will be 1.7 cm above isocenter.
  }

  //
  // High res scoring
  //

  if (_phantomType == "LuciteBlockHighRes")
  {

    G4Box* scoringBox = new G4Box("scoringBox", xHalfSize, yHalfSize, 0.3*cm);
    G4LogicalVolume * scoringBox_logical = new G4LogicalVolume(scoringBox, 0, "scoringBox_logical", 0, 0, 0);

    /*
    Calculating the new offset. Original offset is 1.7 cm above isocenter, minus 5 cm (which is the half length).
    So the half length puts the top of the box at the center of the world, then we shift it up by 1.7 cm.
    If we want it to run from 37-43 mm, then we should shift it down by 3.7 cm. And make it 0.6 cm thick (0.3 cm half length).

    */

    G4ThreeVector phantomOffset = G4ThreeVector(0, 0, ((-0.3*cm)+(1.7*cm)-(3.7*cm)));
    G4VPhysicalVolume* scoringBox_physical = new G4PVPlacement(0, phantomOffset, scoringBox_logical, "scoringBox_logical", motherWorldLogical, false, 0, false);

    //Visualize scoring box
    G4VisAttributes* scoring_vis = new G4VisAttributes();
    scoring_vis->SetColor(1, 0, 0);
    scoring_vis->SetVisibility(true);
    scoringBox_logical->SetVisAttributes(scoring_vis);

    //Slice up the scoring box
    G4double zAxisResolution = 0.01*mm;
    G4int zIncrements = (0.3*2.*cm)/zAxisResolution; //Set zIncrements according to desired resolution 
    G4int xIncrements = 1; G4int yIncrements = 1;
    G4int numVoxels = xIncrements*yIncrements*zIncrements;

    G4Box* ScoringZCuts = new G4Box("ScoringZCuts", 1*cm, 1*cm, (zAxisResolution/2.));
    G4LogicalVolume* ScoringZCuts_log = new G4LogicalVolume(ScoringZCuts, 0,"ScoringZCutsLog");
    G4VPhysicalVolume* ScoringZCuts_physical = new G4PVReplica("ScoringZCutsPhysical",
                                                                ScoringZCuts_log,
                                                                scoringBox_physical,
                                                                kZAxis,
                                                                zIncrements,
                                                                zAxisResolution);
  }

}

void ParallelWorldConstruction::ConstructSD() 
{

  //Get the DetectorConstruction pointer
  DetectorConstruction* pDetectorConstruction = ((DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4String _phantomType = pDetectorConstruction->GetPhantomType();

  if (_phantomType == "LuciteBlock" || _phantomType == "LuciteBlockHighRes")
  {
    // std::cout << "Parallel world scoring activated" << std::endl;

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
    SetSensitiveDetector("ScoringZCutsLog",MultiFuncDetector);
  }
}
