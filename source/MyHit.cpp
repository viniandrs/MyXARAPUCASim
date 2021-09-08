#include "../include/MyHit.h"
#include "G4UnitsTable.hh"

using namespace std;

/*
In order to make it easy for the computer to transfer data (without needing to store it in a variable),
we can use the member initializers. In the parenthesis, we'll put the starting value of each member.
*/

MyHit :: MyHit(G4Step *aStep) :
    m_TrackID(aStep->GetTrack()->GetTrackID()),
    m_PID( aStep->GetTrack()->GetParticleDefinition()->GetParticleName() ),
    m_EnDep(aStep->GetTotalEnergyDeposit()),
    m_Position(aStep->GetPreStepPoint()->GetPosition()),
    m_Direction(aStep->GetPreStepPoint()->GetMomentumDirection())
{}

MyHit :: ~MyHit(){}

void MyHit::Print()
{

    G4cout
        << "Track ID: " << m_TrackID
        << "// Particle ID: " << m_PID
        << "// EnDep: " << setw(7) << G4BestUnit(m_EnDep, "Energy")
        << "// Position: " << setw(7) << G4BestUnit(m_Position, "Length")
        << "// Direction: " << m_Direction
        << G4endl;
}