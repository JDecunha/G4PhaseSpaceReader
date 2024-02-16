#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
//Standard Library
#include <fstream>
#include <iostream>


class G4ParticleGun;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* event);

  private:
    friend class RunAction;

    G4ParticleGun* gun{};
    PrimaryGeneratorMessenger* pPrimaryMessenger{};

};

