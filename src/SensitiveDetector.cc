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

#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(G4String name)
:G4VSensitiveDetector(name)
{
	ofs.open("Water_190MeV_Gauss");

	BoxX = 30*cm;
	BoxY = 30*cm;
	BoxZ = 30*cm;

	VoxelSize = 0.1*cm;

	NumOfRepY = BoxY/VoxelSize;
	NumOfRepZ = BoxZ/VoxelSize;

}

SensitiveDetector::~SensitiveDetector()
{

	G4int nPrimaries = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
	for(G4int i=0; i<NumOfRepZ;i++)
	{
		for(G4int j=0; j<NumOfRepY;j++){
			ofs<<i<<"\t"<<j<<"\t"<<DEMatrix[i][j]/nPrimaries/MeV<<G4endl;
		}
		ofs<<G4endl;
	}
	ofs.close();

}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{

}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

	G4int RepZNo = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0);
	G4int RepYNo = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);

	//G4cout<<"RepZ no.:"<<RepZNo<<", RepY no.:"<<RepYNo<<G4endl;

	G4double dE = aStep->GetTotalEnergyDeposit();
	//G4cout<<"dE:"<<dE<<G4endl;

	DEMatrix[RepZNo][RepYNo]+= dE;


    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}
