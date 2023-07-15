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

int main(int argc,char** argv) {

 G4long seed = 132334;
 G4UIExecutive* ui = 0;
 G4String outputFileName;
 G4String* foutputfFileName = &outputFileName; 
 
 switch(argc)
 { 
	 case 1:
		 G4cout << "Interactive mode" << G4endl;
		 ui = new G4UIExecutive(argc, argv);
		 break;
	 case 2:
		 G4cout << "Single job mode : set random seed to be " << seed << "." << G4endl;
		 G4Random::setTheSeed(seed);
		 break;
	 case 3:
		 G4cout << "Batch job mode" << G4endl;
		 outputFileName = argv[2];
		 //G4Random::setTheSeed(seed);
		 G4Random::setTheEngine(new CLHEP::MTwistEngine);
		 break;
	 default:
		 G4cout << "Interactive Mode" << G4endl;
		 ui = new G4UIExecutive(argc, argv);
		 break;
 }


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
  // ^^^ most of the standard physics lists are available by this interface

 // G4PhysicsListHelper * phLHelper = G4PhysicsListHelper::GetPhysicsListHelper();
 // phLHelper->DumpOrdingParameterTable();

  DarkMatterPhysics* darkMatterPhysics = new DarkMatterPhysics();
  //bool config = darkMatterPhysics->DarkMatterPhysicsConfigure();
  DarkMatterParametersFactory* DMpar = DarkMatterParametersFactory::GetInstance();
  G4cout << "BiasSigmaFactor0 = " << DMpar->GetRegisteredParam("BiasSigmaFactor0") << G4endl;
  G4cout << "EThresh = " << DMpar->GetRegisteredParam("EThresh")/CLHEP::MeV << G4endl;
  G4cout << "DMProcessType = " << DMpar->GetRegisteredParam("DMProcessType") << G4endl;
  G4cout << "DMMass = " << DMpar->GetRegisteredParam("DMMass")/CLHEP::MeV << G4endl;
  G4cout << "Epsilon = " << DMpar->GetRegisteredParam("Epsilon") << G4endl;
  phys->RegisterPhysics(darkMatterPhysics);
  // ^^^ Here the "extension" part ends ^^^

  runManager->SetUserInitialization(phys);  // init phys

  // User action initialization
  switch(argc)
  { 
	case 1:
		runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer()));
		break;
 	case 2:
		runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer()));
		break;
 	case 3:
		runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer(), foutputfFileName));
		break;
        default:
		runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer()));
		break;
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
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
	    
  }
  else
  // Batch mode
  { 
    // G4UIterminal is a (dumb) terminal.
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

