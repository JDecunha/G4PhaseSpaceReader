//This project
#include "RunAction.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"


RunAction::RunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  //Set scoring geometry parameters (the parallel world pulls these parameters to ensure consistency between histograms and scoring geometry)
  _scoringHalfLength = 5*cm;
  _scoringResolution = 0.1*mm;
  _numBins = (_scoringHalfLength*2.)/_scoringResolution;

  //Proton energy spectrum params
  G4double minEnergy = 0.001; //in MeV (the minimum is specified by NIST-PSTAR)
  G4double maxEnergy = 300; //300 MeV, upper end of clinical range
  G4double energySpectrumNbins = 256; //Currently we are spacing logarithmically
  // G4double energyResolution = 0.001; //1 keV resolution (rough 300k bins per histogram though) ... this was so many it brought the program to a halt.
  // G4int energySpectrumNbins = (maxEnergy-minEnergy)/energyResolution;

  //Creating histograms
  //Dose scorer
  analysisManager->CreateH1("Dose" ,"Dose along phantom profile", _numBins, 0, _numBins);
  //Dose uncertainty scorer
  analysisManager->CreateH1("DoseSquaredEventByEvent" ,"Dose squared after each event along phantom profile", _numBins, 0, _numBins);
  analysisManager->CreateH1("DoseSquaredTemporaryEventAccumulator" ,"Holds the dose during a single event before being squared and cleared.", _numBins, 0, _numBins);
  //Proton energy spectrum scorer
  analysisManager->CreateH2("ProtonEnergySpectrum" ,"Holds the proton energy spectrum in each bin.", _numBins, 0, _numBins, energySpectrumNbins,  minEnergy, maxEnergy, "none", "none", "none", "none", "linear", "log");
  analysisManager->CreateH2("ProtonSlowingEnergySpectrum" ,"Holds the proton energy spectrum (for all proton steps) in each bin.", _numBins, 0, _numBins, energySpectrumNbins,  minEnergy, maxEnergy, "none", "none", "none", "none", "linear", "log");
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // auto analysisManager = G4AnalysisManager::Instance();
  // G4String fileName = "test_output.root";
  // analysisManager->OpenFile(fileName);
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  // Ideally, you would deactivate the histogram below so it doesn't save. But the command below doesn't work.
  // analysisManager->SetH1Activation(2, false); //Deactivate the temporary histogram

  analysisManager->Write();
  analysisManager->CloseFile();
}
