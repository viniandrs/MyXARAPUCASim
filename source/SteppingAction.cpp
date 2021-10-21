#include "../include/SteppingAction.h"
#include "../include/Constants.h"

#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepPoint.hh"

#include <unistd.h>

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}


void SteppingAction::UserSteppingAction(const G4Step *aStep)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    //============================ Collecting information about the Step ============================

    G4Track *track = aStep->GetTrack();
    G4String particle = track->GetDefinition()->GetParticleName();

    //PreStep volume info
    G4StepPoint *aPrePoint = aStep->GetPreStepPoint();
    G4VPhysicalVolume *aPrePV = aPrePoint->GetPhysicalVolume();
    G4String PreVolName = "";
    if (aPrePV)
        PreVolName = aPrePV->GetName();

    //PostStep volume info
    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *aPostPV = aPostPoint->GetPhysicalVolume();
    G4String PostVolName = "";
    if (aPostPV)
        PostVolName = aPostPV->GetName();

    //============================ Gathering data ============================

    //Alpha spectrum
    if (particle == "alpha")
    {
        
        if (PreVolName == "aluminumAlphaSource" && PostVolName == "Argon")
        {
            /* Get the kinetic energy an alpha particle has when it leaves the aluminum and
            register it on the spectrum */
            G4double kinEnergy = track->GetKineticEnergy();
            analysisManager->FillH1(1, kinEnergy);

            /* Registering the TrackID so we can track the alpha particle and its photons*/
            G4int trackID = track->GetTrackID();
            analysisManager->FillNtupleIColumn(1, 1, trackID);

        }
    }

    //Counting the photons that entered the Arapuca and registering its alpha's energy
    if (particle == "opticalphoton")
    {
        //Checking if the particle passed from the Argon to one of the detector screens
        if ((PreVolName == "Argon" && PostVolName == "detector2") ||
            (PreVolName == "Argon" && PostVolName == "detector1"))
        {
            analysisManager->FillNtupleIColumn(0, 0, 1); //registering the photon
            analysisManager->AddNtupleRow(0);
        }

        //Liquid argon scintillation spectrum

        /* If CreatorProcess is null, this means the track is created by the event generator, i.e. the primary track. 
        If it is not, it points to the process which created the track.*/

        else if (track->GetCreatorProcess() != NULL)
        {
            G4double photonEnergy = track->GetTotalEnergy()/eV;
            analysisManager->FillH1(0, photonEnergy);
        }
    }
}