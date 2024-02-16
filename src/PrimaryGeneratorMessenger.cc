//MicroTrackGenerator
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
//Geant4
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* primary_action) : pPrimaryGenerator(primary_action)
{
  
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{

}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String value)
{

}

