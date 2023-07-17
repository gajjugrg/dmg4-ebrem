#include "globals.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingActionDMG4.hh"

#include "QGSP_BERT.hh"
#include "FTFP_BERT.hh"

#include "DarkMatterPhysics.hh"
#include "DarkMatter.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "G4PhysListFactory.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4UIExecutive.hh"
#include "G4ios.hh"

#include "DarkMatterParametersFactory.hh"
#include "Randomize.hh"

#include <unistd.h>
#include <cstdlib>

void print_usage(){
  G4cerr << "Usage: " << G4endl;
  G4cerr << "   -m [macro (input) file name]" << G4endl;
  G4cerr << "   -o [output (date) file name]" << G4endl;
  G4cerr << "   -s [random number randSeed]"  << G4endl;
}

int main(int argc,char** argv){
  
  int optC = 0;
  
  G4long randSeed = time(NULL);
  G4UIExecutive* ui = 0;
  G4String macroFilename, dataFilename;
  G4String* dataFilename_ptr = &dataFilename;
  while((optC = getopt(argc, argv, "m:o:s:"))!= -1) {
      switch(optC){
      case 'm':
        macroFilename = optarg;
        break;

      case 'o':
        dataFilename = optarg;
        break;

      case 's':
        randSeed = atol(optarg);
        break;

	  case ':':
	  case '?':
		print_usage();
	    exit(EXIT_FAILURE);
		break;
	  }
  }

  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  G4Random::setTheSeed(randSeed);
  //G4Random::showEngineStatus();
	
	// Run manager
#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
	G4int nThreads = G4Threading::G4GetNumberOfCores();
	runManager->SetNumberOfThreads(nThreads);
#else
	G4RunManager * runManager = new G4RunManager;
#endif
	
	// UserInitialization classes
	DetectorConstruction* detectorConstruction = new DetectorConstruction;
	runManager->SetUserInitialization(detectorConstruction);
	
	// ___ Here the "extension" part starts ___
	G4PhysListFactory factory;
	G4VModularPhysicsList * phys = factory.GetReferencePhysList("FTFP_BERT");
	// most of the standard physics lists are available by this interface
	
	// G4PhysicsListHelper * phLHelper = G4PhysicsListHelper::GetPhysicsListHelper();
	// phLHelper->DumpOrdingParameterTable();
	
	DarkMatterPhysics* darkMatterPhysics = new DarkMatterPhysics();
	// bool config = darkMatterPhysics->DarkMatterPhysicsConfigure();
	DarkMatterParametersFactory* DMpar = DarkMatterParametersFactory::GetInstance();
	G4cout << "BiasSigmaFactor0 = " << DMpar->GetRegisteredParam("BiasSigmaFactor0") << G4endl;
	G4cout << "EThresh = " << DMpar->GetRegisteredParam("EThresh")/CLHEP::MeV << G4endl;
	G4cout << "DMProcessType = " << DMpar->GetRegisteredParam("DMProcessType") << G4endl;
	G4cout << "DMMass = " << DMpar->GetRegisteredParam("DMMass")/CLHEP::MeV << G4endl;
	G4cout << "Epsilon = " << DMpar->GetRegisteredParam("Epsilon") << G4endl;
	phys->RegisterPhysics(darkMatterPhysics);
	// ^^^ Here the "extension" part ends ^^^
	
	runManager->SetUserInitialization(phys);  // init phys
  
  if(!dataFilename.empty())
  {
	runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer(), dataFilename_ptr));}
  else {
	runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer()));
  }

	
#ifdef G4VIS_USE
	// Visualization, if you choose to have it!
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	
	// User interactions
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	
	if(!ui)
	{ // batch mode
	  G4String command = "/control/execute ";
	  UImanager->ApplyCommand(command+macroFilename);
	  
	}
	else
	{
      ui = new G4UIExecutive(argc,argv);
	  UImanager->ApplyCommand("/control/execute vis.mac");
	  ui->SessionStart();
	  delete ui;
	}
	
#ifdef G4VIS_USE
	delete visManager;
#endif
	delete runManager;
	
	return 0;
  }

