//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
//Geant4
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* detector_construction) : pDetectorConstruction(detector_construction)
{
//     // // Directories in which commands are organised.
//     geometry_directory = new G4UIdirectory("/geometry/");
//     geometry_directory->SetGuidance("Commands relating to runAction / scoring.");

//     // Set the phantom type
//     geometry_type = new G4UIcmdWithAString("/geometry/phantom", this);
//     geometry_type->SetGuidance("Set the phantom type either (Wellplate) or (LuciteBlock) or (LuciteHighRes).");
//     geometry_type->SetParameterName("phantom", false);
//     geometry_type->AvailableForStates(G4State_PreInit);

//     // Set the phantom thickness
//     phantom_thickness = new G4UIcmdWithADoubleAndUnit("/geometry/thickness", this);
//     phantom_thickness->SetGuidance("Set the phantom thickness.");
//     phantom_thickness->SetParameterName("thickness", false);
//     phantom_thickness->AvailableForStates(G4State_PreInit);

//     // Set the phantom thickness
//     scoring_resolution = new G4UIcmdWithADoubleAndUnit("/geometry/scoringResolution", this);
//     scoring_resolution->SetGuidance("Set the scoring resolution.");
//     scoring_resolution->SetParameterName("resolution", false);
//     scoring_resolution->AvailableForStates(G4State_PreInit);
}


DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
//     delete geometry_directory;
//     delete geometry_type;
//     delete phantom_thickness;
//     delete scoring_resolution;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    // std::cout << "AAAAAA" << std::endl;

    // if (command == geometry_type)
    // {
    //   pDetectorConstruction->SetPhantomType(value);
    //   std::cout << "PHANTOM TYPE SET: " << value << std::endl;
    // }
    // else if (command == phantom_thickness)
    // {
    //   G4double thickness = phantom_thickness->GetNewDoubleValue(value.c_str());
    //   G4double halfLength = thickness/2;
    //   pDetectorConstruction->SetScoringHalfLength(halfLength);
    // }
    // else if (command == scoring_resolution)
    // {
    //   pDetectorConstruction->SetScoringResolution(scoring_resolution->GetNewDoubleValue(value.c_str()));
    // }
}

