#include "EventAction.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

EventAction::EventAction(G4double thresholdMeV, const G4String& outFileName)
    : fTheta(0.), fThreshold(thresholdMeV * MeV) {
    for (int i = 0; i < kNLayers; i++) { fEdep[i] = 0.; fHit[i] = false; }

    fOutFile.open(outFileName);
    fOutFile << "theta_deg";
    for (int i = 0; i < kNLayers; i++) fOutFile << ",edep" << i << "_MeV";
    for (int i = 0; i < kNLayers; i++) fOutFile << ",det" << i;
    fOutFile << "\n";
}

EventAction::~EventAction() {
    if (fOutFile.is_open()) fOutFile.close();
}

void EventAction::BeginOfEventAction(const G4Event*) {

    for (int i = 0; i < kNLayers; i++) { fEdep[i] = 0.; fHit[i] = false; }
}

void EventAction::EndOfEventAction(const G4Event*) {
    G4double thetaDeg = fTheta * (180.0 / pi);

    fOutFile << thetaDeg;
    for (int i = 0; i < kNLayers; i++) fOutFile << "," << fEdep[i]/MeV;
    for (int i = 0; i < kNLayers; i++) {
        G4int det = (fEdep[i] >= fThreshold) ? 1 : 0;
        fOutFile << "," << det;
    }
    fOutFile << "\n";
}
