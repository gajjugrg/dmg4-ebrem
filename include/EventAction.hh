#pragma once

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "DarkMatter.hh"

#include "G4UserEventAction.hh"

class DetectorConstruction;
class SteppingActionDMG4;

class DarkMatter;

class G4Event;


class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction, DetectorConstruction* detectorConstruction, DarkMatter* darkMatter);
    ~EventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    void SetSteppingAction(SteppingActionDMG4* steppingAction) {fSteppingActionDMG4 = steppingAction;}
    DarkMatter* GetDarkMatterPointer() {return fDarkMatter;}
    void CountEmission() {NEmissions++;}

  private:
    RunAction* fRunAction;
    DetectorConstruction* fDetectorConstruction;
    SteppingActionDMG4* fSteppingActionDMG4;
    DarkMatter* fDarkMatter;

    G4int NEmissions;
};
