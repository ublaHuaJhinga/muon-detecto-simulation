#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
    : fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {

    // Only record steps inside the scintillator
    G4LogicalVolume* volume =
        step->GetPreStepPoint()->GetTouchableHandle()
            ->GetVolume()->GetLogicalVolume();

    if (volume->GetName() != "Scintillator") return;

    // Accumulate energy deposit
    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEnergyDeposit(edep);

    // Record theta from the primary muon's momentum direction
    // (use first step in scintillator only)
    G4Track* track = step->GetTrack();
    if (track->GetTrackID() == 1 &&
        track->GetCurrentStepNumber() == 1) {

        G4ThreeVector dir = track->GetMomentumDirection();
        // theta is angle from vertical (z-axis)
        G4double cosTheta = std::abs(dir.z());
        G4double theta    = std::acos(cosTheta);
        fEventAction->SetTheta(theta);
    }
}
