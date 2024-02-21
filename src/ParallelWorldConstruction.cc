#include "ParallelWorldConstruction.hh"
//Geant4
#include "G4SystemOfUnits.hh"
//This project
#include "ParallelWorldConstruction.hh"
#include "StraightLineParameterisation.hh"
//Geant4
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

ParallelWorldConstruction::ParallelWorldConstruction(const G4String& parallelWorldName): G4VUserParallelWorld(parallelWorldName) { }

void ParallelWorldConstruction::Construct() 
{
  //Get the world pointers (required for parallel worlds)
  G4VPhysicalVolume* motherWorldPointer = GetWorld();
  G4LogicalVolume* motherWorldLogical = motherWorldPointer->GetLogicalVolume();

  //Define G4Box that the parameterisation will slice up
  G4double xhalfsize = 1.5*mm; 
  G4double yhalfsize = 1.5*mm;
  G4double zhalfsize = 10*cm;

  G4Box* scoringBox = new G4Box("scoringBox", xhalfsize, yhalfsize, zhalfsize);
  G4LogicalVolume * scoringBox_logical = new G4LogicalVolume(scoringBox, 0, "scoringBox_logical", 0, 0, 0);

  //Slice up the scoring box
  G4double zAxisResolution = 0.5*mm;
  G4int zincrements = (zhalfsize*2.)/zAxisResolution; //Set zincrements according to desired resolution 
  G4int xincrements = 1; G4int yincrements = 1;
  G4int nrhovoxels = xincrements*yincrements*zincrements;

  StraightLineParameterisation* lineParam = new StraightLineParameterisation(xhalfsize,yhalfsize,zhalfsize,zincrements);
  G4PVParameterised* phantomVoxels = new G4PVParameterised("voxelsAlongLine", scoringBox_logical, motherWorldLogical, kZAxis, nrhovoxels, lineParam);

}
