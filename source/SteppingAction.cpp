#include "SteppingAction.h"
#include "Constants.h"
#include "PrimaryGeneratorAction.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4TrackStatus.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4StepPoint.hh"

/* Alteracao 5 5 17 */
#include "G4ios.hh"
#include "G4GeometryTolerance.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4PhysicalConstants.hh"
// */

#include <unistd.h>

// Arquivo txt de out put
#include <fstream>
using namespace std;
//ofstream OutPut_Step(TXT_FILE);

// Conversao de radianos para graus
#define RAD_TO_DEG 180 / pi
//================================================================================

SteppingAction::SteppingAction(EventAction *eventAction, RunAction *runAction, PrimaryGeneratorAction *PrimaryGeneratorAction)
    : G4UserSteppingAction(), fEventAction(eventAction), fRunAction(runAction), fPrimaryGeneratorAction(PrimaryGeneratorAction)
{

    fEventNumber = -1;
    contadorPhotons1 = 0;
    contadorPhotons2 = 0;
    contadorEnergia = 0;
    teste = 0;
    firstStep = true;
}

SteppingAction::~SteppingAction() {}

//================================================================================

void SteppingAction::UserSteppingAction(const G4Step *aStep)
{

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    //============================ Collecting information about the Step ============================ 
    fEventNumber = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    //Position
    G4double step_x = aStep->GetTrack()->GetPosition().x();
    G4double step_y = aStep->GetTrack()->GetPosition().y();
    G4double step_z = aStep->GetTrack()->GetPosition().z();

    //Track information
    G4Track *track = aStep->GetTrack();

    G4String parti = track->GetDefinition()->GetParticleName();
    G4String materia = track->GetMaterial()->GetName();
    const G4VProcess *creator = track->GetCreatorProcess();
    const G4DynamicParticle *dynParticle = track->GetDynamicParticle();
    G4double kinEnergy = track->GetKineticEnergy(); // Kinetic Energy of the track
    /*G4double MomentumDirection = track -> GetMomentumDirection();  // Direction of momentum  (should be an unit vector)
    //G4double Momentum = track -> GetMomentum(); // Total momentum of the track
     ->SEE WHY IS NOT WORKING*/
    G4double TotalEnergy = track->GetTotalEnergy(); // Total energy of the track

    //PreStep Info
    G4StepPoint *aPrePoint = aStep->GetPreStepPoint();
    G4VPhysicalVolume *aPrePV = aPrePoint->GetPhysicalVolume();
    G4String PreVolName = "";
    if (aPrePV)
        PreVolName = aPrePV->GetName();

    //PostStep Info
    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *aPostPV = aPostPoint->GetPhysicalVolume();
    G4String PostVolName = "";
    if (aPostPV)
        PostVolName = aPostPV->GetName();

    //some another info
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4int trackID = aStep->GetTrack()->GetTrackID();

    G4String processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    bool flag_alpha = true;
    //G4bool flag_generate = true;

    G4int a = 0;
    G4int b = 100;

    // Editado para
    //============================================================================
    G4ThreeVector Pre_momentum;
    G4ThreeVector Pos_momentum;

    //G4cout << trackID << G4endl;
    if (parti == "alpha")
    {
        initial_energy = kinEnergy / MeV + edep;
        if (flag_generate == true)
        {

            if (firstOfAll != true)
            {
                if (contadorPhotons1 != 0 || contadorPhotons2 != 0)
                {
                    if (contadorPhotons1 != 0)
                    {
                        contadorPhotons1 = contadorPhotons1 + 1;
                    }
                    if (contadorPhotons2 != 0)
                    {
                        contadorPhotons2 = contadorPhotons2 + 1;
                    }
                    analysisManager->FillH1(3, contadorPhotons1 + contadorPhotons2);

                    /*
                    if( contadorPhotons != contadorEnergia){
                    G4cout << " WARNING ************ Contador = " << contadorPhotons << " ID = " << contadorEnergia << " event = " << fEventNumber << G4endl;
                    }

                    if(contadorPhotons < (3620*myEnergy-1000)){
                        G4cout << " WARNING 2 ************ Contador = " << contadorPhotons << " myEnergy = " << myEnergy << " event = " << fEventNumber << G4endl;
                    }
                    else{
                        //cout << myEnergy << endl;
                    }
		    if(fEventNumber>=a && fEventNumber<=b){
                    	G4cout << " Registrando energia = " << myEnergy << " contagem = " << contadorPhotons << G4endl;

            	    }
		    */
                    analysisManager->FillH1(2, myEnergy);
                    analysisManager->FillNtupleDColumn(0, contadorPhotons1);                    //preenche o branch da TTree
                    analysisManager->FillNtupleDColumn(1, contadorPhotons2);                    //preenche o branch da TTree
                    analysisManager->FillNtupleDColumn(2, contadorPhotons1 + contadorPhotons2); //preenche o branch da TTree
                    analysisManager->FillNtupleDColumn(3, myEnergy);                            //preenche o branch da TTree
                    analysisManager->FillNtupleDColumn(4, x);
                    analysisManager->FillNtupleDColumn(5, y);
                    analysisManager->FillNtupleDColumn(6, z);
                    analysisManager->AddNtupleRow(1);
                    //G4cout << "finished num. de fotons =" << contadorPhotons << G4endl;
                    //G4cout << "energia total de fotons =" << contadorEnergia/MeV << G4endl;
                }
                //cout << "zerando" << endl;
                contadorEnergia = 0;
                contadorPhotons1 = 0;
                contadorPhotons2 = 0;
            }
            else
            {
                firstOfAll = false;
            }

            flag_alpha = false;

            myEnergy = 0;
            x = 0;
            y = 0;
            z = 0;
        }

        x = step_x;
        y = step_y;
        z = step_z;
        if (edep != 0)
        {
            temp_edep = edep;
        }

        //G4cout << parti << " Initial Energy = " << initial_energy << "; KE = "<< kinEnergy/MeV << "; Edep = " << edep << "; preVol = "<< PreVolName << "; postVol = "<< PostVolName << "; processo = " << processName << " eventID = " << fEventNumber << " ID = " << trackID << G4endl;

        isAlpha = true;
    }

    if (creator)
    {
        if (parti == "opticalphoton")
        {

            if (PreVolName == "detector1" || PreVolName == "detector2")
            {
                if (temp_ID == trackID)
                {                                            // estaria contando alguem que n entrou no detector
                    contadorPhotons1 = contadorPhotons1 - 1; // subtrai
                    contadorPhotons2 = contadorPhotons2 - 1; // subtrai
                    G4cout << " WARNING DETECTOR ----> comming from " << PreVolName << " to " << PostVolName << G4endl;
                }
            }

            if (isAlpha == true)
            {
                myEnergy += temp_edep;
            }
            isAlpha = false;
            if (PostVolName == "detector1" || PostVolName == "detector2")
            {

                //if(fEventNumber>=a && fEventNumber<=b && isAlpha == true){
                //        G4cout << "Energy that generated photons = " << temp_edep << " in a total of " << myEnergy << ". This should generate " << 3621*temp_edep << G4endl;

                //}

                /*if(contadorEnergia<trackID){
                 c ontado*rEnergia = trackID;
                 if(fEventNumber>=a && fEventNumber<=b){
                     G4cout << "registrando ID " << contadorEnergia << G4endl;
            }
            }

            */

                if (temp_ID != 0 && temp_ID != trackID)
                {
                    if (PostVolName == "detector1")
                    {
                        contadorPhotons1 = contadorPhotons1 + 1.0;
                    }
                    if (PostVolName == "detector2")
                    {
                        contadorPhotons2 = contadorPhotons2 + 1.0;
                    }
                    analysisManager->FillH1(1, kinEnergy / eV);
                    //if(fEventNumber>=a && fEventNumber<=b){
                    //   G4cout << "contando photons " << contadorPhotons << G4endl;

                    //}
                    temp_ID = trackID;
                }

                G4String creatorName = creator->GetProcessName();
                //G4cout << "Debuga fdp ... " << creatorName << G4endl;
                /*if(fEventNumber>=a && fEventNumber<=b){
                 *       G4cout << "Particle " << parti << " created by " << creatorName << G4endl;
                 *       G4cout << parti << "; preVol = "<< PreVolName << "; postVol = "<< PostVolName << "; processo = " << processName << " eventID = " << fEventNumber << " ID = " << trackID << G4endl;
            }*/
            }
        }
    }
    else
    {
        if (parti != "alpha")
        {
            cout << "Particle without creator" << endl;
            G4cout << parti << " Energy = " << kinEnergy / eV << "; KE = " << kinEnergy / eV << "; Edep = " << edep << "; preVol = " << PreVolName << "; postVol = " << PostVolName << "; processo = " << processName << " eventID = " << fEventNumber << " ID = " << trackID << G4endl;
        }
    }

    if (flag_generate == true && parti == "alpha")
    {
        //G4cout << "começo de um evento: gera uma alpha" << G4endl;
        flag_generate = false;
    }
}