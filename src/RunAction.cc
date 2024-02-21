//This project
#include "RunAction.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"


RunAction::RunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(false);

  // Creating histograms
  analysisManager->CreateH1("Dose" ,"Dose along phantom profile", 201, 0, 200);
  analysisManager->CreateH1("DoseUncertainty" ,"Dose Uncertainty along phantom profile", 101, 0, 100);
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