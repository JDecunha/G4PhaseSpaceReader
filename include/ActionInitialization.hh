#pragma once

#include "G4VUserActionInitialization.hh"
#include "G4Types.hh"

class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const override;
    virtual void Build() const override;
};

    
