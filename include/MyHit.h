#pragma once

#include "G4VHit.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

class MyHit : public G4VHit
{
public:
    MyHit(G4Step *aStep);
    ~MyHit();

    virtual void Print();

    inline int GetTrackID() {return m_TrackID;}
    inline G4String GetPID() {return m_PID;}
    inline double GetEnDep() {return m_EnDep;}
    inline G4ThreeVector GetPosition() {return m_Position;}
    inline G4ThreeVector GetDirection() {return m_Direction;}

private: //The data members (set of variables which will hold information)
    int m_TrackID;
    G4String m_PID;
    double m_EnDep;
    G4ThreeVector m_Position, m_Direction;
};
