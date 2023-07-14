#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingActionDMG4.hh"


ActionInitialization::ActionInitialization(DetectorConstruction* detectorConstruction, DarkMatter* darkMatter)
 : fDetectorConstruction(detectorConstruction),
   fDarkMatter(darkMatter), fOutputFileNamePtr(nullptr)
{}

ActionInitialization::ActionInitialization(DetectorConstruction* detectorConstruction, DarkMatter* darkMatter, const G4String* outputFileNamePtr)
 : fDetectorConstruction(detectorConstruction),
   fDarkMatter(darkMatter),fOutputFileNamePtr(outputFileNamePtr)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction;
  if( fOutputFileNamePtr != nullptr)
	  runAction = new RunAction(fOutputFileNamePtr);
  else
	  runAction = new RunAction();
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);

  RunAction* runAction;
 if( fOutputFileNamePtr != nullptr)
	runAction = new RunAction(fOutputFileNamePtr);
 else
	runAction = new RunAction;
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction(runAction, fDetectorConstruction, fDarkMatter);
  SetUserAction(eventAction);

  SetUserAction(new SteppingActionDMG4(eventAction));
  
}

