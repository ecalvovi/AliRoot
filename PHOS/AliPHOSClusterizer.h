#ifndef ALIPHOSCLUSTERIZER_H
#define ALIPHOSCLUSTERIZER_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */
                            
/* $Id$ */

//_________________________________________________________________________
//  Base class for the clusterization algorithm (pure abstract)
//*-- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (SUBATECH & Kurchatov Institute)
// --- ROOT system ---

#include "TTask.h" 

// --- Standard library ---
#include <iostream> 

// --- AliRoot header files ---

class AliRunLoader;
//#include "AliPHOSDigit.h"



class AliPHOSClusterizer : public TTask {

public:

  AliPHOSClusterizer() ;        // default ctor
  AliPHOSClusterizer(const char * headerFile, const char * name);
  virtual ~AliPHOSClusterizer() ; // dtor

  virtual Float_t GetEmcClusteringThreshold()const {cout << "Not Defined" << endl ; return 0. ; }  
  virtual Float_t GetEmcLocalMaxCut()const {cout << "Not Defined" << endl ; return 0. ; } 
  virtual Float_t GetEmcLogWeight()const {cout << "Not Defined" << endl ; return 0. ; } 
  virtual Float_t GetEmcTimeGate() const {cout << "Not Defined" << endl ; return 0. ; }  ;
  virtual Float_t GetCpvClusteringThreshold()const {cout << "Not Defined" << endl ; return 0. ; } 
  virtual Float_t GetCpvLocalMaxCut()const {cout << "Not Defined" << endl ; return 0. ; } 
  virtual Float_t GetCpvLogWeight()const {cout << "Not Defined" << endl ; return 0. ; } 
  virtual char *  GetRecPointsBranch() const {cout << "Not Defined" << endl ; return 0 ; }  ;
  virtual const Int_t GetRecPointsInRun()  const {cout << "Not Defined" << endl ; return 0 ; } 
  virtual char *  GetDigitsBranch() const{cout << "Not Defined" << endl ; return 0 ; }   ;

  virtual void MakeClusters() {cout << "Not Defined" << endl ; } 
  virtual void Print(Option_t * option)const {cout << "Not Defined" << endl ; } 

  virtual void SetEmcClusteringThreshold(Float_t cluth) {cout << "Not Defined" << endl ; } 
  virtual void SetEmcLocalMaxCut(Float_t cut) {cout << "Not Defined" << endl ; } 
  virtual void SetEmcLogWeight(Float_t w) {cout << "Not Defined" << endl ; } 
  virtual void SetEmcTimeGate(Float_t gate) {cout << "Not Defined" << endl ; } 
  virtual void SetCpvClusteringThreshold(Float_t cluth) {cout << "Not Defined" << endl ; } 
  virtual void SetCpvLocalMaxCut(Float_t cut) {cout << "Not Defined" << endl ; } 
  virtual void SetCpvLogWeight(Float_t w) {cout << "Not Defined" << endl ;  } 
  virtual void SetDigitsBranch(const char * title) {cout << "Not Defined" << endl ; }  
  virtual void SetRecPointsBranch(const char *title) {cout << "Not Defined" << endl ; } 
  virtual void SetUnfolding(Bool_t toUnfold ){cout << "Not Defined" << endl ;}  
  virtual const char * Version() const {cout << "Not Defined" << endl ; return 0 ; }  
protected:
  AliRunLoader* fRunLoader;//!

  ClassDef(AliPHOSClusterizer,2)  // Clusterization algorithm class 

} ;

#endif // AliPHOSCLUSTERIZER_H
