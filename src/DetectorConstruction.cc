#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"

#include "G4ios.hh"


DetectorConstruction::DetectorConstruction()
{
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 500*cm, env_sizeZ = 500*cm;
  G4Material* env_mat = new G4Material("Graphite", 6., 12.0107*(g/mole), 2.5*g/cm3);
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_C");
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_GRAPHITE");
  G4cout << env_mat->GetA()/(g/mole) << G4endl;
  G4cout << env_mat->GetZ() << G4endl;
  G4cout << env_mat->GetDensity()/(g/cm3) << G4endl;

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     
  logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");            

  G4VPhysicalVolume* physWorld =
  new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);     
  //
  // Envelope
  //
  G4Box* solidEnv = new G4Box("Envelope", 0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ);
  G4LogicalVolume* logicEnv =
  new G4LogicalVolume(solidEnv, env_mat, "Envelope");  
  new G4PVPlacement( 0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);        


  return physWorld;
}
