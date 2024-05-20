//This project
#include "RunAction.hh"
#include "DetectorConstruction.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"
#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif


RunAction::RunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  // Pull user inputs from detector construction
  DetectorConstruction* pDetectorConstruction = ((DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double phantomHalfSize = pDetectorConstruction->GetPhantomHalfLength();
  G4double scoringResolution = pDetectorConstruction->GetScoringResolution();
  G4String _phantomType = pDetectorConstruction->GetPhantomType();

  //Proton energy spectrum params
  G4double minEnergy = 0.001; //in MeV (the minimum is specified by NIST-PSTAR)
  G4double maxEnergy = 300; //300 MeV, upper end of clinical range
  G4double energySpectrumNbins = 512; //Currently we are spacing logarithmically
  G4double minLET = 0.3;
  G4double maxLET = 100;

  _numBins = 0;

  if (_phantomType == "LuciteBlock")
  {
    // std::cout << "RunAction: Phantom type is LuciteBlock" << std::endl;
    _numBins = (phantomHalfSize*2.)/scoringResolution;
  }
  else if (_phantomType == "WellPlate")
  {
    // std::cout << "RunAction: Phantom type is WellPlate" << std::endl;
    _numBins = 48; // 48 well plate

  }
  else if (_phantomType == "LuciteBlockHighRes")
  {
    // std::cout << "RunAction: Phantom type is LuciteBlockHighRes" << std::endl;
    _numBins = (60*mm)/scoringResolution; // The 60 mm magic number comes from us scoring from 37 to 43 mm.
  }
  else if (_phantomType == "LateralScoring")
  {
    _numBins = 120*160; 
  }
  else if (_phantomType == "LateralScoringWide")
  {
    _numBins = 50*40; 
  }
  else
  {
    std::cout << "RunAction: Phantom type " << _phantomType << " is unknown." << std::endl;
  }

  //Creating histograms
  //Dose scorer
  analysisManager->CreateH1("Dose" ,"Dose along phantom profile", _numBins, 0, _numBins);
  //Dose uncertainty scorer
  analysisManager->CreateH1("DoseSquaredEventByEvent" ,"Dose squared after each event along phantom profile", _numBins, 0, _numBins);
  analysisManager->CreateH1("DoseSquaredTemporaryEventAccumulator" ,"Holds the dose during a single event before being squared and cleared.", _numBins, 0, _numBins);
  //Proton energy spectrum scorer
  // (const G4String &name, const G4String &title, G4int nxbins, G4double xmin, G4double xmax, G4int nybins, G4double ymin, G4double ymax, const G4String &xunitName="none", const G4String &yunitName="none", const G4String &xfcnName="none", const G4String &yfcnName="none")=0
  analysisManager->CreateH2("ProtonEnergySpectrum" ,"Holds the proton energy spectrum in each bin.", _numBins, 0, _numBins, energySpectrumNbins,  minEnergy, maxEnergy, "none", "none", "none", "none", "linear", "log");
  // analysisManager->CreateH2("ProtonSlowingEnergySpectrum" ,"Holds the proton energy spectrum (for all proton steps) in each bin.", _numBins, 0, _numBins, energySpectrumNbins,  minEnergy, maxEnergy, "none", "none", "none", "none", "linear", "log");
  analysisManager->CreateH2("LETSpectrum" ,"LET spectrum in each bin.", _numBins, 0, _numBins, energySpectrumNbins,  minLET, maxLET, "none", "none", "none", "none", "linear", "log");
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  // Ideally, you would deactivate the histogram below so it doesn't save. But the command below doesn't work.
  // analysisManager->SetH1Activation(2, false); //Deactivate the temporary histogram

  analysisManager->Write();
  analysisManager->CloseFile();
}
