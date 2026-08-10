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

    G4LogicalVolume* volume =
        step->GetPreStepPoint()->GetTouchableHandle()
            ->GetVolume()->GetLogicalVolume();

    if (volume->GetName() != "Scintillator") return;

    fEventAction->MarkHit();

    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEnergyDeposit(edep);
}
