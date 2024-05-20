#include <string>
#include <iostream>
//This simulation
#include "LETSpectrumScorer.hh"
//Geant4
#include "G4AnalysisManager.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTouchable.hh"


LETSpectrumScorer::LETSpectrumScorer(G4String name, G4int depth):G4VPrimitiveScorer(name,depth)
{
  _stoppingPowerTable = PullStoppingPowersFromCsv();
}

G4bool LETSpectrumScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*) 
{
  //Filtering by:
  //1.) First step in volume
  //2.) Particle type is proton

  if(aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary && aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "proton")
  {
    G4int indexa = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    G4int indexb = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth+1);
    G4int index = (indexb*120)+indexa;
    G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
    G4double LET = LookupLET(energy);
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(1, index, LET);
  }

  return 0;
}

G4double LETSpectrumScorer::LookupLET(G4double protonEnergy)
{

  double LastPercentDiff = 1e6;

  for (int pStarLoopIndex = 0; pStarLoopIndex < std::get<0>(_stoppingPowerTable).size(); ++pStarLoopIndex) //Loop over PSTAR to find the Stopping power
  {

    double pStarEnergyValue = std::get<0>(_stoppingPowerTable)[pStarLoopIndex];
    double percentDiff = (std::fabs(pStarEnergyValue - protonEnergy)/protonEnergy)*100;
    double change = percentDiff - LastPercentDiff;

    if (change > 0.) //We found entry in PSTAR if true
    {
      G4double LET = std::get<1>(_stoppingPowerTable)[pStarLoopIndex-1];
      LET = LET*0.1; //convert to keV/um;
      return LET;
    }

    LastPercentDiff = percentDiff;
  }

  std::cout << "Issue! energy match not found" << std::endl;
  return -1;
}

std::pair<std::vector<double>,std::vector<double>> LETSpectrumScorer::PullStoppingPowersFromCsv()
{
  //The two vectors that get filled
  std::vector<double> protonEnergy;
  std::vector<double> protonElecStoppingPower;

  //The path, an input stream, and a temp to hold the line being readout
  std::string filePath = "/rsrch3/home/radphys_rsch/jdecunha/seadragon2/G4PhaseSpaceReader/data/PSTAR_LETs_512.csv";
  // std::string filePath = "/rsrch3/home/radphys_rsch/jdecunha/seadragon2/G4PhaseSpaceReader/data/PSTAR_LETs_512.csv";
  std::ifstream SPFile(filePath);
  std::string ReadoutLine;

  while (std::getline(SPFile,ReadoutLine)) //Split the file by lines
  {
    //Split the line by words
    std::istringstream spaceSplitter(ReadoutLine); std::string word;
    
    //Get the two comma separated values and fill
    std::getline(spaceSplitter,word, ',');
    protonEnergy.push_back(std::stod(word));
    std::getline(spaceSplitter,word, ',');
    protonElecStoppingPower.push_back(std::stof(word));
  }

  // for (const auto& val : protonElecStoppingPower)
  // {
  //  std::cout << val << std::endl;
  // }

  return std::make_pair(protonEnergy,protonElecStoppingPower);
}
