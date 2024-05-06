#pragma once

#include "G4VUserDetectorConstruction.hh"

class DetectorConstructionMessenger;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
	DetectorConstruction();
	virtual ~DetectorConstruction() override;

	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField() override;
                         
  private:
	G4VPhysicalVolume* ConstructDetector();    

  DetectorConstructionMessenger* pMessenger{};
};

