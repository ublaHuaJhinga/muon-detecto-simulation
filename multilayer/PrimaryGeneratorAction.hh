#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class EventAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction(EventAction* eventAction);
    ~PrimaryGeneratorAction() override;
    void GeneratePrimaries(G4Event*) override;

private:
    G4ParticleGun* fParticleGun;
    EventAction* fEventAction;
};

#endif
