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
  // std::cout << "Before primary gen" << std::endl;
  SetUserAction(new PrimaryGeneratorAction());
  // std::cout << "Before per thread run action" << std::endl;
  SetUserAction(new RunAction());
  // std::cout << "After per thread run action" << std::endl;
}  

