#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "QGSP_BERT.hh"

#include <cstdlib>
#include <string>

int main(int argc, char** argv) {

    G4double threshold = 0.5;
    if (argc > 1) threshold = std::atof(argv[1]);

    G4String outFileName = "muon_data_thr" + std::to_string(threshold) + ".csv";

    auto* runManager = new G4RunManager();

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BERT);

    auto* eventAction = new EventAction(threshold, outFileName);
    runManager->SetUserAction(new PrimaryGeneratorAction(eventAction));
    runManager->SetUserAction(eventAction);
    runManager->SetUserAction(new SteppingAction(eventAction));

    runManager->Initialize();

    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/run/initialize");
    UI->ApplyCommand("/run/beamOn 10000");

    delete runManager;
    return 0;
}
