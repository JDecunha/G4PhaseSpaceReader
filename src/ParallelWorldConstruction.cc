//Geant4
#include "G4SystemOfUnits.hh"
//This project
#include "ParallelWorldConstruction.hh"
#include "StraightLineParameterisation.hh"
#include "RunAction.hh"
//This project: Scorers
#include "EdepScorer.hh"
#include "EdepSquaredEventbyEventScorer.hh"
#include "ProtonSpectrumScorer.hh"
//Geant4
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"

ParallelWorldConstruction::ParallelWorldConstruction(const G4String& parallelWorldName): G4VUserParallelWorld(parallelWorldName) { }

void ParallelWorldConstruction::Construct() 
{
  //Get the world pointers (required for parallel worlds)
  G4VPhysicalVolume* motherWorldPointer = GetWorld();
  G4LogicalVolume* motherWorldLogical = motherWorldPointer->GetLogicalVolume();

  //Get the RunAction pointer so that you can ensure the output histograms have a number of bins that corresponds 
  //to the desired number of voxels
  RunAction* pRunAction = ((RunAction*)G4RunManager::GetRunManager()->GetUserRunAction());

  //Define G4Box that the parameterisation will place many copies of
  G4double xHalfSize = 5*cm; 
  G4double yHalfSize = 5*cm;
  G4double zHalfSize = pRunAction->GetScoringHalfLength();

  G4Box* scoringBox = new G4Box("scoringBox", xHalfSize, yHalfSize, zHalfSize);
  G4LogicalVolume * scoringBox_logical = new G4LogicalVolume(scoringBox, 0, "scoringBox_logical", 0, 0, 0);

  //Visualize scoring box
  G4VisAttributes* scoring_vis = new G4VisAttributes();
  scoring_vis->SetColor(1, 0, 0);
  scoring_vis->SetVisibility(true);
  scoringBox_logical->SetVisAttributes(scoring_vis);

  //Slice up the scoring box
  G4double zAxisResolution = pRunAction->GetScoringResolution();
  G4int zIncrements = (zHalfSize*2.)/zAxisResolution; //Set zIncrements according to desired resolution 
  G4int xIncrements = 1; G4int yIncrements = 1;
  G4int numVoxels = xIncrements*yIncrements*zIncrements;

  StraightLineParameterisation* lineParam = new StraightLineParameterisation(xHalfSize,yHalfSize,zHalfSize,zIncrements);
  G4PVParameterised* phantomVoxels = new G4PVParameterised("voxelsAlongLine", scoringBox_logical, motherWorldLogical, kZAxis, numVoxels, lineParam);

}

void ParallelWorldConstruction::ConstructSD() 
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

  //Register sensitive detector with SDManager, and register SD with logical volume
  SDManager->AddNewDetector(MultiFuncDetector);
  SetSensitiveDetector("scoringBox_logical",MultiFuncDetector);
}
