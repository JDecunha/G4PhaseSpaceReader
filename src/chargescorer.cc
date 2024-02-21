#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#include "chargescorer.hh"
#include "G4VSolid.hh"
#include "G4String.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4VTouchable.hh"

//G4int numEnergyBins = 1400;
//G4int numEnergyBinsLow = 10;
//G4double widthEnergyBinHigh = 1*keV;
//G4double widthEnergyBinLow = 0.1*keV;
//static std::vector<G4double> binCounts (50*numEnergyBins,0);

//G4String filename = "PrimEl.csv";
//std::ofstream out(filename.data());


/*
from the documentation:
generates a G4THitsMap<G4double> that maps a G4double value to its key integer number. 
By default, the key is taken as the copy number of the G4LogicalVolume to which G4MultiFunctionalDetector is assigned. 
In case the logical volume is uniquely placed in its mother volume and the mother is replicated, 
the copy number of its mother volume can be taken by setting the second argument of the G4VPrimitiveScorer constructor, "depth" to 1, i.e. one level up.
*/

chargescorer::chargescorer(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1) {
  SetUnit("Gy");

}


chargescorer::chargescorer(G4String name, const G4String& unit,G4int depth):G4VPrimitiveScorer(name,depth),HCID(-1) 
{
  SetUnit(unit);
}

chargescorer::~chargescorer() 
{
  
}
//Determines whether a charged particle has just changed logical volumes
//If so adds / subtracts (leaving/entering) charge of particle
G4bool chargescorer::ProcessHits(G4Step* aStep,G4TouchableHistory*) 
{

    std::cout << "Hit!" << std::endl;
    G4double charge =  aStep->GetTrack()->GetDefinition()->GetPDGCharge();
    G4double negcharge = -charge;
    G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    G4int postindex = ((G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);


   if (charge != 0) //check the particle has charge
   {
 
    if (index != postindex) //check the particle has changed objects
    {
    G4String upperindex = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    G4String postuppderindex = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();

    G4String prevolume = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    G4String postvolume = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();

      if (prevolume != "ChargeSensitiveRegion") { EvtMap->add(index,charge); return true; }

      if (postvolume != "ChargeSensitiveRegion") { EvtMap->add(index,negcharge); return true; }

    EvtMap->add(index,negcharge); //subtract the charge from the voxel it is just leaving
    EvtMap->add(postindex,charge); //add the charge to the voxel it is now entering
    
    }

  }

 return false;

}

void chargescorer::Initialize(G4HCofThisEvent* HCE) {
    EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),GetName());
    if(HCID < 0) {HCID = GetCollectionID(0);}
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void chargescorer::EndOfEvent(G4HCofThisEvent*) {;}

void chargescorer::clear() {
    EvtMap->clear();
}

void chargescorer::DrawAll() {;}

void chargescorer::PrintAll() {
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
       << "  dose deposit: "
       << *(itr->second)/GetUnitValue()
       << " ["<<GetUnit() <<"]"
       << G4endl;
  }
}

void chargescorer::SetUnit(const G4String& unit) {
    CheckAndSetUnit(unit,"Dose");
}

G4double chargescorer::ComputeVolume(G4Step* aStep, G4int idx) {
  G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VPVParameterisation* physParam = physVol->GetParameterisation();
  G4VSolid* solid = 0;
  if(physParam) { // for parameterized volume
      if(idx<0) {
          G4ExceptionDescription ED;
          ED << "Incorrect replica number --- GetReplicaNumber : " << idx << G4endl;
          G4Exception("chargescorer::ComputeVolume","DetPS0004",JustWarning,ED);
      }

      solid = physParam->ComputeSolid(idx, physVol);
      solid->ComputeDimensions(physParam,idx,physVol);
  } else {
      solid = physVol->GetLogicalVolume()->GetSolid();
  }
  //This returns an estimation of the volume, even though it's "cubic" I believe it's still accurate
  //Returns a cubic estimation
  return solid->GetCubicVolume();
}