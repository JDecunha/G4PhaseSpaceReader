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
    //Variables to store data we're pulling from the phase space
    double x,y,z;
    double wx,wy,wz;
    double energy;
    int ptclType;

    //Get the next entry in the phase space file
    PhaseSpaceAccessTool psAccess;
    std::string _phaseSpaceCurrentLine = psAccess.GetNextPhaseSpaceEntry();

    //Go through each entry in the line
    int currentItem = 0;
    std::stringstream stringStream(_phaseSpaceCurrentLine);
    std::string word {};
    while(stringStream >> word)
    {
        if (currentItem == 1) {ptclType = std::stoi(word);}
        if (currentItem == 3) {energy = std::stod(word);}
        if (currentItem == 5) {x = std::stod(word);}
        if (currentItem == 6) {y = std::stod(word);}
        if (currentItem == 7) {z = std::stod(word);}
        if (currentItem == 8) {wy = std::stod(word);}
        if (currentItem == 9) {wx = std::stod(word);}
        if (currentItem == 10) {wz = std::stod(word);}

        ++currentItem;
    }

    //Take all the information we've extracted and actually send it to G4 Particle Gun
    gun->SetParticlePosition(G4ThreeVector(x*cm, y*cm, z*cm)); //Set the primary particle origin
    gun->SetParticleMomentumDirection(G4ThreeVector(wx,wy,wz)); //Set appropriate direction (just the direction cosines themselves)
    gun->SetParticleEnergy(energy*MeV);

    //Set the particle type
    auto particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = nullptr;
    //Getting MCNP Particle definitions from: https://mcnp.lanl.gov/pdf_files/TechReport_2017_LANL_LA-UR-17-29981_WernerArmstrongEtAl.pdf (PDF pg. 46)
    //1,2,3,9 are neutron, photon, electron, and proton respectively
    if (ptclType == 1) { particle = particleTable->FindParticle("neutron"); } 
    else if (ptclType == 2) { particle = particleTable->FindParticle("gamma"); }
    else if (ptclType == 3) { particle = particleTable->FindParticle("electron"); }
    else if (ptclType == 9) { particle = particleTable->FindParticle("proton"); }
    else if (ptclType == 31) { particle = particleTable->FindParticle("deuteron"); }
    else if (ptclType == 34) { particle = particleTable->FindParticle("alpha"); }
    else if (ptclType == 35) { particle = particleTable->FindParticle("pi-"); }
    else { std::cout << "Particle type: : " << ptclType << std::endl; throw std::runtime_error("Phase space reader currently only reads electrons, neutrons, protons, and photons. ?! What is THIS particle !! ");}

    gun->SetParticleDefinition(particle);

    //Shoot!
    if (particle)
    {
        // std::cout << "Particle: " << gun->GetParticleDefinition()->GetParticleName() << " Energy: " << gun->GetParticleEnergy() << std::endl;
        // std::cout << "Position: " << gun->GetParticlePosition() << std::endl;
        // std::cout << "Momentum: " << gun->GetParticleMomentumDirection() << std::endl;
        gun->GeneratePrimaryVertex(event);
    }
    else
    {
        std::cout << "Skipping unknown particle type: " << ptclType << std::endl;
    }
}
