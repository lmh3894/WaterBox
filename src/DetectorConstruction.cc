// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// 	Author: yskim
//

#include "DetectorConstruction.hh"

#include "G4Box.hh"

#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"

#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"

#include "SensitiveDetector.hh"

#include "G4PVReplica.hh"

#include "G4RotationMatrix.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction()
{
	BoxX = 30*cm;
	BoxY = 30*cm;
	BoxZ = 30*cm;

	VoxelSize = 0.1*cm;
	NumOfRepY = BoxY/VoxelSize;
	NumOfRepZ = BoxZ/VoxelSize;
}

DetectorConstruction::~DetectorConstruction()
{

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// World
	G4VSolid* sol_World = new G4Box("World", 0.5*m, 0.5*m, 0.5*m);
	G4Material* AIR = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
	G4LogicalVolume* lv_World = new G4LogicalVolume(sol_World, AIR, "World");
	G4VPhysicalVolume* pv_World =
			new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), lv_World, "World", 0, false, 0);



	// User geometry

	// Water box (x: 30 cm, y: 30 cm, z: 30 cm, at (0, 0, 0))
	G4VSolid* sol_WaterBox = new G4Box("WaterBox", BoxX/2, BoxY/2, BoxZ/2);
	G4Material* WATER = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
	G4LogicalVolume* lv_WaterBox = new G4LogicalVolume(sol_WaterBox, WATER, "WaterBox");
	G4VPhysicalVolume* pv_WaterBox =
			new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), lv_WaterBox, "WaterBox", lv_World, false, 10);




	// Replica of WaterBox (y-dir 0.01 cm slices)
	G4VSolid*sol_RepY = new G4Box("RepY",BoxX/2,VoxelSize/2,BoxZ/2);
	G4LogicalVolume*lv_RepY = new G4LogicalVolume(sol_RepY, WATER,"RepY");
	G4VPhysicalVolume*pv_RepY = new G4PVReplica("RepY",lv_RepY,lv_WaterBox,kYAxis,NumOfRepY,VoxelSize);

	// Replica of WaterBox (z-dir 0.01 cm slices)
	G4VSolid*sol_RepZ = new G4Box("RepZ",BoxX/2,VoxelSize/2,VoxelSize/2);
	G4LogicalVolume*lv_RepZ = new G4LogicalVolume(sol_RepZ, WATER,"RepZ");
	G4VPhysicalVolume*pv_RepZ = new G4PVReplica("RepZ",lv_RepZ,lv_RepY,kZAxis,NumOfRepZ,VoxelSize);


	// Visualization
	G4VisAttributes* va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
	va_World->SetForceWireframe(true);
	lv_World->SetVisAttributes(va_World);

	G4VisAttributes* va_WaterBox = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	va_WaterBox->SetForceWireframe(true);
	lv_WaterBox->SetVisAttributes(va_WaterBox);

	return pv_World;
}

void DetectorConstruction::ConstructSDandField()
{
	// Set sensitive detector
	SetSensitiveDetector("RepZ", new SensitiveDetector("detector"));
}
