#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "QGSP_BERT.hh"

int main() {
    auto* runManager = new G4RunManager();

    // Detector
    runManager->SetUserInitialization(new DetectorConstruction());

    // Physics list
    runManager->SetUserInitialization(new QGSP_BERT);

    // User actions
    auto* eventAction = new EventAction();
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(eventAction);
    runManager->SetUserAction(new SteppingAction(eventAction));

    runManager->Initialize();

    // Run
    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/run/initialize");
    UI->ApplyCommand("/run/beamOn 10000");

    delete runManager;
    return 0;
}
