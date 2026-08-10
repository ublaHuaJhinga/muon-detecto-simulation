#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DetectorConstruction::DetectorConstruction()
    : fScintThickness(2.0*cm),
      fScintWidth(50.0*cm),
      fScintHeight(50.0*cm),
      fLayerSpacing(15.0*cm)
{}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    G4NistManager* nist = G4NistManager::Instance();

    G4double worldSize = 2.0 * m;
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld = new G4Box("World",
        worldSize/2, worldSize/2, worldSize/2);

    G4LogicalVolume* logicWorld = new G4LogicalVolume(
        solidWorld, worldMat, "World");

    G4VPhysicalVolume* physWorld = new G4PVPlacement(
        nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    G4Material* scintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Box* solidScint = new G4Box("Scintillator",
        fScintWidth/2, fScintHeight/2, fScintThickness/2);

    G4VisAttributes* scintVis = new G4VisAttributes(G4Colour(0.2, 0.6, 1.0, 0.5));
    scintVis->SetForceSolid(true);

    for (G4int i = 0; i < kNLayers; i++) {
        G4LogicalVolume* logicScint = new G4LogicalVolume(
            solidScint, scintMat, "Scintillator");
        logicScint->SetVisAttributes(scintVis);

        G4double zPos = ((kNLayers - 1) / 2.0 - i) * fLayerSpacing;

        new G4PVPlacement(
            nullptr, G4ThreeVector(0, 0, zPos),
            logicScint, "Scintillator", logicWorld, false, i, true);
    }

    return physWorld;
}
