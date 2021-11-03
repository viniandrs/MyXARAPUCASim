#include "../include/DetectorConstruction.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4NistManager.hh"
#include "CLHEP/Units/SystemOfUnits.h"

#include "G4MaterialPropertiesTable.hh"
#include "G4VisAttributes.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

using namespace CLHEP;

//Constructor for member initializers
Detector::Detector() : G4VUserDetectorConstruction(), fScoringVolume(0) {}

Detector::~Detector() { delete fScoringVolume; }

G4VPhysicalVolume *Detector::Construct()
{
    G4NistManager *nistManager = G4NistManager::Instance();

    //######################### Defining materials from NIST table #########################

    //These elements may be excluded if we don't use them further in the simulation
    nistManager->FindOrBuildElement("Be");
    nistManager->FindOrBuildElement("N");
    nistManager->FindOrBuildElement("O");
    nistManager->FindOrBuildElement("Ti");
    //-----------------------------------------------------------------------------

    auto Al = nistManager->FindOrBuildElement("Al");
    auto H = nistManager->FindOrBuildElement("H");
    auto Cl = nistManager->FindOrBuildElement("Cl");
    auto C = nistManager->FindOrBuildElement("C");
    auto Si = nistManager->FindOrBuildElement("Si");
    auto Cr = nistManager->FindOrBuildElement("Cr");
    auto Mn = nistManager->FindOrBuildElement("Mn");
    auto Fe = nistManager->FindOrBuildElement("Fe");
    auto Ni = nistManager->FindOrBuildElement("Ni");
    auto Air = nistManager->FindOrBuildMaterial("G4_AIR");
    auto Teflon = nistManager->FindOrBuildMaterial("G4_TEFLON");
    auto LAr = nistManager->FindOrBuildMaterial("G4_lAr");

    //######################### Creating materials from NIST table #########################

    //Declaring variables that will be used to build the materials
    G4double density;
    G4int nComponents;
    G4double fractionmass;
    G4double z, a; //atomic number and molar mass, respectively
    G4int nAtoms;  //number of atoms
    G4double temperature;
    G4double pressure;

    //Stainless steel----------------------------------------------------------------------------
    G4Material *StainlessSteel = new G4Material("StainlessSteel", density = 8.06 * g / cm3, nComponents = 6);
    StainlessSteel->AddElement(C, fractionmass = 0.001);
    StainlessSteel->AddElement(Si, fractionmass = 0.007);
    StainlessSteel->AddElement(Cr, fractionmass = 0.18);
    StainlessSteel->AddElement(Mn, fractionmass = 0.01);
    StainlessSteel->AddElement(Fe, fractionmass = 0.712);
    StainlessSteel->AddElement(Ni, fractionmass = 0.09);

    //PVC C3H4Cl----------------------------------------------------------------------------------
    G4Material *PVC = new G4Material("PVC", density = 1.16 * g / cm3, 3);
    PVC->AddElement(C, nAtoms = 5);
    PVC->AddElement(H, nAtoms = 4);
    PVC->AddElement(Cl, nAtoms = 1);

    //Liquid nitrogen ----------------------------------------------------------------------------
    G4Material *NL2 = new G4Material("LiquidNitrogen", z = 7, a = 14.01 * g / mole, density = 0.808 * g / cm3, kStateLiquid, temperature = 77.0 * kelvin);

    //Aluminum ------------------------------------------------------------------------------------
    G4Material *Aluminium = new G4Material("Aluminio", density = 2.70 * g / cm3, nComponents = 1);
    Aluminium->AddElement(Al, nAtoms = 1);

    //Vacuum ---------------------------------------------------------------------------------------
    G4Material *Vacuum = new G4Material("interGalactic", z = 1, a = 1.008 * g / mole, density = 1.e-25 * g / cm3, kStateGas,
                                        temperature = 2.73 * kelvin, pressure = 3.e-18 * pascal);

    //######################### Declaring variables #########################

    G4int NUMENTRIES;

    //Optical properties
    NUMENTRIES = 14;
    G4double Scint_PP[NUMENTRIES] = {9.2 * eV, 9.3 * eV, 9.4 * eV, 9.5 * eV,
                                     9.6 * eV, 9.7 * eV, 9.76377 * eV, 9.8 * eV,
                                     9.9 * eV, 10.0 * eV, 10.05 * eV, 10.1 * eV,
                                     10.2 * eV, 10.3 * eV};

    G4double Scint_FAST[NUMENTRIES] = {0.000856175, 0.00839924, 0.0528321,
                                       0.213077, 0.551004, 0.913594, 1,
                                       0.971252, 0.66205, 0.161926, 0.289355,
                                       0.0810867, 0.0145697, 0.00167853};

    G4double Scint_SLOW[NUMENTRIES] = {0.000856175, 0.00839924, 0.0528321,
                                       0.213077, 0.551004, 0.913594, 1,
                                       0.971252, 0.66205, 0.161926, 0.289355,
                                       0.0810867, 0.0145697, 0.00167853};

    //UV foton spectrum array
    G4double PhotonEnergy[NUMENTRIES] = {4.96 * eV, 5.39 * eV, 6.20 * eV, 6.89 * eV,
                                         8.27 * eV, 8.86 * eV, 9.69 * eV, 10.3 * eV,
                                         11.3 * eV, 12.4 * eV, 13.8 * eV, 15.5 * eV, 24.8 * eV, 24.9 * eV};

    G4double reflec_ino = 0.20;
    G4double Reflec_inox[NUMENTRIES] = {reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino,
                                        reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino};

    //proprieta ottiche scint (da trovare)
    //ENTENDER ESSES DADOS, APARENTEMENTE ELA OS RECEBEU DE ALGUÉM DA ITALIA
    NUMENTRIES = 5;
    G4double PhotonEnergyN[35] = {1.8002708291 * eV, 1.9481638324 * eV, 1.8712998009 * eV, 2.031612752 * eV, 2.122530612 * eV, 2.2219671291 * eV, 2.3311783976 * eV, 2.4516802288 * eV, 2.5853189354 * eV, 2.7343666014 * eV, 2.9016513225 * eV, 3.0907383696 * eV, 3.306187123 * eV, 3.5539235215 * eV, 3.8417935227 * eV, 4.1804092554 * eV, 4.5844859047 * eV, 5.075036896 * eV, 5.6831472915 * eV, 6.456829363 * eV, 7.3367183961 * eV, 8.1468599034 * eV, 8.7942508537 * eV, 9.3735970883 * eV, 9.7402642614 * eV, 10.0346587111 * eV, 10.2939327045 * eV, 10.401432965 * eV, 10.5669602239 * eV, 10.7726825649 * eV, 10.7960361308 * eV, 10.8904717926 * eV, 11.0108653265 * eV, 11.1339505034 * eV, 11.2217603734 * eV};

    G4double n_lAr[35] = {1.2249193575067825, 1.224940655396017, 1.224940655396017, 1.2252814216237677, 1.22538791106994, 1.225686081519222, 1.2269000612055847, 1.2272834232118046, 1.2280714451134787, 1.2281779345596509, 1.2294345100244823, 1.2309466601601273, 1.2320328525110833, 1.2330125554158673, 1.2348441738900287, 1.2377406868259118, 1.2414465195527031, 1.2461320551842787, 1.2536928058625028, 1.2648954955998148, 1.28143463770843, 1.301258737209988, 1.3225062859789587, 1.345444112684444, 1.364484425660028, 1.3856545275590553, 1.4069737146827237, 1.4279201887447894, 1.4510731241315424, 1.4716468851320057, 1.5005409993414198, 1.5292534159333024, 1.550184981472904, 1.5699536822603057, 1.5907957966651227};

    G4double abs_pvc[14] = {0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm};

    G4double PhotonEnergy2[NUMENTRIES] = {2.034 * eV, 2.341 * eV, 2.757 * eV, 3.353 * eV, 4.136 * eV};

    G4double l_lAr[NUMENTRIES] = {3448 * m, 4082 * m, 6329 * m, 9174 * m, 12346 * m};

    //World size
    G4double env_sizeXY = 300 * cm, env_sizeZ = 300 * cm;
    G4double world_sizeXY = 5 * env_sizeXY;
    G4double world_sizeZ = 5 * env_sizeZ;

    //Cyllinder's outer and inner radios and height (ask the information to Ana)
    G4double R_b = 26.5 * cm;
    G4double R_a = 20.0 * cm;
    G4double H_b = 43.5 * cm;
    G4double bigger_base_height = 8.5 * cm;

    //Smaller cyllinder's outer and inner radios and height
    G4double r_b = 15.3 * cm;
    G4double r_a = 14.9 * cm;
    G4double h_b = 35.0 * cm;
    G4double smaller_height = 1.5 * cm;

    //??????????
    G4double R = 145.0 * mm;
    G4double esp = 20.0 * mm;
    G4double dif_h = 10.0 * cm;

    //ARAPUCA
    G4double b_maior = 12.3 * cm;
    G4double b_menor = 10.0 * cm;
    G4double a_maior = 10.0 * cm;
    G4double a_menor = 7.5 * cm;
    G4double espeAra = 1.5 * cm;

    //Alpha source support
    G4double esp_alpha = 1.0 * cm;
    G4double altura = 10.0 * cm;
    G4double largura = 5.0 * cm;
    G4double diam_buraco = 1.0 * cm;
    G4double esp_aluminio = 140 * um;

    //UM DIA VOU ENTENDER ESSES VETORES
    G4ThreeVector pos1  = G4ThreeVector(0, 0, 0);
    G4ThreeVector pos2  = G4ThreeVector(0, 0, 0.5 * (8.5 * cm));
    G4ThreeVector pos3  = G4ThreeVector(0, 0, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height - 2.0 * esp));
    G4ThreeVector pos4  = G4ThreeVector(0, 0, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height) + dif_h);
    G4ThreeVector pos5  = G4ThreeVector(0, -0.75 * cm, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h - esp));
    G4ThreeVector pos6  = G4ThreeVector(0, 3.0 * cm, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h - esp));
    G4ThreeVector pos7  = G4ThreeVector(0, 3.5 * cm - 0.5 * esp_aluminio, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h - esp));
    G4ThreeVector pos8  = G4ThreeVector(0, 0, 0.5 * (8.5 * cm));
    G4ThreeVector pos9  = G4ThreeVector(0, 0, 0.5 * (8.5 * cm + smaller_height));
    G4ThreeVector pos10 = G4ThreeVector(0, 4.0 * cm - esp_aluminio, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h - esp));
    G4ThreeVector pos11 = G4ThreeVector(0, -0.75 * cm + 0.25 * espeAra, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h - esp));
    G4ThreeVector pos12 = G4ThreeVector(0, -0.75 * cm - 0.25 * espeAra, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h - esp));

    G4ThreeVector trans1  = G4ThreeVector(0, 0, -0.5 * (H_b - bigger_base_height));
    G4ThreeVector trans2  = G4ThreeVector(0, 0, -0.5 * (h_b - smaller_height));
    G4ThreeVector trans3  = G4ThreeVector(0, 0, 0);
    G4ThreeVector trans4  = G4ThreeVector(0, 3.0 * cm, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h - esp));
    G4ThreeVector trans5  = G4ThreeVector(0, 0, -0.5 * (h_b - smaller_height) + 0.5 * esp);
    G4ThreeVector trans6  = G4ThreeVector(0, 0, 0.5 * (-0.5 * H_b + 0.5 * bigger_base_height + dif_h));
    G4ThreeVector trans7  = G4ThreeVector(0, -0.75 * cm, -0.5 * (h_b + smaller_height) + 0.5 * esp + a_menor);
    G4ThreeVector trans8  = G4ThreeVector(0, 3.0 * cm, -0.5 * (h_b + smaller_height) + 0.5 * esp + a_menor);
    G4ThreeVector trans9  = G4ThreeVector(0, 3.5 * cm - 0.5 * esp_aluminio, -0.5 * (h_b + smaller_height) + 0.5 * esp + a_menor);
    G4ThreeVector trans10 = G4ThreeVector(0, 4.0 * cm - esp_aluminio, -0.5 * (h_b + smaller_height) + 0.5 * esp + a_menor);
    G4ThreeVector trans11 = G4ThreeVector(0, -0.75 * cm + 0.25 * espeAra, -0.5 * (h_b + smaller_height) + 0.5 * esp + a_menor);
    G4ThreeVector trans12 = G4ThreeVector(0, -0.75 * cm - 0.25 * espeAra, -0.5 * (h_b + smaller_height) + 0.5 * esp + a_menor);

    //######################### Creating the world #########################
    G4Box *solidWorld = new G4Box("World", 0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0,               //no rotation
                                                     G4ThreeVector(), //at (0,0,0)
                                                     logicWorld,      //its logical volume
                                                     "World",         //its name
                                                     0,               //its mother  volume
                                                     true,            //no boolean operation
                                                     0,               //copy number
                                                     true);           //overlaps checking

    //######################### Creating the cryostat #########################

    //creating the outer part of the cryostat
    G4Tubs *bigger_base = new G4Tubs("biggerBase",             //name
                                     0,                        //inner radius
                                     0.5 * R_a,                //outer radius
                                     0.5 * bigger_base_height, //height
                                     0.0 * deg,                //starting angle
                                     360 * deg);               //segment angle

    G4Tubs *cryostat_wall = new G4Tubs("cryostatWall",
                                       0.5 * R_a,
                                       0.5 * R_b,
                                       0.5 * H_b,
                                       0.0 * deg,
                                       360 * deg);

    //Union solid of the cryostat wall with the base
    G4UnionSolid *outer_cryostat = new G4UnionSolid("outerCryostat", cryostat_wall, bigger_base, 0, trans1);

    G4LogicalVolume *logicalCriostatoExt = new G4LogicalVolume(outer_cryostat,   //volume
                                                               StainlessSteel,   //material
                                                               "outerCryostat"); //name

    G4VPhysicalVolume *physCriostatoExt = new G4PVPlacement(0,                   //no rotation
                                                            pos1,                //at position
                                                            logicalCriostatoExt, //its logical volume
                                                            "criostatoExt",      //its name
                                                            logicWorld,          //its mother  volume
                                                            true,                //no boolean operation
                                                            0,                   //copy number
                                                            true);

    fScoringVolume = logicalCriostatoExt;

    //creating the inner part of the cryostat and putting it in the outer part
    G4Tubs *smaller_base = new G4Tubs("smallerBase",
                                      0,
                                      0.5 * r_a,
                                      0.5 * smaller_height,
                                      0.0 * deg,
                                      360 * deg);

    G4Tubs *inner_cryostat = new G4Tubs("innerCryostat",
                                        0.5 * r_a,
                                        0.5 * r_b,
                                        0.5 * h_b,
                                        0.0 * deg,
                                        360 * deg);

    G4UnionSolid *Criostato_int = new G4UnionSolid("criostato_int", inner_cryostat, smaller_base, 0, trans2);

    G4LogicalVolume *logicalCriostatoInt = new G4LogicalVolume(Criostato_int, StainlessSteel, "criostato_int");

    G4VPhysicalVolume *physCriostatoInt = new G4PVPlacement(0,                   //no rotation
                                                            pos2,                //at position
                                                            logicalCriostatoInt, //its logical volume
                                                            "criostatoInt",      //its name
                                                            logicWorld,          //its mother  volume
                                                            true,                //no boolean operation
                                                            0,                   //copy number
                                                            NTP_Temperature);    //??? QUE TEMPERATURA É ESSA?

    fScoringVolume = logicalCriostatoInt;

    //######################### Creating the ARAPUCA #########################

    //base cyllinders
    G4Tubs *ARAbase1 = new G4Tubs("arabase1", 0, 0.5 * R, 0.5 * esp, 0.0 * deg, 360 * deg);

    G4LogicalVolume *logicalARAbase1 = new G4LogicalVolume(ARAbase1, PVC, "ARABase1");

    G4VPhysicalVolume *physARAbase1 = new G4PVPlacement(0,               //no rotation
                                                        pos3,            //at position
                                                        logicalARAbase1, //its logical volume
                                                        "arapucabase1",  //its name
                                                        logicWorld,      //its mother  volume
                                                        true,            //no boolean operation
                                                        0,               //copy number
                                                        true);

    fScoringVolume = logicalARAbase1;

    G4Tubs *ARAbase2 = new G4Tubs("arabase2", 0, 0.5 * R, 0.5 * esp, 0.0 * deg, 360 * deg);

    G4LogicalVolume *logicalARAbase2 = new G4LogicalVolume(ARAbase2, PVC, "ARAbase2");

    G4VPhysicalVolume *physARAbase2 = new G4PVPlacement(0,               //no rotation
                                                        pos4,            //at position
                                                        logicalARAbase2, //its logical volume
                                                        "arapucabase2",  //its name
                                                        logicWorld,      //its mother  volume
                                                        true,            //no boolean operation
                                                        0,               //copy number
                                                        true);

    fScoringVolume = logicalARAbase2;

    //ARAPUCA frame

    G4Box *ArapucaExt = new G4Box("ArapucaExt", 0.5 * b_maior, 0.5 * espeAra, 0.5 * b_menor);
    G4Box *ArapucaInt = new G4Box("ArapucaInt", 0.5 * a_maior, 0.5 * (espeAra + 1.0 * cm), 0.5 * a_menor);

    G4SubtractionSolid *Arapuca = new G4SubtractionSolid("arapuca", ArapucaExt, ArapucaInt, 0, trans3);

    G4LogicalVolume *logicalARApuca = new G4LogicalVolume(Arapuca, Teflon, "logicalArapuca");

    G4VPhysicalVolume *physARApuca = new G4PVPlacement(0,              //no rotation
                                                       pos5,           //at position
                                                       logicalARApuca, //its logical volume
                                                       "ARAPUCA",      //its name
                                                       logicWorld,     //its mother  volume
                                                       true,           //no boolean operation
                                                       0,              //copy number
                                                       true);

    fScoringVolume = logicalARApuca;

    //detector Arapuca

    //face facing the detector
    G4Box *Detector1 = new G4Box("detector1", 0.5 * a_maior, 0.25 * (espeAra), 0.5 * a_menor);

    G4LogicalVolume *logicalDetector1 = new G4LogicalVolume(Detector1, Teflon, "LogDetector1");

    G4VPhysicalVolume *physDetector1 = new G4PVPlacement(0,                //no rotation
                                                         pos11,            //at position
                                                         logicalDetector1, //its logical volume
                                                         "detector1",      //its name
                                                         logicWorld,       //its mother  volume
                                                         true,             //no boolean operation
                                                         0,                //copy number
                                                         true);

    fScoringVolume = logicalDetector1;

    //face back to the detector

    G4Box *Detector2 = new G4Box("detector2", 0.5 * a_maior, 0.25 * (espeAra), 0.5 * a_menor);

    G4LogicalVolume *logicalDetector2 = new G4LogicalVolume(Detector2, Teflon, "LogDetector2");

    G4VPhysicalVolume *physDetector2 = new G4PVPlacement(0,                //no rotation
                                                         pos12,            //at position
                                                         logicalDetector2, //its logical volume
                                                         "detector2",      //its name
                                                         logicWorld,       //its mother  volume
                                                         true,             //no boolean operation
                                                         0,                //copy number
                                                         true);

    fScoringVolume = logicalDetector2;

    //Alpha source support

    G4Box *SuporteAlphaBox = new G4Box("Suporte", 0.5 * largura, 0.5 * altura, 0.5 * esp_alpha);
    G4Tubs *aluminum_alpha = new G4Tubs("aluminio_alpha", 0, 0.5 * diam_buraco, 0.5 * esp_aluminio, 0.0 * deg, 360 * deg);

    G4RotationMatrix *rotacao = new G4RotationMatrix(); //talvez de ruim pq não incluimos essa classe
    rotacao->rotateY(0. * deg);
    rotacao->rotateX(90. * deg);
    rotacao->rotateZ(180. * deg);

    G4SubtractionSolid *AlphaSupport = new G4SubtractionSolid("SuporteAlpha", SuporteAlphaBox, aluminum_alpha, rotacao, pos6);

    G4LogicalVolume *logicalAlpha = new G4LogicalVolume(AlphaSupport, PVC, "logAlpha");

    G4VPhysicalVolume *physAlpha = new G4PVPlacement(rotacao,        //no rotation
                                                     pos7,           //at position
                                                     logicalAlpha,   //its logical volume
                                                     "alphaSupport", //its name
                                                     logicWorld,     //its mother  volume
                                                     true,           //no boolean operation
                                                     0,              //copy number
                                                     true);

    fScoringVolume = logicalAlpha;

    //Alpha source itself

    G4LogicalVolume *logical_aluminum_alpha = new G4LogicalVolume(aluminum_alpha, Aluminium, "logAluminio");

    G4VPhysicalVolume *phys_Alpha_aluminio = new G4PVPlacement(rotacao,                //no rotation
                                                               pos6,                   //at position
                                                               logical_aluminum_alpha, //its logical volume
                                                               "aluminumAlphaSource",  //its name
                                                               logicWorld,             //its mother  volume
                                                               true,                   //no boolean operation
                                                               0,                      //copy number
                                                               true);

    //Filling the space between the cyllinders with liquid nitrogen

    G4Tubs *fill_step1 = new G4Tubs("fill_step1", 0.5 * (r_b + 0.01 * cm), 0.5 * (R_a - 0.01 * cm), 0.5 * h_b, 0.0 * deg, 360 * deg);

    G4LogicalVolume *logical_NL2 = new G4LogicalVolume(fill_step1, NL2, "logNL2");

    G4VPhysicalVolume *physNL2 = new G4PVPlacement(0,           //no rotation
                                                   pos8,        //at position
                                                   logical_NL2, //its logical volume
                                                   "LNitrogen", //its name
                                                   logicWorld,  //its mother  volume
                                                   true,        //no boolean operation
                                                   0,           //copy number
                                                   true);

    fScoringVolume = logical_NL2;

    //Filling the inner cyllinder with LAr but leaving room for the inner geometries so there's no overlaps

    G4Tubs *fill_step2 = new G4Tubs("fill_step2", 0, 0.5 * (r_a - 0.01 * cm), 0.5 * (h_b - smaller_height), 0.0 * deg, 360 * deg);

    G4SubtractionSolid *sub1 = new G4SubtractionSolid("sub1", fill_step2, ARAbase1, 0, trans5);
    G4SubtractionSolid *sub2 = new G4SubtractionSolid("sub2", sub1, ARAbase2, 0, trans6);
    G4SubtractionSolid *sub3 = new G4SubtractionSolid("sub3", sub2, Arapuca, 0, trans7);
    G4SubtractionSolid *sub4 = new G4SubtractionSolid("sub4", sub3, Detector1, 0, trans11);
    G4SubtractionSolid *sub5 = new G4SubtractionSolid("sub5", sub4, Detector2, 0, trans12);
    G4SubtractionSolid *sub6 = new G4SubtractionSolid("sub6", sub5, aluminum_alpha, rotacao, trans8);
    G4SubtractionSolid *sub7 = new G4SubtractionSolid("sub7", sub6, AlphaSupport, rotacao, trans9);

    //NÃO CONHEÇO ESSAS PROPRIEDADES, PEDIR AJUDA PARA OS PROFS
    //Adding optical properties to LAr

    G4MaterialPropertiesTable *LAr_props = new G4MaterialPropertiesTable();
    LAr_props->AddProperty("FASTCOMPONENT", Scint_PP, Scint_FAST, NUMENTRIES = 14);
    LAr_props->AddProperty("SLOWCOMPONENT", Scint_PP, Scint_SLOW, NUMENTRIES = 14);
    LAr_props->AddProperty("RINDEX", PhotonEnergyN, n_lAr, NUMENTRIES = 5);
    LAr_props->AddProperty("ABSLENGTH", PhotonEnergy2, l_lAr, NUMENTRIES = 5);

    LAr_props->AddConstProperty("SCINTILLATIONYIELD", 3621.0 / MeV);
    LAr_props->AddConstProperty("RESOLUTIONSCALE", 1.);

    LAr_props->AddConstProperty("FASTTIMECONSTANT", 6. * ns);
    LAr_props->AddConstProperty("SLOWTIMECONSTANT", 1.8 * us);
    LAr_props->AddConstProperty("YIELDRATIO", 0.25);

    LAr->SetMaterialPropertiesTable(LAr_props);

    LAr->GetIonisation()->SetBirksConstant(0.0000001 * mm / MeV);

    G4LogicalVolume *logical_LAr = new G4LogicalVolume(sub7, LAr, "logLAr");

    G4VPhysicalVolume *physLAr = new G4PVPlacement(0,           //no rotation
                                                   pos9,        //at position
                                                   logical_LAr, //its logical volume
                                                   "Argon",     //its name
                                                   logicWorld,  //its mother  volume
                                                   true,        //no boolean operation
                                                   0,           //copy number
                                                   true);

    fScoringVolume = logical_LAr;

    //######################### Optical properties #########################

    //PVC

    G4MaterialPropertiesTable *prop_pvc = new G4MaterialPropertiesTable();
    prop_pvc->AddProperty("ABSLENGTH", PhotonEnergy, abs_pvc, NUMENTRIES);
    PVC->SetMaterialPropertiesTable(prop_pvc);

    //Stainless steel
    G4MaterialPropertiesTable *ssteel_props = new G4MaterialPropertiesTable();
    ssteel_props->AddProperty("REFLECTIVITY", Scint_PP, Reflec_inox, NUMENTRIES);

    G4OpticalSurface *ssteel_surface = new G4OpticalSurface("StainlessSteelSurface");

    ssteel_surface->SetModel(glisur);
    ssteel_surface->SetType(dielectric_metal);
    ssteel_surface->SetFinish(polished);

    ssteel_surface->SetMaterialPropertiesTable(ssteel_props);

    new G4LogicalSkinSurface("InoxProperty", logicWorld, ssteel_surface);

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  Colors @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //red, green, blue
    G4VisAttributes *criostato_externo = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
    criostato_externo->SetForceSolid(false);
    logicalCriostatoExt->SetVisAttributes(criostato_externo);

    G4VisAttributes *criostato_interno = new G4VisAttributes(G4Colour(0.87, 0.87, 0.87, 0.2));
    criostato_interno->SetForceSolid(false);
    logicalCriostatoInt->SetVisAttributes(criostato_interno);

    G4VisAttributes *ARAbase_1 = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    logicalARAbase1->SetVisAttributes(ARAbase_1);

    G4VisAttributes *ARAbase_2 = new G4VisAttributes(G4Colour(0.8, 0.3, 0.0));
    logicalARAbase2->SetVisAttributes(ARAbase_2);

    G4VisAttributes *Arapuca_color = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
    logicalARApuca->SetVisAttributes(Arapuca_color);

    G4VisAttributes *alpha_color = new G4VisAttributes(G4Colour(0.41, 0.41, 0.41, 0.7));
    logicalAlpha->SetVisAttributes(alpha_color);

    G4VisAttributes *NL2_color = new G4VisAttributes(G4Colour(0., 0., 0., 0.1));
    logical_NL2->SetVisAttributes(NL2_color);

    G4VisAttributes *LAr_color = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.1));
    logical_LAr->SetVisAttributes(LAr_color);

    G4VisAttributes *Aluminium_color = new G4VisAttributes(G4Colour(0.66, 0.66, 0.66, 0.8));
    logical_aluminum_alpha->SetVisAttributes(Aluminium_color);

    G4VisAttributes *Detector_color1 = new G4VisAttributes(G4Colour(0.80, 0.36, 0.36, 0.4));
    logicalDetector1->SetVisAttributes(Detector_color1);

    G4VisAttributes *Detector_color2 = new G4VisAttributes(G4Colour(0.4, 1.0, 0.7, 0.4));
    logicalDetector2->SetVisAttributes(Detector_color2);

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     The End    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    return physWorld;
}