#pragma once

//Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

class Eppendorf_48WellPlate_Model
{
  public:
	void Construct(G4LogicalVolume* motherVol, G4double phantomHalfLength);
	void ConstructLiquidandScorers(G4LogicalVolume* motherVol, G4double volumeMl, G4double phantomHalfLength);

};

