#include <vector>
#include <string>
#include <iostream>
//This simulation
#include "EdepSquaredEventbyEventScorer.hh"
//Geant4
#include "G4AnalysisManager.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTouchable.hh"


EdepSquaredEventbyEventScorer::EdepSquaredEventbyEventScorer(G4String name, G4int depth): G4VPrimitiveScorer(name,depth) { }

G4bool EdepSquaredEventbyEventScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*) 
{
  G4double edep =  aStep->GetTotalEnergyDeposit();

  if (edep > 0)
  {
    G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    //Add the energy deposition to the temporary edep accumulator for this event (will be squared later)
    // analysisManager->FillH1(3 , i, _doseAccumulatorOfThisEvent[i]);
  }

  return 0;
}

void EdepSquaredEventbyEventScorer::EndOfEvent(G4HCofThisEvent*)
{

  // for (int i = 0; i <= _doseAccumulatorOfThisEvent.size(); i++)
  // {
  //   G4double doseThisEvent = _doseAccumulatorOfThisEvent[i];
    
  //   if(doseThisEvent != 0)
  //   {
  //     //Square the dose for this  event
  //     doseThisEvent = doseThisEvent*doseThisEvent;

  //     auto analysisManager = G4AnalysisManager::Instance();
  //     analysisManager->FillH1(2 , i, _doseAccumulatorOfThisEvent[i]);
  //   }
  // }

  //Clear the dose counter for this event
  //TODO: I think we can replace this with G4AnalysisManager::ScaleH1 and set the scale to zero to reset it.
  // std::fill(_doseAccumulatorOfThisEvent.begin(), _doseAccumulatorOfThisEvent.end(), 0.0);

}