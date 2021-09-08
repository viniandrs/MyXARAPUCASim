#include "../include/DetectorConstruction.h"

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;

#include "../include/SensorDetector.h"
#include "G4MaterialPropertiesTable.hh"
#include "G4VisAttributes.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

//Does the setup of the materials used to build the detector
void Detector::DefineMaterials()
{

}

G4VPhysicalVolume *Detector::Construct()
{   
    /*
    Defining materials and their optical properties. In this case we'll assign each material
    a Refractive index.
    */
    const int N = 2;
    G4double photonEnergy[N] = {1.65 * eV, 3.10 * eV};

    G4NistManager *nist = G4NistManager::Instance();

    //Water -------------------------------------------------------------------
    auto water = nist->FindOrBuildMaterial("G4_WATER");
    auto waterPropertiesTable = new G4MaterialPropertiesTable();
    water->SetMaterialPropertiesTable(waterPropertiesTable);
    G4double waterRIndex[N] = {1.33, 1.33};
    waterPropertiesTable->AddProperty("RINDEX", photonEnergy, waterRIndex, N);

    //Glass---------------------------------------------------------------------
    auto glass = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
    auto glassPropertiesTable = new G4MaterialPropertiesTable();
    glass->SetMaterialPropertiesTable(glassPropertiesTable);
    G4double glassRIndex[N] = {2.5, 2.5};
    glassPropertiesTable->AddProperty("RINDEX", photonEnergy, glassRIndex, N);

    G4double glassABSLength[N] = {10 * cm, 10 * cm};
    glassPropertiesTable->AddProperty("ABSLENGTH", photonEnergy, glassABSLength, N);

    //Air ----------------------------------------------------------------------
    auto air = nist->FindOrBuildMaterial("G4_AIR");
    auto airPropertiesTable = new G4MaterialPropertiesTable();
    air->SetMaterialPropertiesTable(airPropertiesTable);
    G4double airRIndex[N] = {1.0, 1.0};
    airPropertiesTable->AddProperty("RINDEX", photonEnergy, airRIndex, N);

    /*
    Building the detector
    */

    //World
    auto worldBox = new G4Box("MySolidWorld", 2 * m, 2 * m, 2 * m);
    auto logicWorld = new G4LogicalVolume(worldBox, air, "World");
    logicWorld->SetVisAttributes(new G4VisAttributes(false));
    auto physWorld = new G4PVPlacement(
        0,               //no rotation
        G4ThreeVector(), //at (0,0,0)
        logicWorld,      //its logical volume
        "World",         //its name
        0,               //its mother  volume
        false,           //no boolean operation
        0,               //copy number
        true);           //overlaps checking

    //Water box
    G4double waterSide = 20. * cm;
    auto waterBox = new G4Box("ExteriorBox", waterSide, waterSide, waterSide);
    auto logicWater = new G4LogicalVolume(waterBox, water, "LArVolume");
    logicWater->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
    auto physWaterBox = new G4PVPlacement(
        0,               //no rotation
        G4ThreeVector(), //at (0,0,0)
        logicWater,      //its logical volume
        "waterBox",      //its name
        logicWorld,      //its mother  volume
        false,           //no boolean operation
        0,               //copy number
        true);           //overlaps checking

    //Glass box
    G4double glassSide = 5. * cm;
    auto glassBox = new G4Box("ExteriorBox", glassSide, waterSide, waterSide);
    auto logicGlass = new G4LogicalVolume(glassBox, glass, "LArVolume");
    logicGlass->SetVisAttributes(new G4VisAttributes(G4Color::Gray()));
    auto physGlassBox = new G4PVPlacement(
        0,               //no rotation
        G4ThreeVector(), //at (0,0,0)
        logicGlass,      //its logical volume
        "glassBox",      //its name
        logicWater,      //its mother  volume
        false,           //no boolean operation
        0,               //copy number
        true);           //overlaps checking

    //Air-water surface--------------------------------------------------------------------------------------------------------
    auto surfaceAirWater = new G4OpticalSurface("Air->Water Surface");
    surfaceAirWater->SetModel(unified);
    surfaceAirWater->SetType(dielectric_dielectric);
    surfaceAirWater->SetFinish(polished);

    auto surfaceAirWaterProp = new G4MaterialPropertiesTable();
    surfaceAirWater->SetMaterialPropertiesTable(surfaceAirWaterProp);

    auto logSurfaceAirWater = new G4LogicalBorderSurface("Air->Water Interface", physWorld, physWaterBox, surfaceAirWater);
    auto logSurfaceWaterAir = new G4LogicalBorderSurface("Water->Air Interface", physWaterBox, physWorld, surfaceAirWater);

    //Reflectivity
    G4double reflecAirWater[N] = {0.9, 0.9};
    surfaceAirWaterProp->AddProperty("REFLECTIVITY", photonEnergy, reflecAirWater, N);

    //Water-glass surface--------------------------------------------------------------------------------------------------------
    auto surfaceWaterGlass = new G4OpticalSurface("Water->Glass Surface");
    surfaceWaterGlass->SetModel(unified);
    surfaceWaterGlass->SetType(dielectric_dielectric);
    surfaceWaterGlass->SetFinish(ground);
    surfaceWaterGlass->SetSigmaAlpha(1.0 * degree);

    auto surfaceWaterGlassProp = new G4MaterialPropertiesTable();
    surfaceWaterGlass->SetMaterialPropertiesTable(surfaceWaterGlassProp);

    auto logSurfaceWaterGlass = new G4LogicalBorderSurface("Water->Glass Interface", physWaterBox, physGlassBox, surfaceWaterGlass);
    auto logSurfaceGlassWater = new G4LogicalBorderSurface("Glass->Water Interface", physGlassBox, physWaterBox, surfaceWaterGlass);

    //Reflectivity

    G4double reflecWaterGlass[N] = {0.9, 0.9};
    surfaceWaterGlassProp->AddProperty("REFLECTIVITY", photonEnergy, reflecWaterGlass, N);

    /*
    Creating a hit-detector volume of silicon so we can edit its optical properties.
    */

    //importing silicon and setting up its properties
    auto silicon = nist->FindOrBuildMaterial("G4_Si");
    auto siliconPropertiesTable = new G4MaterialPropertiesTable();
    silicon->SetMaterialPropertiesTable(siliconPropertiesTable);

    G4double siliconRIndex[N] = { 1, 1 };
    siliconPropertiesTable->AddProperty("RINDEX", photonEnergy, siliconRIndex, N);

    G4double siliconABSLength[N] = {1 * nm, 1 * nm};
    siliconPropertiesTable->AddProperty("ABSLENGTH", photonEnergy, siliconABSLength, N);

    //geometry
    G4double size = 1 * m, thickness = 10 * mm, distance = 5 * cm;
    auto detectorBox = new G4Box("detectorBox", size / 2, thickness / 2, size / 2);
    auto logDetectorBox = new G4LogicalVolume(detectorBox, silicon, "LogicalDetectorBox");
    logDetectorBox->SetVisAttributes( new G4VisAttributes( G4Color::Red() ) );
    auto physDetectorBox = new G4PVPlacement(
        0,                                                //no rotation
        {0, - waterSide - thickness - distance, 0}, //position of the center of the detector
        logDetectorBox,                                   //its logical volume
        "World/Detector",                                 //its name
        logicWorld,                                       //its mother  volume
        false,                                            //no boolean operation
        0,                                                //copy number
        true);                                            //overlaps checking
        
    logDetectorBox->SetSensitiveDetector(new MySensorDetector("Photon Counter"));

    return physWorld;
}