// $Id$

/**************************************************************************
 * This file is property of and copyright by the ALICE HLT Project        * 
 * ALICE Experiment at CERN, All rights reserved.                         *
 *                                                                        *
 * Primary Authors: Matthias Richter <Matthias.Richter@ift.uib.no>        *
 *                  Timm Steinbeck <timm@kip.uni-heidelberg.de>           *
 *                  for The ALICE HLT Project.                            *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/** @file   AliHLTTPCSliceTrackerComponent.cxx
    @author Timm Steinbeck, Matthias Richter
    @date   
    @brief  The TPC conformal mapping tracker component.
*/

// see header file for class documentation
// or
// refer to README to build package
// or
// visit http://web.ift.uib.no/~kjeks/doc/alice-hlt

#if __GNUC__>= 3
using namespace std;
#endif

#include "AliHLTTPCSliceTrackerComponent.h"
#include "AliHLTTPCTransform.h"
#include "AliHLTTPCConfMapper.h"
#include "AliHLTTPCVertex.h"
#include "AliHLTTPCVertexData.h"
#include "AliHLTTPCClusterDataFormat.h"
#include "AliHLTTPCTransform.h"
#include "AliHLTTPCTrackSegmentData.h"
#include "AliHLTTPCTrackArray.h"
#include "AliHLTTPCTrackletDataFormat.h"
#include "AliHLTTPCInterMerger.h"
#include "AliHLTTPCMemHandler.h"
#include "AliHLTTPCDefinitions.h"
//#include "AliHLTTPC.h"
//#include <stdlib.h>
//#include <cerrno>

// this is a global object used for automatic component registration, do not use this
AliHLTTPCSliceTrackerComponent gAliHLTTPCSliceTrackerComponent;

/** ROOT macro for the implementation of ROOT specific class methods */
ClassImp(AliHLTTPCSliceTrackerComponent)

AliHLTTPCSliceTrackerComponent::AliHLTTPCSliceTrackerComponent()
  :
  fTracker(NULL),
  fVertex(NULL),
  fDoNonVertex(false),
  fDoPP(false),
  fDoPbPb(false),
  fMultiplicity(4000),
  fBField(0.4),
  fnonvertextracking(kFALSE),
  fmainvertextracking(kTRUE),
  fpInterMerger(NULL),
  fPhisegment(50),
  fEtasegment(100),
  fTrackletlength(3),
  fTracklength(60),
  fRowscopetracklet(6),
  fRowscopetrack(6),
  fMinPtFit(0),
  fMaxangle(0.1745),
  fGoodDist(5),
  fHitChi2Cut(100),
  fGoodHitChi2(5),
  fTrackChi2Cut(50),
  fMaxdist(50),
  fMaxphi(0.1),
  fMaxeta(0.1)
{
  // see header file for class documentation
  // or
  // refer to README to build package
  // or
  // visit http://web.ift.uib.no/~kjeks/doc/alice-hlt
  fEta[0] = 0.;
  fEta[1] = 1.1;
}

AliHLTTPCSliceTrackerComponent::~AliHLTTPCSliceTrackerComponent()
{
  // see header file for class documentation
}

// Public functions to implement AliHLTComponent's interface.
// These functions are required for the registration process

const char* AliHLTTPCSliceTrackerComponent::GetComponentID()
{
  // see header file for class documentation

  return "TPCSliceTracker";
}

void AliHLTTPCSliceTrackerComponent::GetInputDataTypes(AliHLTComponentDataTypeList& list)
{
  // see header file for class documentation
  list.clear();
  list.push_back( AliHLTTPCDefinitions::fgkClustersDataType );
  list.push_back( AliHLTTPCDefinitions::fgkVertexDataType );
}

AliHLTComponentDataType AliHLTTPCSliceTrackerComponent::GetOutputDataType()
{
  // see header file for class documentation
  return AliHLTTPCDefinitions::fgkTrackSegmentsDataType;
}

void AliHLTTPCSliceTrackerComponent::GetOutputDataSize( unsigned long& constBase, double& inputMultiplier )
{
  // see header file for class documentation
  // XXX TODO: Find more realistic values.
  constBase = 0;
  inputMultiplier = 1;
}

