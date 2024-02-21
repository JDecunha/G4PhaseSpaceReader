#pragma once

//Geant4
#include "globals.hh"
#include "G4VUserParallelWorld.hh"

class ParallelWorldConstruction : public G4VUserParallelWorld {
  public:
  ParallelWorldConstruction(const G4String& parallelWorldName);

  ~ParallelWorldConstruction() { }

  virtual void Construct();

  private:

};

