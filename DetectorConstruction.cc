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
      fScintHeight(50.0*cm)
{}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    G4NistManager* nist = G4NistManager::Instance();

    // World volume
    G4double worldSize = 2.0 * m;
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld = new G4Box("World",
        worldSize/2, worldSize/2, worldSize/2);

    G4LogicalVolume* logicWorld = new G4LogicalVolume(
        solidWorld, worldMat, "World");

    G4VPhysicalVolume* physWorld = new G4PVPlacement(
        nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    // Plastic scintillator slab
    // G4_PLASTIC_SC_VINYLTOLUENE is Geant4's built-in plastic scintillator
    G4Material* scintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Box* solidScint = new G4Box("Scintillator",
        fScintWidth/2, fScintHeight/2, fScintThickness/2);

    G4LogicalVolume* logicScint = new G4LogicalVolume(
        solidScint, scintMat, "Scintillator");

    // Place scintillator at origin, perpendicular to z-axis
    new G4PVPlacement(
        nullptr, G4ThreeVector(0, 0, 0),
        logicScint, "Scintillator", logicWorld, false, 0, true);

    // Visual attributes
    G4VisAttributes* scintVis = new G4VisAttributes(G4Colour(0.2, 0.6, 1.0, 0.5));
    scintVis->SetForceSolid(true);
    logicScint->SetVisAttributes(scintVis);

    return physWorld;
}
