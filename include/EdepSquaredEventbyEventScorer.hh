#pragma once

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class EdepSquaredEventbyEventScorer : public G4VPrimitiveScorer
{

 public:
      EdepSquaredEventbyEventScorer(G4String name, G4int depth=0);

      virtual void EndOfEvent(G4HCofThisEvent*);
      
  private: 
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

};