AliHLTComponent* AliHLTTPCSliceTrackerComponent::Spawn()
{
  // see header file for class documentation
  return new AliHLTTPCSliceTrackerComponent;
}

void AliHLTTPCSliceTrackerComponent::SetTrackerParam(Int_t phiSegments, Int_t etaSegments,
				   Int_t trackletlength, Int_t tracklength,
				   Int_t rowscopetracklet, Int_t rowscopetrack,
				   Double_t minPtFit, Double_t maxangle,
				   Double_t goodDist, Double_t hitChi2Cut,
				   Double_t goodHitChi2, Double_t trackChi2Cut,
				   Int_t maxdist, Double_t maxphi,Double_t maxeta)
{
  // see header file for class documentation
    //fTracker->SetClusterFinderParam( fXYClusterError, fZClusterError, kTRUE ); // ??
    //Set parameters input to the tracker
    //If no arguments are given, default parameters will be used
    
    fTracker->SetNSegments(phiSegments,etaSegments);
    fTracker->SetMaxDca(minPtFit);
    //   fTracker->MainVertexSettings(trackletlength,tracklength,rowscopetracklet,rowscopetrack);

    Logging( kHLTLogDebug, "HLT::TPCSliceTracker::SetTrackerParam", "Tracking", "==============================" );

    if ( fmainvertextracking == kTRUE && fnonvertextracking == kFALSE){
	fTracker->SetTrackCuts(hitChi2Cut,goodHitChi2,trackChi2Cut,maxdist,kTRUE);
	fTracker->SetTrackletCuts(maxangle,goodDist,kTRUE);

	fTracker->MainVertexSettings( trackletlength, tracklength, rowscopetracklet, rowscopetrack, maxphi, maxeta);	
	Logging( kHLTLogDebug, "HLT::TPCSliceTracker::SetTrackerParam", "Tracking", "MAINVERTEXTRACKING" );
    }
    else if ( fmainvertextracking == kTRUE && fnonvertextracking == kTRUE){
	fTracker->SetTrackCuts(hitChi2Cut,goodHitChi2,trackChi2Cut,maxdist,kTRUE);
	fTracker->SetTrackCuts(hitChi2Cut,goodHitChi2,trackChi2Cut,maxdist,kFALSE);
	fTracker->SetTrackletCuts(maxangle,goodDist,kTRUE);
	fTracker->SetTrackletCuts(maxangle,goodDist,kFALSE);

	fTracker->MainVertexSettings( trackletlength, tracklength, rowscopetracklet, rowscopetrack, maxphi, maxeta);
	fTracker->NonVertexSettings( trackletlength, tracklength, rowscopetracklet, rowscopetrack);	
	Logging( kHLTLogDebug, "HLT::TPCSliceTracker::SetTrackerParam", "Tracking", "MAINVERTEXTRACKING - NONVERTEXTRACKING" );
    }
    else if ( fmainvertextracking == kFALSE && fnonvertextracking == kTRUE){
	fTracker->SetTrackCuts(hitChi2Cut,goodHitChi2,trackChi2Cut,maxdist,kFALSE);
	fTracker->SetTrackletCuts(maxangle,goodDist,kFALSE);

	fTracker->NonVertexSettings( trackletlength, tracklength, rowscopetracklet, rowscopetrack);
	Logging( kHLTLogDebug, "HLT::TPCSliceTracker::SetTrackerParam", "Tracking", "NONVERTEXTRACKING" );
    }
    
    //fTracker->SetParamDone(true);
    /* Matthias 13.12.2006
     * the global variable AliHLTTPCS::fgDoVertexFit has never been used so far
     * and has always been kTRUE.
     * In order to remove the AliHLTTPC class (which is the old steering class for
     * HLT (TPC) tracking) from the compilation, this function can not be activated
     * again. We have to think about a more elegant way to specify the parameters
     * anyway. The following line was surely for some testing, but was never active
     * in a tested release.
     */
    //AliHLTTPC::SetVertexFit( kFALSE );
    
    fTracker->InitVolumes();
    }


