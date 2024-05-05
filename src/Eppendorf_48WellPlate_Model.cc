//MicroTrackGenerator
#include "Eppendorf_48WellPlate_Model.hh"
//Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4VisAttributes.hh"

void Eppendorf_48WellPlate_Model::Construct(G4LogicalVolume* motherVol)
{
  //Define materials
  G4NistManager * man = G4NistManager::Instance();
  G4Material* GPPS = man->FindOrBuildMaterial("G4_POLYSTYRENE");

  //The overall plate dimensions
  double width = 85.5*mm;
  double length = 127.8*mm;
  double bt = 1.366666*mm;
  double h2 = 17.0*mm;
  double height = h2+bt; // The total thickness of the plate is h2 + bt (where I've taken bt from Madison's measurement)
  //Madison's bt = 1.36666 mm, diagram bt = 1.4 mm, so basically the same.

  //
  // Solid materials
  //

  G4VSolid* solidWellPlate = new G4Box("WellPlate", width/2, length/2, height/2);

  // G4Cons(const G4String& pName,
  //             G4double  pRmin1,
  //             G4double  pRmax1,
  //             G4double  pRmin2,
  //             G4double  pRmax2,
  //             G4double  pDz,
  //             G4double  pSPhi,
  //             G4double  pDPhi)

  // G4VSolid* solidWell = new G4Tubs("Well", 0., (10.7/2.)*mm, h2/2, 0., 2*M_PI*rad); //diameter 10.7 mm, height 17.0 mm
  G4VSolid* solidWell = new G4Cons("Well", 0., (10.4/2.)*mm, 0., (10.7/2.)*mm,  h2/2, 0., 2*M_PI*rad); //diameter 10.7 mm, height 17.0 mm

  
  //
  // Boolean solid
  //

  double initialWidthOffset = 10.2*mm;
  double initialLengthOffset = 18.4*mm;

  //So the cylinders start centered around the center of the plate
  //The cylinders are 17.0 mm tall, and the plate is 18.3666 tall. So there is 0.68333 mm spacing on each side
  //I just need to move the cylinders up by this 0.6833333 mm. So there is bt below them and they are exposed to the air on top.
  double initialHeightOffset = bt/2; 

  double widthSpacing = 13.0*mm; //spacing between adjacent wells
  double heightSpacing = 13.0*mm; //spacing between adjaent wells

  double widthCoordinateSystemShift = width/2; //to move the coordinate system from the center of the plate to the bottom corner
  double lengthCoordinateSystemShift = length/2; //to move the coordinate system from the center of the plate to the bottom corner

  for (int Row = 0; Row < 8; ++Row)
  {

    for (int Column = 0; Column < 6; ++Column)
    {
      double width = initialWidthOffset + (widthSpacing*Column) - widthCoordinateSystemShift;
      double length = initialLengthOffset + (heightSpacing*Row) - lengthCoordinateSystemShift;
      double height = initialHeightOffset; // I don't need to shift the height into a new coordinate system because it is already centered
      // The width and length however I am using the top right corner as the initial reference

      solidWellPlate = new G4SubtractionSolid("Well_MinusWellPlate", solidWellPlate, solidWell, 0, G4ThreeVector(width, length, height));
    }

    
  }
  
  // Should shift the plate up by 1.6 mm so that its position matches the diagram
  // Does this mean there will be an air gap on the bottom of the plate?


  G4LogicalVolume* logicWellPlate = new G4LogicalVolume(solidWellPlate,  //its solid
                                    GPPS,  //its material
                                    "WellPlate_logical");    //its name

  G4VPhysicalVolume* physicalWellPlate = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(0.,0.,1.6*mm),  //at (0,0,0), a requirement of the mother volume
                                  logicWellPlate,    //its logical volume
                                  "WellPlate_world",    //its name
                                  motherVol,      //its mother  volume
                                  false,      //no boolean operation
                                  0, false);      //copy number

  //Visualize Wells plate
  G4VisAttributes* scoring_vis = new G4VisAttributes();
  scoring_vis->SetColor(1, 0, 0);
  scoring_vis->SetVisibility(true);
  logicWellPlate->SetVisAttributes(scoring_vis);

  //Visualize Plate
  // G4VisAttributes* second_vis = new G4VisAttributes();
  // second_vis->SetColor(1, 0, 0);
  // second_vis->SetVisibility(true);
  // logicTubs->SetVisAttributes(second_vis);

  // (0, phantomOffset, LucitePhantom_log, "LucitePhantom", logicWorld, false, 0, false);

}