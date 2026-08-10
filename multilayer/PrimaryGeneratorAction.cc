#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction* eventAction)
    : fEventAction(eventAction) {
    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* muon = particleTable->FindParticle("mu-");
    fParticleGun->SetParticleDefinition(muon);

    fParticleGun->SetParticleEnergy(4.0 * GeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {

    G4double theta, cosTheta;
    do {
        theta    = G4UniformRand() * (pi / 2.0);
        cosTheta = std::cos(theta);
    } while (G4UniformRand() > cosTheta * cosTheta);

    fEventAction->SetTheta(theta);

    G4double phi = G4UniformRand() * 2.0 * pi;

    G4double sinTheta = std::sin(theta);
    G4ThreeVector direction(
        sinTheta * std::cos(phi),
        sinTheta * std::sin(phi),
       -cosTheta
    );
    fParticleGun->SetParticleMomentumDirection(direction);

    G4double x0 = (G4UniformRand() - 0.5) * 40.0 * cm;
    G4double y0 = (G4UniformRand() - 0.5) * 40.0 * cm;
    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, 80.0*cm));

    fParticleGun->GeneratePrimaryVertex(event);
}