void AliHLTTPCSliceTrackerComponent::SetTrackerParam( Bool_t doPP, Bool_t doPbPb, Int_t multiplicity, 
						      Double_t bField, Int_t etasegment, Double_t hitchi2cut, 
						      Int_t rowscopetracklet, Int_t rowscopetrack, 
						      Int_t trackletlength, Int_t tracklength )
{
  // see header file for class documentation
  AliHLTTPCTransform::SetBField( bField );
  Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoInit", "BField", "Setting b field to %f T\n", bField );
  
  if ( doPP )
    {
      //tracker->SetClusterFinderParam(xyerror,zerror,kTRUE); // ??
      
      SetTrackerParam( 50, 100, 3, 10,
		       2, 2,
		       0, 0.1745, 5, 100,
		       5, 50, 50, 0.1, 0.1);
    }
  else if(doPbPb)
    {
      int mults[] = { 1000, 2000, 4000, 8000 };
      int multCount = 4;
      int closestMult = 0;
      int i;
      int multDist, tmpMultDist;
      if ( multiplicity>mults[closestMult] )
	multDist = multiplicity-mults[closestMult];
      else
	multDist = mults[closestMult]-multiplicity;
      for ( i = 1; i < multCount; i++ )
	{
	  if ( multiplicity>mults[i] )
	    tmpMultDist = multiplicity-mults[i];
	  else
	    tmpMultDist = mults[i]-multiplicity;
	  if ( tmpMultDist < multDist )
	    {
	      closestMult = i;
	      multDist = tmpMultDist;
	    }
	}
      
      double bfs[] = { 0.2, 0.4 };
      int bfCount = 2;
      int closestBf = 0;
      double bfDist, tmpBFDist;
      if ( bField>bfs[closestBf] )
	bfDist = bField-bfs[closestBf];
      else
	bfDist = bfs[closestBf]-bField;
      for ( i = 1; i < bfCount; i++ )
	{
	  if ( bField>bfs[i] )
	    tmpBFDist = bField-bfs[i];
	  else
	    tmpBFDist = bfs[i]-bField;
	  if ( tmpBFDist < bfDist )
	    {
	      closestBf = i;
	      bfDist = tmpBFDist;
	    }
	}
      
      switch ( closestMult )
	{
	case 0: // 1000
	  switch ( closestBf )
	    {
	    case 0: // 0.2
	      SetTrackerParam( 50, 100, 3, 10,
			       2, 4,
			       0, 0.1745, 5, 100,
			       5, 50, 50, 0.1, 0.1);
	      break;
	    case 1: // 0.4
	      SetTrackerParam( 50, 100, 3, 10,
			       2, 4,
			       0, 0.1745, 5, 100,
			       5, 50, 50, 0.1, 0.1);
	      break;
	    }
	  break;
	case 1: // 2000
	  switch ( closestBf )
	    {
	    case 0: // 0.2
	      SetTrackerParam( 50, 100, 3, 10,
			       2, 4,
			       0, 0.1745, 5, 30,
			       5, 20, 50, 0.1, 0.1);
	      break;
	    case 1: // 0.4
	      SetTrackerParam( 50, 100, 3, 10,
			       2, 5,
			       0, 0.1745, 5, 30,
			       5, 20, 50, 0.1, 0.1);
	      break;
	    }
	  break;
	case 2: // 4000
	  switch ( closestBf )
	    {
	    case 0: // 0.2
	      SetTrackerParam( 50, 100, 3, 10,
			       2 , 10,
			       0, 0.1745, 5, 20,
			       5, 10 , 50, 0.1, 0.1);
	      break;
	    case 1: // 0.4
	      SetTrackerParam( 50, 100, 3, 10,
			       2, 10,
			       0, 0.1745, 5, 20,
			       5, 10, 50, 0.1, 0.1);
	      break;
	    }
	  break;
	case 3: // 8000
	  switch ( closestBf )
	    {
	    case 0: // 0.2
	      SetTrackerParam( 50, 100, 3, 10,
			       3, 15,
			       0, 0.1745, 5, 10,
			       5, 5, 50, 0.1, 0.1);
	      break;
	    case 1: // 0.4
	      SetTrackerParam( 50, 100, 3, 10,
			       2, 15,
			       0, 0.1745, 5, 15,
			       5, 5, 50, 0.1, 0.1);
	      break;
	    }
	  break;
	}
      //	Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoInit", "BField", "Setting b field to %f\n", bfs[closestBf] );
      //	AliHLTTPCTransform::SetBField( bfs[closestBf] );
      //	AliHLTTPCTransform::SetBField( bField );
      //	Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoInit", "BField", "Setting b field to %f\n", bField );
    }
  else
    {
      SetTrackerParam( 50, etasegment, trackletlength, tracklength,
		       rowscopetracklet, rowscopetrack,
		       0, 0.1745, 5, hitchi2cut,
		       5, 50, 50, 0.1, 0.1);
    }
}
	
