#pragma once

//Geant4
#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

    G4double GetScoringHalfLength() const { return _scoringHalfLength; }
    G4double GetScoringResolution() const { return _scoringResolution; }
    G4int GetNumBins() const { return _numBins; }

  private:
    //Values that the scoring world grabs to ensure consistency between output and scoring geometry
    G4double _scoringHalfLength;
    G4double _scoringResolution;
    G4int _numBins;
};



