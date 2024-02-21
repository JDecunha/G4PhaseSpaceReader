#pragma once

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class EdepUncertaintyScorer : public G4VPrimitiveScorer
{
 public:
      EdepUncertaintyScorer(G4String name, G4int depth=0);

      virtual void EndOfEvent(G4HCofThisEvent*);
      
  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

};