int AliHLTTPCSliceTrackerComponent::DoInit( int argc, const char** argv )
    {
  // see header file for class documentation
    Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoInit", "DoInit", "DoInit()" );

    if ( fTracker || fVertex )
	return EINPROGRESS;
    fTracker = new AliHLTTPCConfMapper();
    fVertex = new AliHLTTPCVertex();
    fEta[0] = 0.;
    fEta[1] = 1.1;
    fDoNonVertex = false;
    Bool_t bDoMerger=kTRUE;
    fMultiplicity = 4000;
    fBField = 0.5;
    fDoPP = false;
    fDoPbPb = false;
    fPhisegment=50;
    fEtasegment=100;
    fTrackletlength=3;
    fTracklength=60;
    fRowscopetracklet=6;
    fRowscopetrack=6;
    fMinPtFit=0;
    fMaxangle=0.1745;
    fGoodDist=5;
    fHitChi2Cut=100;
    fGoodHitChi2=5;
    fTrackChi2Cut=50;
    fMaxdist=50;
    fMaxphi=0.1;
    fMaxeta=0.1;

    int i = 0;
    char* cpErr;
    while ( i < argc )
	{
	if ( !strcmp( argv[i], "-disable-merger" ) ){
	    bDoMerger = kFALSE;
	    i++;
	    continue;	    
	}

	if ( !strcmp( argv[i], "-pp-run" ) )
	    {
	    fDoPP = true;
	    i++;
	    continue;
	    }
	if ( !strcmp( argv[i], "-PbPb-run" ) )
	  {
	    fDoPbPb = true;
	    i++;
	    continue;
	  }
	if ( !strcmp( argv[i], "-multiplicity" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing multiplicity", "Missing event multiplicity specifier." );
		return ENOTSUP;
		}
	    fMultiplicity = strtoul( argv[i+1], &cpErr, 0 );
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing multiplicity", "Cannot convert event multiplicity specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }
	if ( !strcmp( argv[i], "-bfield" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing B-field", "Missing B-field specifier." );
		return ENOTSUP;
		}
	    fBField = strtod( argv[i+1], &cpErr );
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing B-field", "Cannot convert B-field specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }

	if ( !strcmp( argv[i], "-nonvertextracking" ) ){
	  fnonvertextracking = kTRUE;
	  i++;
	  continue;	    
	}
	
	if ( !strcmp( argv[i], "-mainvertextrackingoff" ) ){	
	  fmainvertextracking = kFALSE;
	  i++;
	  continue;	    
	}
	
	if ( !strcmp( argv[i], "-etarange" ) ){	
	  if ( argc <= i+1 ){
	    Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing Eta range", "Missing Eta-range specifiers." );
	    return ENOTSUP;
	  }
	  fEta[1] = strtod( argv[i+1], &cpErr );
	  if ( *cpErr ){
	    Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing Eta range", "Cannot convert Eta-range specifier '%s'.", argv[i+1] );
	    return EINVAL;
	  }
	  
	  i += 2;
	  continue;
	}
	if ( !strcmp( argv[i], "-etasegment" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing etasegment", "Missing etasegment specifier." );
		return ENOTSUP;
		}
	    fEtasegment = strtol( argv[i+1], &cpErr,10);
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing etasegment", "Cannot convert etasegment specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }
	
	if ( !strcmp( argv[i], "-chi2cut" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing chi2cut", "Missing chi2cut specifier." );
		return ENOTSUP;
		}
	    fHitChi2Cut = strtod( argv[i+1], &cpErr );
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing chi2cut", "Cannot convert chi2cut specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }
	if ( !strcmp( argv[i], "-rowscopetracklet" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing RowScopeTracklet", "Missing etasegment specifier." );
		return ENOTSUP;
		}
	    fRowscopetracklet = strtol( argv[i+1], &cpErr,10 );
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing RowScopeTracklet", "Cannot convert etasegment specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }

	if ( !strcmp( argv[i], "-rowscopetrack" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing RowScopeTrack", "Missing etasegment specifier." );
		return ENOTSUP;
		}
	    fRowscopetrack = strtol( argv[i+1], &cpErr,10 );
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing RowScopeTrack", "Cannot convert etasegment specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }

	if ( !strcmp( argv[i], "-trackletlength" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing TrackletLength", "Missing etasegment specifier." );
		return ENOTSUP;
		}
	    fTrackletlength = strtol( argv[i+1], &cpErr,10 );
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing TrackletLength", "Cannot convert etasegment specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }

	if ( !strcmp( argv[i], "-tracklength" ) )
	    {
	    if ( argc <= i+1 )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing TrackLength", "Missing etasegment specifier." );
		return ENOTSUP;
		}
	    fTracklength = strtol( argv[i+1], &cpErr,10 );
	    if ( *cpErr )
		{
		Logging( kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Missing TrackLength", "Cannot convert etasegment specifier '%s'.", argv[i+1] );
		return EINVAL;
		}
	    i += 2;
	    continue;
	    }

	Logging(kHLTLogError, "HLT::TPCSliceTracker::DoInit", "Unknown Option", "Unknown option '%s'", argv[i] );
	return EINVAL;
	}
    if (fBField == 0.){
      // parameter for B=0 T 
      fDoPP = kTRUE;
      fnonvertextracking = kTRUE;
      fmainvertextracking = kFALSE;
    }
        
    if (bDoMerger)
      fpInterMerger = new AliHLTTPCInterMerger();

    SetTrackerParam(fDoPP,fDoPbPb,fMultiplicity,fBField,fEtasegment,fHitChi2Cut,fRowscopetracklet,fRowscopetrack,fTrackletlength,fTracklength);
    return 0;
    }

