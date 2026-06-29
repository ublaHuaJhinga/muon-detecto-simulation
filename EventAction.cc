#include "EventAction.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

EventAction::EventAction() : fEdep(0.), fTheta(0.) {
    fOutFile.open("muon_data.csv");
    fOutFile << "theta_deg,edep_MeV,detected\n";
}

EventAction::~EventAction() {
    if (fOutFile.is_open()) fOutFile.close();
}

void EventAction::BeginOfEventAction(const G4Event*) {
    fEdep  = 0.;
    fTheta = 0.;
}

void EventAction::EndOfEventAction(const G4Event*) {
    // Detection threshold: muon must deposit at least 0.5 MeV
    G4double threshold = 0.5 * MeV;
    G4int detected = (fEdep >= threshold) ? 1 : 0;

    G4double thetaDeg = fTheta * (180.0 / pi);
    fOutFile << thetaDeg << "," << fEdep/MeV << "," << detected << "\n";
}
