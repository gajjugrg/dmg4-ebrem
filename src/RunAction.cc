#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
	: G4UserRunAction(),
	  fOutputFileNamePtr(nullptr)
{
	// set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);
	
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	
	// Create directories
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	// Creating ntuple
	analysisManager->CreateNtuple("brem", "e- bremsstrahlung in C-12");
	analysisManager->CreateNtupleIColumn("TrackID");
	analysisManager->CreateNtupleDColumn("fKineticEnergy");
	analysisManager->CreateNtupleDColumn("fTrackLength_X0");
	analysisManager->CreateNtupleSColumn("fProcess");
	analysisManager->FinishNtuple();
}

RunAction::RunAction(const G4String* outputFileName)
	: G4UserRunAction(),
	  fOutputFileNamePtr(outputFileName)
{
	// set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);
	
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	G4cout << "Using " << analysisManager->GetType() << G4endl;

	// Create directories
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	// Creating ntuple
	analysisManager->CreateNtuple("brem", "e- bremsstrahlung in C-12");
	analysisManager->CreateNtupleIColumn("TrackID");
	analysisManager->CreateNtupleDColumn("fKineticEnergy");
	analysisManager->CreateNtupleDColumn("fTrackLength_X0");
	analysisManager->CreateNtupleSColumn("fProcess");
	analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// default mode
void RunAction::BeginOfRunAction(const G4Run*)
{
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	// set output file name
	if( fOutputFileNamePtr == nullptr)
	{
		// single file mode
	        G4String fileName = "bremOutput.root";
		G4cout << "Single File mode: Setting output file name: " << fileName << G4endl;
	        analysisManager->SetFileName(fileName);
	}
	else
	{ // batch mode
	  G4cout << "Batch mode: Setting output file name: " << *fOutputFileNamePtr << G4endl;
	  analysisManager->SetFileName(*fOutputFileNamePtr);
	}
	analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
	if(IsMaster()){
		G4cout << "----------------End of Global Run ---------------" << G4endl;
	}
	else{
		G4cout << "----------------End of Local Run ----------------" << G4endl;
	}

	auto analysisManager = G4AnalysisManager::Instance();
	//write and close the file at the end of each run
	analysisManager->Write();
	analysisManager->CloseFile();
}
