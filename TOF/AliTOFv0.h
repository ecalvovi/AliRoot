//_________________________________________________________________________
// Implementation version v0 of TOF Manager class
// NO HOLES i.e. FULL COVERAGE
// NB: for this version no hits are defined
//*-- 
//*-- Authors: Pierella, Seganti, Vicinanza (Bologna and Salerno University)

#ifndef ALITOFv0_H
#define ALITOFv0_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include "AliTOF.h"
#include "AliHit.h"
 
 
class AliTOFv0 : public AliTOF {
 
public:
  AliTOFv0();
  AliTOFv0(const char *name, const char *title);
  virtual ~AliTOFv0(void) ;
  virtual void   BuildGeometry();
  virtual void   CreateGeometry();
  virtual void   CreateMaterials();
  virtual void   Init();
  virtual Int_t  IsVersion() const {return 0;}
  virtual void   TOFpc(Float_t xtof,Float_t ytof,Float_t zlenC,Float_t zlenB,
                       Float_t zlenA,Float_t ztof0);
  virtual void   StepManager();
  virtual void   DrawModule();

private:
  Int_t fIdFTOA; // FTOA volume identifier (outer plate A)
  Int_t fIdFTOB; // FTOB volume identifier (outer plate B)
  Int_t fIdFTOC; // FTOC volume identifier (outer plate C)
  Int_t fIdFLTA; // FLTA volume identifier (inner plate A)
  Int_t fIdFLTB; // FLTB volume identifier (inner plate B)
  Int_t fIdFLTC; // FLTC volume identifier (inner plate C)
  
   ClassDef(AliTOFv0,1)  //Time Of Flight version 0
};
 
#endif /*  ALITOFv0_H */
