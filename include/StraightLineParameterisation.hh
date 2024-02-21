#pragma once

#include "G4VPVParameterisation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>

class G4LogicalVolume;

class StraightLineParameterisation : public G4VPVParameterisation {

public:
  
  // Constructor
  StraightLineParameterisation(const G4double& Halfx, const G4double& Halfy, const G4double& Halfz, const G4int& zInc);
  
  // Destructor
  virtual ~StraightLineParameterisation();
  
  // Method
  virtual void ComputeDimensions(G4Box& box, const G4int copyNo, const G4VPhysicalVolume*) const;
  virtual void ComputeTransformation(const G4int, G4VPhysicalVolume*) const;
    
private:
  
  G4double _halfx, _halfy, _halfz;
  G4int _zinc;
  
  // Mutable keyword is used for internal counters
  // The const methods above can only modify parameters marked with mutable
  mutable G4int znum;

};