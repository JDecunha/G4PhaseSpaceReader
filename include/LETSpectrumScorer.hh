#pragma once

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <vector>

class LETSpectrumScorer : public G4VPrimitiveScorer
{
 public:
      LETSpectrumScorer(G4String name, G4int depth=0);
      
  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      std::pair<std::vector<double>,std::vector<double>> PullStoppingPowersFromCsv();
      G4double LookupLET(G4double protonEnergy);

  private:
    std::pair<std::vector<double>,std::vector<double>> _stoppingPowerTable;


};
