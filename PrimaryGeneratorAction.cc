#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);

    // Use muon-minus as primary particle
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* muon = particleTable->FindParticle("mu-");
    fParticleGun->SetParticleDefinition(muon);

    // Typical sea-level cosmic muon energy ~ 4 GeV
    fParticleGun->SetParticleEnergy(4.0 * GeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    // Cosmic muon angular distribution: I(theta) ~ cos^2(theta)
    // Sample theta using rejection sampling
    G4double theta, cosTheta;
    do {
        theta    = G4UniformRand() * (pi / 2.0); // 0 to 90 degrees
        cosTheta = std::cos(theta);
    } while (G4UniformRand() > cosTheta * cosTheta);

    // Random azimuthal angle phi
    G4double phi = G4UniformRand() * 2.0 * pi;

    // Direction vector: muons travel downward (negative z)
    G4double sinTheta = std::sin(theta);
    G4ThreeVector direction(
        sinTheta * std::cos(phi),
        sinTheta * std::sin(phi),
       -cosTheta   // downward
    );
    fParticleGun->SetParticleMomentumDirection(direction);

    // Spawn muons from a plane 80 cm above the detector
    G4double x0 = (G4UniformRand() - 0.5) * 40.0 * cm;
    G4double y0 = (G4UniformRand() - 0.5) * 40.0 * cm;
    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, 80.0*cm));

    fParticleGun->GeneratePrimaryVertex(event);
}
