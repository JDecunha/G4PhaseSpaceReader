#pragma once

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class ProtonSpectrumScorer : public G4VPrimitiveScorer
{
 public:
      ProtonSpectrumScorer(G4String name, G4int depth=0);
      
  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

};
