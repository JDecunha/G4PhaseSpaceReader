//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
//Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction()
{
  pMessenger = new DetectorConstructionMessenger(this);
}  

DetectorConstruction::~DetectorConstruction()
{
  delete pMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructDetector();
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
  //Define water material
  G4NistManager * man = G4NistManager::Instance();
  G4Material * H2O = man->FindOrBuildMaterial("G4_WATER");

  //Create the world volume
  double sideLength = 5*cm;
  G4VSolid* solidWorld = new G4Box("World", sideLength/2,sideLength/2,sideLength);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,  //its solid
                                    H2O,  //its material
                                    "World");    //its name

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(),  //at (0,0,0), a requirement of the mother volume
                                  "World",    //its name
                                  logicWorld,    //its logical volume
                                  0,      //its mother  volume
                                  false,      //no boolean operation
                                  0);      //copy number
  
  return physiWorld;
}
