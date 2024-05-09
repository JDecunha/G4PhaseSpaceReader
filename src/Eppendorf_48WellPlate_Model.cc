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

  G4VSolid* solidWellPlate = new G4Box("WellPlate", length/2, width/2,  height/2);

  double lidWidth = 84.3*mm;
  double lidLength = 127.6*mm;
  double lidHeight = bt;
  G4VSolid* solidLid = new G4Box("Lid", lidLength/2, lidWidth/2,  lidHeight/2);

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
      double widthShift = initialWidthOffset + (widthSpacing*Column) - widthCoordinateSystemShift;
      double lengthShift = initialLengthOffset + (heightSpacing*Row) - lengthCoordinateSystemShift;
      double heightShift = initialHeightOffset; // I don't need to shift the height into a new coordinate system because it is already centered
      // The width and length however I am using the top right corner as the initial reference

      solidWellPlate = new G4SubtractionSolid("Well_MinusWellPlate", solidWellPlate, solidWell, 0, G4ThreeVector(lengthShift, widthShift, -heightShift));
    }

    
  }
  
  // Should shift the plate up by h-(h2+bt) (approx 1.6 mm) mm so that its position matches the diagram
  // There will be an air gap on the bottom of the plate

  G4LogicalVolume* logicWellPlate = new G4LogicalVolume(solidWellPlate,  //its solid
                                    GPPS,  //its material
                                    "WellPlate_logical");    //its name

  G4VPhysicalVolume* physicalWellPlate = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(0.,0.,-((20*mm)-height)),  //Shift up because the wells have a tiny air gap under them
                                  logicWellPlate,    //its logical volume
                                  "WellPlate_physical",    //its name
                                  motherVol,      //its mother  volume
                                  false,      //no boolean operation
                                  0, false);      //copy number

  // Place the lid
  G4LogicalVolume* logicLid = new G4LogicalVolume(solidLid,  //its solid
                                    GPPS,  //its material
                                    "Lid_logical");    //its name

  G4VPhysicalVolume* physicalLid = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(0.,0.,-(((20*mm)-height)+(height/2)+(lidHeight/2))),  //Shift up because the wells have a tiny air gap under them
                                  logicLid,    //its logical volume
                                  "Lid_physical",    //its name
                                  motherVol,      //its mother  volume
                                  false,      //no boolean operation
                                  0, false);      //copy number

  //Visualize Well plate
  G4VisAttributes* plate_vis = new G4VisAttributes();
  plate_vis->SetColor(1, 0, 0);
  plate_vis->SetVisibility(true);
  logicWellPlate->SetVisAttributes(plate_vis);

  //Visualize Lid
  G4VisAttributes* lid_vis = new G4VisAttributes();
  lid_vis->SetColor(0.902, 0.416, 0.055);
  lid_vis->SetVisibility(true);
  logicLid->SetVisAttributes(lid_vis);

}

void Eppendorf_48WellPlate_Model::ConstructLiquidandScorers(G4LogicalVolume* motherVol, G4double volumeMl, G4double phantomThickness)
{
  //Define materials
  G4NistManager * man = G4NistManager::Instance();
  G4Material* Water = man->FindOrBuildMaterial("G4_WATER");

  //The overall plate dimensions
  double width = 85.5*mm;
  double length = 127.8*mm;
  double bt = 1.366666*mm;
  double h2 = 17.0*mm;
  double height = h2+bt; // The total thickness of the plate is h2 + bt (where I've taken bt from Madison's measurement)
  //Madison's bt = 1.36666 mm, diagram bt = 1.4 mm, so basically the same.

  // Volume of a cylinder =  pi * r^2 * h
  // h = (volume)/(pi*r^2);
  double liquidHeight = (volumeMl*1000)/(3.14159*10.4*mm*10.4*mm);
  double scorerHeight = 1.0*mm;
  liquidHeight = liquidHeight-scorerHeight;


  G4VSolid* solidLiquid = new G4Tubs("Liquid", 0., (10.4/2.)*mm, liquidHeight/2, 0., 2*M_PI*rad); //diameter 10.7 mm, height 17.0 mm
  G4VSolid* solidScorer = new G4Tubs("Scorer", 0., (10.4/2.)*mm, scorerHeight/2, 0., 2*M_PI*rad); //diameter 10.7 mm, height 17.0 mm

  G4LogicalVolume* logicLiquid = new G4LogicalVolume(solidLiquid,  //its solid
                                    Water,  //its material
                                    "Liquid_Logical");    //its name

  G4LogicalVolume* logicScorer = new G4LogicalVolume(solidScorer,  //its solid
                                    Water,  //its material
                                    "Scorer_Logical");    //its name


  
  //
  // Add liquid
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
      double widthShift = initialWidthOffset + (widthSpacing*Column) - widthCoordinateSystemShift;
      double lengthShift = initialLengthOffset + (heightSpacing*Row) - lengthCoordinateSystemShift;
      double heightShift = initialHeightOffset; // I don't need to shift the height into a new coordinate system because it is already centered
      // The width and length however I am using the top right corner as the initial reference

      double plateUpwardShift = (20*mm)-height;

      G4int copyNo = Row*8+Column;

      //Logical rationale for the height shift below:
      //1.) Plate upward shift shifts us up the 1.7 mm the plate is shifted upwards (center is sitting on the center of the plate now)
      //2.) Subtracting height/2 takes us to the bottom of the plate (center is sitting on the bottom of the plate now)
      //3.) Adding bt takes the center to the bottom of the well (center is sitting on the bottom of the well now)
      //4.) Adding scorer height takes us up ~1 mm above the scorer
      //5.) Adding liquidHeight/2 takes us up so the bottom of the liquid is on top of the scorer

      G4VPhysicalVolume* physicalLiquid = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(lengthShift, widthShift, -(plateUpwardShift-(height/2)+bt+scorerHeight+(liquidHeight/2))),  //Shift up because the wells have a tiny air gap under them
                                  logicLiquid,    //its logical volume
                                  "Liquid_physical",    //its name
                                  motherVol,      //its mother  volume
                                  false,      
                                  copyNo, false);     

      G4VPhysicalVolume* physicalScorer = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(lengthShift, widthShift, -(plateUpwardShift-(height/2)+bt+(scorerHeight/2))),  //Shift up because the wells have a tiny air gap under them
                                  logicScorer,    //its logical volume
                                  "Scorer_physical",    //its name
                                  motherVol,      //its mother  volume
                                  false,      
                                  copyNo, false);     
    }

    
  }

  
  
  //Visualize 
  G4VisAttributes* liquid_Vis = new G4VisAttributes();
  liquid_Vis->SetColor(0, 0, 1);
  liquid_Vis->SetVisibility(true);
  logicLiquid->SetVisAttributes(liquid_Vis);

  G4VisAttributes* scorer_Vis = new G4VisAttributes();
  scorer_Vis->SetColor(1, 1, 1);
  scorer_Vis->SetVisibility(true);
  logicScorer->SetVisAttributes(scorer_Vis);

}