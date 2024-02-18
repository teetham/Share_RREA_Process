#include "stepping.hh"
#include "G4Timer.hh"

#include "G4RunManager.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}


void MySteppingAction::UserSteppingAction(const G4Step *step)
{   
    // retrieve the position in z-axis at time 4E-7 second of runaway electron 
    // runaaway elctron mean electron that have energy higher than 0.549 MeV

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4Track* track = step->GetTrack();
    G4double PreTime = step->GetPreStepPoint()->GetGlobalTime();
    G4double PostTime = step->GetPostStepPoint()->GetGlobalTime();
    G4String particleType = track->GetDefinition()->GetParticleName();
    G4double PreKineticEnergy =step->GetPreStepPoint()->GetKineticEnergy();
    G4double PostKineticEnergy =step->GetPostStepPoint()->GetKineticEnergy();

    // G4double posX = step->GetPreStepPoint()->GetPosition().x() /1000.;
    // G4double posY = step->GetPreStepPoint()->GetPosition().y() /1000.;
    G4double posZ = step->GetPreStepPoint()->GetPosition().z() /1000.;
    
   if(PostTime/s > 4E-7)
   {
        if(particleType == "e-" && PreKineticEnergy/MeV >= 0.549)
        {
            //save kinetic energy and the position in z-axis to csv file
            G4cout << "KE:" << PreKineticEnergy << '\t' << posZ << G4endl;
            man->FillNtupleDColumn(0, PreKineticEnergy);
            man->FillNtupleDColumn(1, posZ/1000.);
            man->AddNtupleRow(0);
        }
        track->SetTrackStatus(fStopAndKill);
   }
}