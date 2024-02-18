#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    //create output file called output.csv
    //normally we run multithreading with 8 threading
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->OpenFile("output.csv");
    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleDColumn("KineticEnergy");
    man->CreateNtupleDColumn("PositionZ");
    man->FinishNtuple(0);
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();

    fTimer.Stop();
    PrintTime();
}

void MyRunAction::PrintTime()
{
    G4double time = fTimer.GetRealElapsed();
    G4cout << "Elapsed time: " << time << G4endl;
}