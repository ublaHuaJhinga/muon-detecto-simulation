#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"
#include <fstream>

class EventAction : public G4UserEventAction {
public:
    EventAction(G4double thresholdMeV = 0.5, const G4String& outFileName = "muon_data.csv");
    ~EventAction() override;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    void AddEnergyDeposit(G4int layer, G4double edep) { fEdep[layer] += edep; }
    void SetTheta(G4double theta)                     { fTheta = theta; }
    void MarkHit(G4int layer)                         { fHit[layer] = true; }

private:
    static const int kNLayers = DetectorConstruction::kNLayers;

    G4double fEdep[kNLayers];
    G4bool   fHit[kNLayers];
    G4double fTheta;
    G4double fThreshold;
    std::ofstream fOutFile;
};

#endif
