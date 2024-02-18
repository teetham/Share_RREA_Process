#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4ElectroMagneticField.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "construction.hh"
#include "event.hh"
#include "G4Timer.hh"
#include "G4Track.hh"

class MySteppingAction : public G4UserSteppingAction
{
public:
    MySteppingAction(MyEventAction* eventAction);
    ~MySteppingAction();
    virtual void UserSteppingAction(const G4Step*);
    
private:
    MyEventAction *fEventAction;
    G4Timer* ftimer2 = new G4Timer();
};

#endif
