//This project
#include "RunAction.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4AccumulableManager.hh"
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

  //Let's try making our custom accumulable. We should test it with dose actually!
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  doseAccumulable = new G4VectorAccumulable<G4double>("doseAccumulable", 200); 
  accumulableManager->RegisterAccumulable(doseAccumulable);
}

RunAction::~RunAction()
{
  delete doseAccumulable;
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

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  //Let's print the dose accumulable to see if it's working
  if(isMaster)
  {
    G4VectorAccumulable<G4double>* doseAccum = static_cast<G4VectorAccumulable<G4double>*>(accumulableManager->GetAccumulable("doseAccumulable"));
    auto doseVector = doseAccum->GetVector();
    for (const auto& item : doseVector)
    {
      std::cout << item << std::endl;
    }
  }
}