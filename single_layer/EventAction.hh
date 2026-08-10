#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <fstream>

class EventAction : public G4UserEventAction {
public:
    EventAction(G4double thresholdMeV = 0.5, const G4String& outFileName = "muon_data.csv");
    ~EventAction() override;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    void AddEnergyDeposit(G4double edep) { fEdep += edep; }
    void SetTheta(G4double theta)        { fTheta = theta; }

    void MarkHit()                       { fHit = true; }

private:
    G4double fEdep;
    G4double fTheta;
    G4double fThreshold;
    G4bool   fHit;
    std::ofstream fOutFile;
};

#endif
