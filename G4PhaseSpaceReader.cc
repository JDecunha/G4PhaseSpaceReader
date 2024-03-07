//This project
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhaseSpaceAccessTool.hh"
#include "ParallelWorldConstruction.hh"

//Extern
#include "CommandLineParser.hh"

//Geant4
#include "G4Types.hh"
#include "G4GenericPhysicsList.hh"
#include "G4ParallelWorldPhysics.hh"
#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

//Global command line parser
CommandLineParser* parser(0);

//Definitions for functions in main
void Parse(int& argc, char** argv);
void CheckCommandLineInputs(Command* commandLine);

//Implementation
int main(int argc,char** argv)
{
  //Disable Geant4Backtrace signal handling to improve performance
  G4Backtrace::DefaultSignals() = std::set<int>{};

  // Parse options given in commandLine
  Parse(argc, argv);
  Command* commandLine(0);
  CheckCommandLineInputs(commandLine); //Check that the required command line inputs have been given, if not throw fatal exception

  //I've verified that setting the seed this way works for multithreading,
  //by looking at number of edeps at end of event action in each thread. 
  //Sergio confirms this works too: https://geant4-forum.web.cern.ch/t/different-random-seeds-but-same-results/324
  G4Random::setTheSeed(stol(parser->GetCommandIfActive("-seed")->GetOption())); 

  //Set up RunManager depending on whether multithreading is enabled
  int nThreads = 1;
  #ifdef G4MULTITHREADED
    G4MTRunManager* runManager= new G4MTRunManager;
    if ((commandLine = parser->GetCommandIfActive("-mt")))
    {
      nThreads = 2; //Can't use MTRunManager with less than 2 threads
      if(commandLine->GetOption() == "NMAX")
      {
       nThreads = G4Threading::G4GetNumberOfCores();
      }
      else
      {
       nThreads = G4UIcommand::ConvertToInt(commandLine->GetOption());
      }
      runManager->SetNumberOfThreads(nThreads);
    }
  #else
    G4RunManager* runManager = new G4RunManager();
  #endif
  
  // First time using a G4GenericPhysicsList, this method seems great
  auto PhysicsConstructors = new std::vector<G4String>;
  PhysicsConstructors->push_back("G4EmStandardPhysics_option4"); //EM
  PhysicsConstructors->push_back("G4HadronPhysicsQGSP_BIC_AllHP"); //Inelastic hadron physics (protons, neutrons, pions, kaons)
  PhysicsConstructors->push_back("G4IonBinaryCascadePhysics"); //Inelastic hadron physics (heavy ions)
  PhysicsConstructors->push_back("G4HadronElasticPhysicsHP"); //Elastic hadron physics 
  PhysicsConstructors->push_back("G4DecayPhysics"); //General radioactive decay
  PhysicsConstructors->push_back("G4StoppingPhysics"); //For nuclear capture of anti-particles
  G4GenericPhysicsList* pPhysicsList = new G4GenericPhysicsList(PhysicsConstructors);

  // Set up the phase space access tool (implements a phasespace for multiple threads accessing a phase space file)
  PhaseSpaceAccessTool psAccess;
  if (commandLine = parser->GetCommandIfActive("-phasespace"))
  {
    psAccess.SetPhaseSpacePath(commandLine->GetOption());
  }

  // Set up the geometry (physical and parallel world)
  auto pDetectorConstruction = new DetectorConstruction();
  G4String scoringWorldName = "ScoringWorld";
  pDetectorConstruction->RegisterParallelWorld(new ParallelWorldConstruction(scoringWorldName)); //Connect the detector construction to the parallel world. (This is done if you want to use layered mass geometry)
  pPhysicsList->RegisterPhysics(new G4ParallelWorldPhysics(scoringWorldName));

  // Set mandatory user initialization classes
  runManager->SetUserInitialization(pDetectorConstruction);
  runManager->SetUserInitialization(pPhysicsList);
  runManager->SetUserInitialization(new ActionInitialization()); //Primary generator initialized thread local in here



  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();


  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if (commandLine = parser->GetCommandIfActive("-mac")) //Run the macro file
  {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + commandLine->GetOption());
  }
  
  delete runManager;
  return 0;
}

void CheckCommandLineInputs(Command* commandLine)
{
  //If the input hasn't been given or the input is empty throw a fatal exception
  
  if ((commandLine = parser->GetCommandIfActive("-mac")))
  {
    if (commandLine->GetOption() == "")
    {
        G4ExceptionDescription description;
        description << "Macro file not given. Set with -mac option at runtime" << G4endl;
        G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Macro file NDEF", FatalException, description, "");
    }
  }
  else
  {
        G4ExceptionDescription description;
        description << "Macro file not given. Set with -mac option at runtime" << G4endl;
        G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Macro file NDEF", FatalException, description, "");
  }

  if ((commandLine = parser->GetCommandIfActive("-out")))
  {
    if (commandLine->GetOption() == "")
    {
        G4ExceptionDescription description;
        description << "Output filename not given. Set with -out option at runtime" << G4endl;
        G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Output filename NDEF", FatalException, description, "");
    }
  }
  else
  {
      G4ExceptionDescription description;
      description << "Output filename not given. Set with -out option at runtime" << G4endl;
      G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Output filename NDEF", FatalException, description, "");  
  }

  if ((commandLine = parser->GetCommandIfActive("-seed")))
  {
    if (commandLine->GetOption() == "")
    {
        G4ExceptionDescription description;
        description << "Random seed not given. Set with -seed option at runtime" << G4endl;
        G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Random seed NDEF", FatalException, description, "");
    }
  }
  else
  {
      G4ExceptionDescription description;
      description << "Random seed not given. Set with -seed option at runtime" << G4endl;
      G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Random seed NDEF", FatalException, description, ""); 
  }

  if ((commandLine = parser->GetCommandIfActive("-phasespace")))
  {
    if (commandLine->GetOption() == "")
    {
        G4ExceptionDescription description;
        description << "Phase space not given. Set with -phasespace option at runtime" << G4endl;
        G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Phase space NDEF", FatalException, description, "");
    }
  }
  else
  {
      G4ExceptionDescription description;
      description << "Phase space not given. Set with -phasespace option at runtime" << G4endl;
      G4Exception("G4PhaseSpaceReader::G4PhaseSpaceReader", "Phase space NDEF", FatalException, description, ""); 
  }


}

void Parse(int& argc, char** argv)
{
  //Parse options given in commandLine
  parser = CommandLineParser::GetParser();

  parser->AddCommand("-mac", Command::WithOption,"Give a mac file to execute","");

  parser->AddCommand("-seed",Command::WithOption,"Provide a seed for the random engine","");

  parser->AddCommand("-out",Command::WithOption,"Output filename","");

  parser->AddCommand("-phasespace",Command::WithOption,"Path to phase space file","");

#ifdef G4MULTITHREADED
  parser->AddCommand("-mt",Command::WithOption,"Launch in MT mode (events computed in parallel)","");
#endif

  if (parser->Parse(argc, argv) != 0) //Exit the application if some condition is met. Not 100% clear on the condition.
  {
    CommandLineParser::DeleteInstance();
    std::exit(0);
  }

  // Kill application if wrong argument in command line
  if (parser->CheckIfNotHandledOptionsExists(argc, argv))
  {
    abort();
  }
}
