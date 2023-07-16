#include "globals.hh"
#include "G4ios.hh"

#include "SteppingActionDMG4.hh"
//#include "RunAction.hh"
//#include "RunActionDMG4.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "DarkMatter.hh"
#include "DarkPhotons.hh"
#include "DarkScalars.hh"
#include "ALP.hh"

#include "DMParticles/DMParticle.hh"

#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessType.hh"
#include "Randomize.hh"

#include "G4ParticleTypes.hh"
#include "G4DynamicParticle.hh"
#include "G4EventManager.hh"
#include "G4TrackVector.hh"
#include "G4SystemOfUnits.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"

#include "G4ParticleTypes.hh"
#include "G4DynamicParticle.hh"
#include "G4EventManager.hh"
#include "G4TrackVector.hh"

#include "g4root.hh"

SteppingActionDMG4::SteppingActionDMG4(EventAction* myEA)
 : eventAction(myEA)
{
  eventAction->SetSteppingAction(this);
}

//SteppingActionDMG4::SteppingActionDMG4(DetectorConstruction* myDetector, EventAction* myEA)
//: eventAction(myEA)
//{
//  eventAction->SetSteppingAction(this);
//}


void SteppingActionDMG4::UserSteppingAction(const G4Step* aStep)
{
  G4StepPoint* SPointPreStep = aStep->GetPreStepPoint();
  //G4StepPoint* SPointPostStep = aStep->GetPostStepPoint();
  //G4double ekin = aStep->GetTrack()->GetKineticEnergy()/GeV;
  //G4double ekinprestep = SPointPreStep->GetKineticEnergy()/GeV;
  //G4ParticleDefinition* theParticleDefinition = aStep->GetTrack()->GetDefinition();

  if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != 0) {
    if((aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()).find(string("DMProcess")) != string::npos) {

      eventAction->CountEmission();

    //  G4cout << "Dark Matter production at E (GeV) = " << SPointPreStep->GetKineticEnergy()/GeV << G4endl;
    //  G4cout <<  "DM Particle " << aStep->GetTrack()->GetDefinition()->GetParticleName() << G4endl;
    //  G4cout << "Dark Matter Energy (GeV) = " << SPointPreStep->GetKineticEnergy()/GeV - aStep->GetPostStepPoint()->GetKineticEnergy()/GeV << G4endl;
    }
  }


	//storing the tracklength and energy information.
	G4Track* track = aStep->GetTrack();
	
	// identify primary track
	if (track->GetTrackID()== 1){
		
		//  G4cout << "PID: " << track->GetDefinition()->GetParticleName() << G4endl;
		auto fKineticEnergy = track->GetKineticEnergy();
		auto trkLength = track ->GetTrackLength();
		auto ProcName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
		
		// Prepare analysis manager
		auto analysisManager = G4AnalysisManager::Instance();
		if(fKineticEnergy/CLHEP::MeV < 1){ track->SetTrackStatus(fStopAndKill);}
		else
		{  // save the data
			analysisManager->FillNtupleIColumn(0, track->GetTrackID());
			analysisManager->FillNtupleDColumn(1, fKineticEnergy/CLHEP::MeV);
			analysisManager->FillNtupleDColumn(2, (trkLength/CLHEP::cm)/ 18.85);
			analysisManager->FillNtupleSColumn(3, ProcName);
			analysisManager->AddNtupleRow();
		}
	}
	else if( track->GetTrackID() != 1 )
	{
		track->SetTrackStatus(fStopAndKill);
	}
}


void SteppingActionDMG4::Reset()
{
  eventAction->GetDarkMatterPointer()->ResetNEmissions();
}


void SteppingActionDMG4::Finalize()
{
}
