#include "EventAction.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

EventAction::EventAction(G4double thresholdMeV, const G4String& outFileName)
    : fEdep(0.), fTheta(0.), fThreshold(thresholdMeV * MeV), fHit(false) {
    fOutFile.open(outFileName);
    fOutFile << "theta_deg,edep_MeV,detected,hit\n";
}

EventAction::~EventAction() {
    if (fOutFile.is_open()) fOutFile.close();
}

void EventAction::BeginOfEventAction(const G4Event*) {

    fEdep = 0.;
    fHit  = false;
}

void EventAction::EndOfEventAction(const G4Event*) {
    G4int detected = (fEdep >= fThreshold) ? 1 : 0;
    G4int hit      = fHit ? 1 : 0;

    G4double thetaDeg = fTheta * (180.0 / pi);
    fOutFile << thetaDeg << "," << fEdep/MeV << "," << detected << "," << hit << "\n";
}
