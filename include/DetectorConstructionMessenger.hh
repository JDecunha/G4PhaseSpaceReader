#pragma once

#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class DetectorConstructionMessenger : public G4UImessenger
{
  public:
	DetectorConstructionMessenger(DetectorConstruction* detector_construction);
	virtual ~DetectorConstructionMessenger();

	void SetNewValue(G4UIcommand* command, G4String value);
                         
  private:
	DetectorConstruction* pDetectorConstruction{};
	G4UIdirectory* geometry_directory{};
	G4UIcmdWithAString* geometry_type{};
	G4UIcmdWithADoubleAndUnit* phantom_thickness{};
	G4UIcmdWithADoubleAndUnit* scoring_resolution{};
};
