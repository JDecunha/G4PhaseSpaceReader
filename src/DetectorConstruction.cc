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
#include "G4PVParameterised.hh"

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
  //Define materials
  G4NistManager * man = G4NistManager::Instance();
  G4Material* air = man->FindOrBuildMaterial("G4_AIR");
  G4Material* lucite = man->FindOrBuildMaterial("G4_LUCITE");

  //Create the world volume
  double sideLength = 100*cm; //The phase space is 48 cm from ISO  
  G4VSolid* solidWorld = new G4Box("World", sideLength/2,sideLength/2,sideLength);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,  //its solid
                                    air,  //its material
                                    "World");    //its name

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(),  //at (0,0,0), a requirement of the mother volume
                                  "World",    //its name
                                  logicWorld,    //its logical volume
                                  0,      //its mother  volume
                                  false,      //no boolean operation
                                  0);      //copy number

  //Create the phantom
  G4double xhalfsize = 8.0*cm; //16 cm x 12 cm x 10 cm thick
  G4double yhalfsize = 6.0*cm;
  G4double zhalfsize = 10.0*cm;
  G4Box* LucitePhantom = new G4Box("LucitePhantom", xhalfsize, yhalfsize, zhalfsize); //Initial phase space covers 13 cm x 9 cm (i.e 6.5 x 4.5 half length)
  G4LogicalVolume* LucitePhantom_log = new G4LogicalVolume(LucitePhantom, lucite,"LucitePhantom_log");
  G4VPhysicalVolume* LucitePhantom_physical = new G4PVPlacement(0, G4ThreeVector(0,0,0), LucitePhantom_log, "LucitePhantom", logicWorld, false, 0, false);

  return physiWorld;
}
