/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
/* $Id$ */
//_________________________________________________________________________
//  A Reconstructed Particle in PHOS    
//  To become a general class of AliRoot ?       
//  Why should I put meaningless comments
//  just to satisfy
//  the code checker                 
//       
//*-- Author: Yves Schutz (SUBATECH)


// --- ROOT system ---

// --- Standard library ---


// --- AliRoot header files ---

#include "AliPHOSRecParticle.h"
#include "AliPHOSLoader.h" 
#include "AliStack.h" 
#include "TParticle.h"

ClassImp(AliPHOSRecParticle)


//____________________________________________________________________________
 AliPHOSRecParticle::AliPHOSRecParticle(const AliPHOSRecParticle & rp)
{
  // copy ctor

  fPHOSTrackSegment = rp.fPHOSTrackSegment ; 
  fDebug            = kFALSE ; 
  fType             = rp.fType ; 
  fIndexInList      = rp.fIndexInList ;

  fPdgCode     = rp.fPdgCode;
  fStatusCode  = rp.fStatusCode;
  fMother[0]   = rp.fMother[0];
  fMother[1]   = rp.fMother[1];
  fDaughter[0] = rp.fDaughter[0];
  fDaughter[1] = rp.fDaughter[1];
  fWeight      = rp.fWeight;
  fCalcMass    = rp.fCalcMass;
  fPx          = rp.fPx;
  fPy          = rp.fPy;
  fPz          = rp.fPz;
  fE           = rp.fE;
  fVx          = rp.fVx;
  fVy          = rp.fVy;
  fVz          = rp.fVz;
  fVt          = rp.fVt;
  fPolarTheta  = rp.fPolarTheta;
  fPolarPhi    = rp.fPolarPhi;
  fParticlePDG = rp.fParticlePDG; 
  
}

//____________________________________________________________________________
const Int_t AliPHOSRecParticle::GetNPrimaries() const  
{ 

  Int_t rv = 0 ;
  AliRunLoader* runget = AliRunLoader::GetRunLoader(AliConfig::fgkDefaultEventFolderName);
  if(runget == 0x0) 
   {
     Error("Init","Can not find run getter in event folder \"%s\"",GetTitle());
     return 0;
   }
  
  AliPHOSLoader* gime = dynamic_cast<AliPHOSLoader*>(runget->GetLoader("PHOSLoader"));
  if ( gime == 0 ) 
   {
     Error("Init","Could not obtain the Loader object !"); 
     return 0;
   } 

  gime->EmcRecPoint(gime->TrackSegment(GetPHOSTSIndex())->GetEmcIndex())->GetPrimaries(rv) ; 
  return rv ; 
}

//____________________________________________________________________________
const TParticle * AliPHOSRecParticle::GetPrimary(Int_t index) const  
{
  if ( index > GetNPrimaries() ) { 
    if (fDebug) 
      cout << "WARNING : AliPHOSRecParticle::GetPrimary -> " << index << " is larger that the number of primaries " 
	   <<  GetNPrimaries() << endl ;
    return 0 ; 
  } else { 
    Int_t dummy ; 
  AliRunLoader* runget = AliRunLoader::GetRunLoader(AliConfig::fgkDefaultEventFolderName);
  if(runget == 0x0) 
   {
     Error("Init","Can not find run getter in event folder \"%s\"",AliConfig::fgkDefaultEventFolderName.Data());
     return 0;
   }
  
  AliPHOSLoader* gime = dynamic_cast<AliPHOSLoader*>(runget->GetLoader("PHOSLoader"));
  if ( gime == 0 ) 
   {
     Error("Init","Could not obtain the Loader object !"); 
     return 0;
   } 

    Int_t primaryindex = gime->EmcRecPoint(gime->TrackSegment(GetPHOSTSIndex())->GetEmcIndex())->GetPrimaries(dummy)[index] ; 
    if (primaryindex >= 10000000) { // it comes from backgroundfile 
      if (fDebug) 
	cout << "WARNING : AliPHOSRecParticle::GetPrimary -> not a signal primary" << endl ;
      return 0 ; 
    } else 
      //      return gime->Primary(primaryindex) ; 
      return runget->Stack()->Particle(primaryindex) ; 
  } 
  return 0 ; 
}

