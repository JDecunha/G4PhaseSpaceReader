#pragma once

#include "G4VUserDetectorConstruction.hh"

class DetectorConstructionMessenger;
class G4VPhysicalVolume;

#include "G4Types.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
	DetectorConstruction(G4String phantomType, G4double phantomHalfLength, G4double scoringResolution);
	virtual ~DetectorConstruction() override;

	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField() override;

	G4double GetPhantomHalfLength() const { return _phantomHalfLength; }
  G4double GetScoringResolution() const { return _scoringResolution; }
  G4String GetPhantomType() const { return _phantomSetup; }
  G4int GetNumBins() const { return _numBins; }

  void SetPhantomHalfLength(G4double halfLength) {_phantomHalfLength = halfLength;};
  void SetScoringResolution(G4double resolution) {_scoringResolution = resolution;};
  void SetPhantomType(G4String phantom) {_phantomSetup = phantom;};
                         
  private:
	G4VPhysicalVolume* ConstructDetector();    
  DetectorConstructionMessenger* pMessenger{};
  
  //Values that the scoring world grabs to ensure consistency between output and scoring geometry
  G4String _phantomSetup;
  G4double _phantomHalfLength;
  G4double _scoringResolution;
  G4int _numBins;

};

