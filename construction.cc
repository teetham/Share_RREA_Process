#include "construction.hh"
#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"
#include "G4ThreeVector.hh"
#include "G4ElectricField.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4DormandPrince745.hh"
#include "G4AutoDelete.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>

#include "G4UniformElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4StepLimiterPhysics.hh"


#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
    nCols = 10;
    nRows = 10;

    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

    fMessenger->DeclareProperty("nCols", nCols, "Number of cols");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");

    fMessenger->DeclareProperty("atmosphere", isAtmosphere, "Construct Atmosphere");

    DefineMaterials();

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    //Create the construction material and add to worldMat

    elN = nist->FindOrBuildElement("N");
    elO = nist->FindOrBuildElement("O");
    elAr = nist->FindOrBuildElement("Ar");

    // G4double density = 0.0006542*g/cm3; //calculate from  paper
    G4double density = 0.001293*g/cm3; //density at STP

    G4double temperature = 273.15*kelvin;
    G4double pressure = 1*atmosphere;

    worldMat = new G4Material("Air", density, 3, kStateGas, temperature, pressure);
    worldMat->AddElement(elN, 78.08*perCent);
    worldMat->AddElement(elO, 20.95*perCent);
    worldMat->AddElement(elAr, 0.97*perCent);

	G4cout << worldMat;

    
}

void MyDetectorConstruction::ConstructAtmosphere()
{    
    //Create the construction according to paper
    G4bool checkOverlaps = 1;

    height = 10.*km; 
    diameter = 3000.*km;
    heightElec = 319.375*m;
    midPositionElec = -4640.3125*m;

    //This tube is world volume
    solidWorld = new G4Tubs("worldTub",0.,diameter/2.,height/2,0.,360.*deg);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "worldLogical");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    //This tube is created for apply the electric field
    solidElec = new G4Tubs("solidElec",0.,diameter/2.,heightElec/2,0.,360.*deg);
    logicElec = new G4LogicalVolume(solidElec, worldMat, "logicElec");
    physElec = new G4PVPlacement(0, G4ThreeVector(0., 0., midPositionElec), logicElec, "physElec", logicWorld, false, 0, checkOverlaps);
}

void MyDetectorConstruction::ConstructElectricField()
{
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    ConstructAtmosphere();
    ConstructElectricField();

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    //Add electric field in downward direction to logicElec
    fEMfield_2 = new G4UniformElectricField(G4ThreeVector(0.0, 0.0, -436.0*kilovolt/m));
    fEquation_2 = new G4EqMagElectricField(fEMfield_2); 
    G4int nvar2 = 8;
    fStepper_2 = new G4ClassicalRK4(fEquation_2,nvar2); 
    auto localFieldMgr_2 = new G4FieldManager(fEMfield_2);
    logicElec->SetFieldManager(localFieldMgr_2,true);   
    fMinStep_2 = 0.00010*mm ;
    auto fIntgrDriver_2 = new G4MagInt_Driver(fMinStep_2,fStepper_2,fStepper_2->GetNumberOfVariables());    
    fChordFinder_2 = new G4ChordFinder(fIntgrDriver_2);
    localFieldMgr_2->SetChordFinder(fChordFinder_2);
}
