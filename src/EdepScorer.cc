#include <vector>
#include <string>
#include <iostream>
//This simulation
#include "EdepScorer.hh"
//Geant4
#include "G4AnalysisManager.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTouchable.hh"


EdepScorer::EdepScorer(G4String name, G4int depth):G4VPrimitiveScorer(name,depth) { std::cout << "Scorer created" << std::endl;}

G4bool EdepScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*) 
{
  G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
  G4double edep =  aStep->GetTotalEnergyDeposit();
  
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0 , index, edep);

  std::cout << "Hit! in volume: " << index << " for: " << edep << std::endl; 

  return 0;
}