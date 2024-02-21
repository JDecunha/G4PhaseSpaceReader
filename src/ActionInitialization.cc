//MicroTrackGenerator
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
//Geant4
#include "G4RunManager.hh"

ActionInitialization::ActionInitialization()
: G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(new RunAction());
}  

