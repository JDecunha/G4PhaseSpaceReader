#pragma once

//Geant4
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

  private:
    G4String _phantomSetup;
    G4double _scoringHalfLength;
    G4double _scoringResolution;
    G4int _numBins;
    
};