int AliHLTTPCSliceTrackerComponent::DoDeinit()
{
  // see header file for class documentation
  if ( fTracker )
    delete fTracker;
  fTracker = NULL;
  if ( fVertex )
    delete fVertex;
  fVertex = NULL;
  if (fpInterMerger) {
    delete fpInterMerger;
  }
  fpInterMerger=NULL;
  return 0;
}

int AliHLTTPCSliceTrackerComponent::DoEvent( const AliHLTComponentEventData& evtData, const AliHLTComponentBlockData* blocks, 
					      AliHLTComponentTriggerData& /*trigData*/, AliHLTUInt8_t* outputPtr, 
					      AliHLTUInt32_t& size, AliHLTComponentBlockDataList& outputBlocks )
    {
  // see header file for class documentation
    Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoEvent", "DoEvent", "DoEvent()" );
    if ( evtData.fBlockCnt<=0 )
      {
	Logging( kHLTLogWarning, "HLT::TPCSliceTracker::DoEvent", "DoEvent", "no blocks in event" );
	return 0;
      }
    const AliHLTComponentBlockData* iter = NULL;
    unsigned long ndx;
    AliHLTTPCClusterData* inPtrSP;
    AliHLTTPCVertexData* inPtrV = NULL;
    const AliHLTComponentBlockData* vertexIter=NULL;
    AliHLTTPCTrackletData* outPtr;
    AliHLTUInt8_t* outBPtr;
    AliHLTUInt32_t vSize = 0;
    UInt_t offset=0, mysize, tSize = 0;
    outBPtr = outputPtr;
    Int_t slice=-1, patch=-1, row[2];
    Int_t minPatch=INT_MAX, maxPatch = 0;
    offset = 0;
    std::vector<Int_t> slices;
    std::vector<Int_t>::iterator slIter, slEnd;
    std::vector<unsigned> sliceCnts;
    std::vector<unsigned>::iterator slCntIter;
    Int_t vertexSlice=-1;

    // Find min/max rows used in total and find and read out vertex if it is present
    // also determine correct slice number, if multiple slice numbers are present in event
    // (which should not happen in the first place) we use the one that occurs the most times
    row[0] = 0;
    row[1] = 0;
    bool found;
    for ( ndx = 0; ndx < evtData.fBlockCnt; ndx++ )
	{
	iter = blocks+ndx;

	if(iter->fDataType!=AliHLTTPCDefinitions::fgkClustersDataType){
	  HLTDebug("Data block type is not of type AliHLTTPCDefinitions::fgkClustersDataType");
	  continue;
	}

	slice = AliHLTTPCDefinitions::GetMinSliceNr( *iter );
	found = false;
	slIter = slices.begin();
	slEnd = slices.end();
	slCntIter = sliceCnts.begin();
	while ( slIter != slEnd )
	    {
	    if ( *slIter == slice )
		{
		found = true;
		break;
		}
	    slIter++;
	    slCntIter++;
	    }
	if ( !found )
	    {
	    slices.insert( slices.end(), slice );
	    sliceCnts.insert( sliceCnts.end(), 1 );
	    }
	else
	    *slCntIter++;

	if ( iter->fDataType == AliHLTTPCDefinitions::fgkVertexDataType )
	    {
	    inPtrV = (AliHLTTPCVertexData*)(iter->fPtr);
	    vertexIter = iter;
	    vSize = iter->fSize;
	    fVertex->Read( inPtrV );
	    vertexSlice = slice;
	    }
	if ( iter->fDataType == AliHLTTPCDefinitions::fgkClustersDataType )
	    {
	    patch = AliHLTTPCDefinitions::GetMinPatchNr( *iter );
	    if ( minPatch>patch )
		{
		minPatch = patch;
		row[0] = AliHLTTPCTransform::GetFirstRow( patch );
		}
	    if ( maxPatch<patch )
		{
		maxPatch = patch;
		row[1] = AliHLTTPCTransform::GetLastRow( patch );
		}
	    }
	}

    // Determine slice number to really use.
    if ( slices.size()>1 )
	{
	Logging( kHLTLogError, "HLT::TPCSliceTracker::DoEvent", "Multiple slices found in event",
		 "Multiple slice numbers found in event 0x%08lX (%lu). Determining maximum occuring slice number...",
		 evtData.fEventID, evtData.fEventID );
	unsigned maxCntSlice=0;
	slIter = slices.begin();
	slEnd = slices.end();
	slCntIter = sliceCnts.begin();
	while ( slIter != slEnd )
	    {
	    Logging( kHLTLogError, "HLT::TPCSliceTracker::DoEvent", "Multiple slices found in event",
		     "Slice %lu found %lu times.", *slIter, *slCntIter );
	    if ( maxCntSlice<*slCntIter )
		{
		maxCntSlice = *slCntIter;
		slice = *slIter;
		}
	    slIter++;
	    slCntIter++;
	    }
	Logging( kHLTLogError, "HLT::TPCSliceTracker::DoEvent", "Multiple slices found in event",
		 "Using slice %lu.", slice );
	}
    else if ( slices.size()>0 )
      {
	slice = *(slices.begin());
      }
    else
      {
	slice = -1;
      }
    
    if ( vertexSlice != slice )
	{
	// multiple vertex blocks in event and we used the wrong one...
	found = false;
	for ( ndx = 0; ndx < evtData.fBlockCnt; ndx++ )
	    {
	    iter = blocks+ndx;
	    if ( iter->fDataType == AliHLTTPCDefinitions::fgkVertexDataType && slice==AliHLTTPCDefinitions::GetMinSliceNr( *iter ) )
		{
		inPtrV = (AliHLTTPCVertexData*)(iter->fPtr);
		vertexIter = iter;
		vSize = iter->fSize;
		fVertex->Read( inPtrV );
		break;
		}
	    }
	}

    fTracker->InitSector( slice, row, fEta );
    fTracker->SetVertex(fVertex);
    mysize = 0;
    // read in all hits
    std::vector<unsigned long> patchIndices;
    std::vector<unsigned long>::iterator pIter, pEnd;
    for ( ndx = 0; ndx < evtData.fBlockCnt; ndx++ )
	{
	iter = blocks+ndx;

	if ( iter->fDataType == AliHLTTPCDefinitions::fgkClustersDataType && slice==AliHLTTPCDefinitions::GetMinSliceNr( *iter ) )
	    {
	    patch = AliHLTTPCDefinitions::GetMinPatchNr( *iter );
	    pIter = patchIndices.begin();
	    pEnd = patchIndices.end();
	    while ( pIter!=pEnd && AliHLTTPCDefinitions::GetMinSliceNr( blocks[*pIter] ) < patch )
		pIter++;
	    patchIndices.insert( pIter, ndx );
	    }
	}
    pIter = patchIndices.begin();
    pEnd = patchIndices.end();
    while ( pIter!=pEnd )
	{
	ndx = *pIter;
	iter = blocks+ndx;

	patch = AliHLTTPCDefinitions::GetMinPatchNr( *iter );
	inPtrSP = (AliHLTTPCClusterData*)(iter->fPtr);
	    
	Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoEvent", "Reading hits",
		 "Reading hits for slice %d - patch %d", slice, patch );
	fTracker->ReadHits( inPtrSP->fSpacePointCnt, inPtrSP->fSpacePoints );
	pIter++;
	}

    outPtr = (AliHLTTPCTrackletData*)(outBPtr);

    if ( fmainvertextracking == kTRUE && fnonvertextracking == kFALSE){	
      Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoEvent", "Tracking", " ---MAINVERTEXTRACKING---");
      fTracker->MainVertexTrackingA();
      fTracker->MainVertexTrackingB();
      fTracker->FillTracks();
    }
    else if ( fmainvertextracking == kTRUE && fnonvertextracking == kTRUE){	
      Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoEvent", "Tracking", " ---MAINVERTEXTRACKING---");
      fTracker->MainVertexTrackingA();
      fTracker->MainVertexTrackingB();
      fTracker->FillTracks();	
      Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoEvent", "Tracking", " ---NONVERTEXTRACKING---");
      fTracker->NonVertexTracking();
    }
    else if ( fmainvertextracking == kFALSE && fnonvertextracking == kTRUE){
      Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoEvent", "Tracking", " ---NONVERTEXTRACKING---");
      fTracker->NonVertexTracking();	
      fTracker->FillTracks();	
    }

    UInt_t ntracks0 =0;
    if(fpInterMerger){
      AliHLTTPCMemHandler memory;
      AliHLTTPCTrackSegmentData *trackdata0  = 
	(AliHLTTPCTrackSegmentData *) memory.Allocate(fTracker->GetTracks());
      memory.TrackArray2Memory(ntracks0,trackdata0,fTracker->GetTracks());
      fpInterMerger->Reset();
      fpInterMerger->Init(row,patch);
      fpInterMerger->FillTracks(ntracks0,trackdata0);
      fpInterMerger->Merge();
    } 
    ntracks0=0;
    AliHLTTPCTrackArray* pArray=fTracker->GetTracks();
    mysize = pArray->WriteTracks( ntracks0, outPtr->fTracklets );
    outPtr->fTrackletCnt = ntracks0;

    Logging( kHLTLogDebug, "HLT::TPCSliceTracker::DoEvent", "Tracks",
	     "Input: Number of tracks: %lu Slice/MinPatch/MaxPatch/RowMin/RowMax: %lu/%lu/%lu/%lu/%lu.", 
	     ntracks0, slice, minPatch, maxPatch, row[0], row[1] );

    fTracker->Reset();

    tSize += mysize+sizeof(AliHLTTPCTrackletData);
    outBPtr += mysize+sizeof(AliHLTTPCTrackletData);
    
    AliHLTComponentBlockData bd;
    FillBlockData( bd );
    bd.fOffset = offset;
    bd.fSize = tSize;
    bd.fSpecification = AliHLTTPCDefinitions::EncodeDataSpecification( slice, slice, minPatch, maxPatch );
    outputBlocks.push_back( bd );

#ifdef FORWARD_VERTEX_BLOCK
    if ( vertexIter )
	{
	// Copy the descriptor block for the vertex information.
	bd = *vertexIter;
	outputBlocks.push_back( bd );
	}
#endif // FORWARD_VERTEX_BLOCK

    size = tSize;
    return 0;
    }

void AliHLTTPCSliceTrackerComponent::SetTrackerParam1()
{
  // see header file for class documentation
  SetTrackerParam( 10, 20, 5, 10, 2,2,
		   0, 1.31, 5, 100,
		   50, 100, 50, 0.1, 0.1);
}

	
