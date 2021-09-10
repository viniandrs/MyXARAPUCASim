#include "../include/PrimaryGeneratorAction.h"
#include "G4Geantino.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"

#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;
using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction() {}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete m_newGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    //------------------- Random initial position of the particles -------------------
    
    //ENTENDER ESSES TREM
    G4double H_b = 43.5 * cm;
    G4double esp = 20.0 * mm;
    G4double esp_alpha = 1 * cm;

    G4double dif_h = 10.0 * cm;
    G4double espessura_maior = 8.5 * cm;

    //Choosing at random a cyllindrical coordinate
    const G4double pi = acos(-1.);
    G4double teta = 2 * pi * G4UniformRand();
    G4double myraio = 0.5 * cm * sqrt(G4UniformRand());

    G4double costeta = cos(teta);
    G4double sinteta = sin(teta);

    G4double x0 = myraio * costeta;
    G4double z0 = myraio * sinteta + 0.5 * (-0.5 * H_b + 0.5 * espessura_maior + dif_h - esp);
    G4double y0 = 3.0 * cm - 140 * (G4UniformRand() - 0.5) * um;

    //the initial position of the particle
    m_newGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    // default particle kinematic
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    m_newGun->SetParticleDefinition(particleTable->FindParticle("alpha"));

    //------------------- Random initial momentum direction -------------------
    G4double phi2 = 2 * pi * G4UniformRand();
    G4double teta2 = pi * G4UniformRand();

    G4double costeta2 = cos(teta2);
    G4double sinteta2 = sin(teta2);
    G4double cosphi2 = cos(phi2);
    G4double sinphi2 = sin(phi2);

    //Momentum vector with a random spherical direction
    G4ThreeVector dir = G4ThreeVector(sinteta2 * cosphi2, costeta2, sinteta2 * sinphi2);

    m_newGun->SetParticleMomentumDirection(dir);

    //------------------- Random initial particle energy -------------------

    /*
    For this, we'll recall that alpha particles emitted by uranium have a distribution of 2.2%, 48.9% and 48.9%,
    for the respective energies  4.464, 4.187 and 4.759 MeV.
    */
    G4double random = 100 * G4UniformRand();

    if (random <= 2.2)
    {
        m_newGun->SetParticleEnergy(4.464 * MeV);
    }
    else if (random > 2.2 && random < 51.1)
    {
        m_newGun->SetParticleEnergy(4.187 * MeV);
    }
    else
    {
        m_newGun->SetParticleEnergy(4.759 * MeV);
    }

    //Generating the event
    m_newGun->GeneratePrimaryVertex(anEvent);

    flag_alpha = true;
    //flag_generate = true;
}