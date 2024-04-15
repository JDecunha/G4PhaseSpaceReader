//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhaseSpaceAccessTool.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    //Create the gun and messenger
    gun = new G4ParticleGun();

    //Set some default values (I think having defaults may help with troubleshooting)
    gun->SetParticlePosition(G4ThreeVector(0,0,48*cm)); //Set the primary particle origin position to 0, 0, 48 cm (location of first phase space Uwe sent). 
    gun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1)); //Set appropriate direction (down in Z axis)
    gun->SetParticleEnergy(1*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    //Take all the information we've extracted and actually send it to G4 Particle Gun
    auto particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = nullptr;
    gun->SetParticlePosition(G4ThreeVector(0*cm, 0*cm, 48*cm)); //Set the primary particle origin
    gun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1)); //Set appropriate direction (just the direction cosines themselves)
    gun->SetParticleEnergy(80*MeV);
    particle = particleTable->FindParticle("proton"); 
    gun->SetParticleDefinition(particle);

    //Shoot!
    if (particle)
    {
        // std::cout << "Particle: " << gun->GetParticleDefinition()->GetParticleName() << " Energy: " << gun->GetParticleEnergy() << std::endl;
        gun->GeneratePrimaryVertex(event);
    }

}
