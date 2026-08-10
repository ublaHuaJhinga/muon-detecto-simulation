#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    G4VPhysicalVolume* Construct() override;

    static const int kNLayers = 3;

private:
    G4double fScintThickness;
    G4double fScintWidth;
    G4double fScintHeight;
    G4double fLayerSpacing;
};

#endif
