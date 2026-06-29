#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <fstream>

class EventAction : public G4UserEventAction {
public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    // Called by SteppingAction when a muon deposits energy in scintillator
    void AddEnergyDeposit(G4double edep) { fEdep += edep; }
    void SetTheta(G4double theta)        { fTheta = theta; }

private:
    G4double fEdep;
    G4double fTheta;
    std::ofstream fOutFile;
};

#endif
