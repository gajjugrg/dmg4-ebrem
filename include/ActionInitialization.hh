#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "DetectorConstruction.hh"
#include "DarkMatter.hh"

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction* detectorConstruction, DarkMatter* darkMatter);
    ActionInitialization(DetectorConstruction* detectorConstruction, DarkMatter* darkMatter, const G4String* outputFileNamePtr);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    DetectorConstruction* fDetectorConstruction;
    DarkMatter* fDarkMatter;
    const G4String* fOutputFileNamePtr;
};

#endif
