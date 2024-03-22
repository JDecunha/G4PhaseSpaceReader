//This project
#include "StraightLineParameterisation.hh"
//Geant4
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
//Standard library


//Word to the wise, xinc,yinc, and zinc have to be even otherwise things won't work quite as planned
StraightLineParameterisation::StraightLineParameterisation(const G4double& Halfx, const G4double& Halfy, const G4double& Halfz, const G4int& zInc)
:G4VPVParameterisation(),_halfx(Halfx),_halfy(Halfy),_halfz(Halfz), _zinc(zInc)
{
    if(_zinc%2 != 0) { G4cout << "StraightLineParameterisation increment numbers must be even. Warning!" << G4endl; }
}

StraightLineParameterisation::~StraightLineParameterisation() {}

void StraightLineParameterisation::ComputeDimensions(G4Box& box, const G4int copyNo, const G4VPhysicalVolume*) const
{
    box.SetXHalfLength(_halfx);
    box.SetYHalfLength(_halfy);
    box.SetZHalfLength((_halfz/_zinc)*0.98); //Introduce the "fudge factor" to give a little spacing between boxes
}

void StraightLineParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physical) const
{
    znum = copyNo;

    //This is an error statment, but also a break statement. Once x,y, and z have been incremented over. break.
    if(copyNo > (_zinc)) { G4cout << "Too many voxels entered into StraightLineParameterisation, Voxel #:" << copyNo+1 << G4endl; return;}

    G4double xpos = 0.;
    G4double ypos = 0.;
    G4double zpos = (_halfz/_zinc) + (znum*((2*_halfz)/_zinc)); 
    zpos = -zpos; //Lower surface at 0, increments in -Z direction by voxel length

    G4ThreeVector origin(xpos, ypos, zpos+1.7*cm); //1.7 cm shift explained in detector construction 
    physical->SetTranslation(origin);
    physical->SetRotation(0);

}