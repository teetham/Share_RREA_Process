#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"

#include "G4ElectricField.hh"
#include "G4EqMagElectricField.hh"

#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"
#include <G4TransportationManager.hh>
#include "G4Cache.hh"

#include "detector.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

class G4FieldManager;
class G4ChordFinder;
class G4EquationOfMotion;
class G4Mag_EqRhs;
class G4EqMagElectricField;
class G4MagIntegratorStepper;
class G4MagInt_Driver;

class G4VPhysicalVolume;
class G4LogicalVolume;



class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }
    
    virtual G4VPhysicalVolume *Construct();
    void ConstructAtmosphere();
    void ConstructElectricField();


private:
    G4Tubs *solidWorld,*solidElec, *solidDetector;
    G4LogicalVolume *logicWorld, *logicElec, *logicDetector;
    G4VPhysicalVolume *physWorld, *physElec, *phyDetector;

    G4Element *elN, *elO, *elAr;
    G4Material *matAir, *worldMat;
    void DefineMaterials();
    virtual void ConstructSDandField();

    G4GenericMessenger *fMessenger;
    G4LogicalVolume *fScoringVolume;
    G4int nRows, nCols, nLayer;
    G4double height, diameter, heightLayer, heightElec, midPositionElec;  

    bool isAtmosphere=true;

    G4ElectricField*        fEMfield_2;
    G4EqMagElectricField*   fEquation_2;
    G4MagIntegratorStepper* fStepper_2;
    G4FieldManager*         fFieldMgr_2;
    G4double                fMinStep_2;
    G4ChordFinder*          fChordFinder_2;
};

#endif