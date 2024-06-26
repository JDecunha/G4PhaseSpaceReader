#include <vector>
#include <string>
#include <iostream>
//This simulation
#include "ProtonSpectrumScorer.hh"
//Geant4
#include "G4AnalysisManager.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTouchable.hh"


ProtonSpectrumScorer::ProtonSpectrumScorer(G4String name, G4int depth):G4VPrimitiveScorer(name,depth) { }

G4bool ProtonSpectrumScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*) 
{
  //Filtering by:
  //1.) First step in volume
  //2.) Particle type is proton

  if(aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary && aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "proton")
  {
    // For wide scorer
    G4int indexa = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    G4int indexb = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth+1);
    G4int index = (indexb*60)+indexa;

    // For depth scoring and well plate scoring
    // G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);

    G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(0, index, energy);
  }


  return 0;
}
