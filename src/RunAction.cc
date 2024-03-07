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
  analysisManager->SetNtupleMerging(false);

  //Set scoring geometry parameters (the parallel world pulls these parameters to ensure consistency between histograms and scoring geometry)
  _scoringHalfLength = 5*cm;
  _scoringResolution = 0.5*mm;
  _numBins = (_scoringHalfLength*2.)/_scoringResolution;

  //Creating histograms
  //Dose scorer
  analysisManager->CreateH1("Dose" ,"Dose along phantom profile", _numBins+1, 0, _numBins);
  analysisManager->CreateH1("NumEvents" ,"Number of events along phantom profile", _numBins+1, 0, _numBins);
  //Dose uncertainty scorer
  analysisManager->CreateH1("DoseSquaredEventByEvent" ,"Dose squared after each event along phantom profile", _numBins+1, 0, _numBins);
  analysisManager->CreateH1("DoseSquaredTemporaryEventAccumulator" ,"Holds the dose during a single event before being squared and cleared.", _numBins+1, 0, _numBins);
  //Proton energy spectrum scorer
  //TODO: Make my H2 here later
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "test_output.csv";
  analysisManager->OpenFile(fileName);
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}