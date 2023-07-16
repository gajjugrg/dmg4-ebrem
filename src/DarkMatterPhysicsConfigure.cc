#include "DarkMatterPhysics.hh"

#include "DarkMatter.hh"
#include "DarkPhotons.hh"
#include "DarkScalars.hh"
#include "DarkPseudoScalars.hh"
#include "DarkAxials.hh"
#include "ALP.hh"
#include "DarkZ.hh"
#include "DarkPhotonsAnnihilation.hh"
#include "DarkScalarsAnnihilation.hh"
#include "DarkPseudoScalarsAnnihilation.hh"
#include "DarkAxialsAnnihilation.hh"


#include "DarkMatterParametersFactory.hh"

#include "G4SystemOfUnits.hh"

bool DarkMatterPhysics::DarkMatterPhysicsConfigure() 
{
  //call an instance of the class
  DarkMatterParametersFactory* DMpar = DarkMatterParametersFactory::GetInstance();  
  
  DMpar->RegisterNewParam("BiasSigmaFactor0", 1);
  DMpar->RegisterNewParam("EThresh", 18.); // for sensitivity calculations invisible mode
  //G4double EThresh = 18.; // for sensitivity calculations visible mode
  //G4double EThresh = 1.; // for shape studies
  //G4double EThresh = 2000.; // to turn off A emissions  

  //select particle type and details
  DMpar->RegisterNewParam("DMProcessType", 4.);
  DMpar->RegisterNewParam("DMMass", 0.0100);
  DMpar->RegisterNewParam("Epsilon", 1e-15); // keeping this as 0.001 for now (default)

  // Initialize for Pb
//  DMpar->RegisterNewParam("ANucl"      ,207.   );
//  DMpar->RegisterNewParam("ZNucl"      ,82.    );
//  DMpar->RegisterNewParam("Density"    ,12.35  );

  // Initialize for Carbon  
  DMpar->RegisterNewParam("ANucl"      ,12.0107*(g/mole));
  DMpar->RegisterNewParam("ZNucl"      ,6.    );
  DMpar->RegisterNewParam("Density"    ,2.5*(g/cm3));

//  Initialize for W
//  DMpar->RegisterNewParam("ANucl"   ,184.   );
//  DMpar->RegisterNewParam("ZNucl"   ,74.    );
//  DMpar->RegisterNewParam("Density" ,19.25  );

  DMpar->RegisterNewParam("DecayType", 2.); // 0 invisible, 2 visible

  // additional parameters for annihilation
  DMpar->RegisterNewParam("RDM", 1./3.);
  DMpar->RegisterNewParam("AlphaD", 0.5);
  
  return true;
}
