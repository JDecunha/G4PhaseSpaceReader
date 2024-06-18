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
    // For wide scorer
    // G4int indexa = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    // G4int indexb = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth+1);
    // G4int index = (indexb*60)+indexa;

    // For well plate scorer
    G4int indexa = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    G4int indexb = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth+1);
    G4int index = (indexb*8)+indexa;

    // G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);

    //Add the energy deposition to the temporary edep histogram for this event (will be squared later)
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(2, index, edep);
  }

  return 0;
}

void EdepSquaredEventbyEventScorer::EndOfEvent(G4HCofThisEvent*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  //Pull the edep of this event histogram and its number of bins
  auto edepThisEventHistogram = analysisManager->GetH1(2);
  G4int nBins = analysisManager->GetH1Nbins(2);

  for (G4int i = 0; i <= nBins; i++)
  {
    //null values are for data that we don't need to pull
    G4double doseThisEvent; unsigned int nullInt; G4double nullDouble; 
    
    edepThisEventHistogram->get_bin_content(i, nullInt, doseThisEvent, nullDouble, nullDouble, nullDouble);

    if(doseThisEvent != 0)
    {
      //Square the dose for this event
      doseThisEvent = doseThisEvent*doseThisEvent;
      //Note: why I have to shift the index back to i-1 to fill is unknown. But from manually examining the output, it is working.
      analysisManager->FillH1(1,(i-1),doseThisEvent); //Fill the output histogram with the squared dose of  this event
    }
  }

  //Reset histogram storing energy for this event
  analysisManager->ScaleH1(2,0.0);
}
