#include <vector>
#include <string>
#include <iostream>
//This simulation
#include "ProtonSlowingSpectrumScorer.hh"
//Geant4
#include "G4AnalysisManager.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTouchable.hh"


ProtonSlowingSpectrumScorer::ProtonSlowingSpectrumScorer(G4String name, G4int depth):G4VPrimitiveScorer(name,depth) { }

G4bool ProtonSlowingSpectrumScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*) 
{
  //Filtering by:
  //1.) Particle type is proton

  if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "proton")
  {
    G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(1, index, energy);
  }


  return 0;
}