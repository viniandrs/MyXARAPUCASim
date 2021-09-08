#include "../include/MyGenerator.h"
#include "G4Geantino.hh"

MyPrimaryGenerator::MyPrimaryGenerator() : G4VUserPrimaryGeneratorAction(), m_newGun(new G4ParticleGun(G4Geantino::GeantinoDefinition())) {}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete m_newGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    int nPhotons = 20;
    for(int i = 0; i < nPhotons; i++) m_newGun->GeneratePrimaryVertex(anEvent);
}