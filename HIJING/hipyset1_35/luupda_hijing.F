* $Id$
    
C*********************************************************************  
    
      SUBROUTINE LUUPDA_HIJING(MUPDA,LFN)  
    
C...Purpose: to facilitate the updating of particle and decay data. 
#include "ludat1_hijing.inc"
#include "ludat2_hijing.inc"
#include "ludat3_hijing.inc"
#include "ludat4_hijing.inc"
      CHARACTER CHINL*80,CHKC*4,CHVAR(19)*9,CHLIN*72,   
     &CHBLK(20)*72,CHOLD*12,CHTMP*12,CHNEW*12,CHCOM*12  
      DATA CHVAR/ 'KCHG(I,1)','KCHG(I,2)','KCHG(I,3)','PMAS(I,1)',  
     &'PMAS(I,2)','PMAS(I,3)','PMAS(I,4)','MDCY(I,1)','MDCY(I,2)',  
     &'MDCY(I,3)','MDME(I,1)','MDME(I,2)','BRAT(I)  ','KFDP(I,1)',  
     &'KFDP(I,2)','KFDP(I,3)','KFDP(I,4)','KFDP(I,5)','CHAF(I)  '/  
    
C...Write information on file for editing.  
      IF(MSTU(12).GE.1) CALL LULIST_HIJING(0)  
      IF(MUPDA.EQ.1) THEN   
        DO 110 KC=1,MSTU(6) 
        WRITE(LFN,1000) KC,CHAF(KC),(KCHG(KC,J1),J1=1,3),   
     &  (PMAS(KC,J2),J2=1,4),MDCY(KC,1) 
        DO 100 IDC=MDCY(KC,2),MDCY(KC,2)+MDCY(KC,3)-1   
  100   WRITE(LFN,1100) MDME(IDC,1),MDME(IDC,2),BRAT(IDC),  
     &  (KFDP(IDC,J),J=1,5) 
  110   CONTINUE    
    
C...Reset variables and read information from edited file.  
      ELSEIF(MUPDA.EQ.2) THEN   
        DO 120 I=1,MSTU(7)  
        MDME(I,1)=1 
        MDME(I,2)=0 
        BRAT(I)=0.  
        DO 120 J=1,5    
  120   KFDP(I,J)=0 
        KC=0    
        IDC=0   
        NDC=0   
  130   READ(LFN,1200,END=140) CHINL    
        IF(CHINL(2:5).NE.'    ') THEN   
          CHKC=CHINL(2:5)   
          IF(KC.NE.0) THEN  
            MDCY(KC,2)=0    
            IF(NDC.NE.0) MDCY(KC,2)=IDC+1-NDC   
            MDCY(KC,3)=NDC  
          ENDIF 
          READ(CHKC,1300) KC    
          IF(KC.LE.0.OR.KC.GT.MSTU(6)) CALL LUERRM_HIJING(27,  
     &    '(LUUPDA_HIJING:) Read KC code illegal, KC ='//CHKC) 
          READ(CHINL,1000) KCR,CHAF(KC),(KCHG(KC,J1),J1=1,3),   
     &    (PMAS(KC,J2),J2=1,4),MDCY(KC,1)   
          NDC=0 
        ELSE    
          IDC=IDC+1 
          NDC=NDC+1 
          IF(IDC.GE.MSTU(7)) CALL LUERRM_HIJING(27,    
     &    '(LUUPDA_HIJING:) Decay data arrays full by KC ='//CHKC) 
          READ(CHINL,1100) MDME(IDC,1),MDME(IDC,2),BRAT(IDC),   
     &    (KFDP(IDC,J),J=1,5)   
        ENDIF   
        GOTO 130    
  140   MDCY(KC,2)=0    
        IF(NDC.NE.0) MDCY(KC,2)=IDC+1-NDC   
        MDCY(KC,3)=NDC  
    
C...Perform possible tests that new information is consistent.  
        MSTJ24=MSTJ(24) 
        MSTJ(24)=0  
        DO 170 KC=1,MSTU(6) 
        WRITE(CHKC,1300) KC 
        IF(MIN(PMAS(KC,1),PMAS(KC,2),PMAS(KC,3),PMAS(KC,1)-PMAS(KC,3),  
     &       PMAS(KC,4)).LT.0..OR.MDCY(KC,3).LT.0) CALL LUERRM_HIJING(17
     $       ,'(LUUPDA_HIJING:) Mass/width/life/(# channels) wrong '/
     $       /'for KC ='//CHKC)  
        BRSUM=0.    
        DO 160 IDC=MDCY(KC,2),MDCY(KC,2)+MDCY(KC,3)-1   
        IF(MDME(IDC,2).GT.80) GOTO 160  
        KQ=KCHG(KC,1)   
        PMS=PMAS(KC,1)-PMAS(KC,3)-PARJ(64)  
        MERR=0  
        DO 150 J=1,5    
        KP=KFDP(IDC,J)  
        IF(KP.EQ.0.OR.KP.EQ.81.OR.IABS(KP).EQ.82) THEN  
        ELSEIF(LUCOMP_HIJING(KP).EQ.0) THEN    
          MERR=3    
        ELSE    
          KQ=KQ-LUCHGE_HIJING(KP)  
          PMS=PMS-ULMASS_HIJING(KP)    
        ENDIF   
  150   CONTINUE    
        IF(KQ.NE.0) MERR=MAX(2,MERR)    
        IF(KFDP(IDC,2).NE.0.AND.(KC.LE.20.OR.KC.GT.40).AND. 
     &  (KC.LE.80.OR.KC.GT.100).AND.MDME(IDC,2).NE.34.AND.  
     &  MDME(IDC,2).NE.61.AND.PMS.LT.0.) MERR=MAX(1,MERR)   
        IF(MERR.EQ.3) CALL LUERRM_HIJING(17,   
     &  '(LUUPDA_HIJING:) Unknown particle code in decay of KC ='//CHKC)   
        IF(MERR.EQ.2) CALL LUERRM_HIJING(17,   
     &  '(LUUPDA_HIJING:) Charge not conserved in decay of KC ='//CHKC)    
        IF(MERR.EQ.1) CALL LUERRM_HIJING(7,    
     &  '(LUUPDA_HIJING:) Kinematically unallowed decay of KC ='//CHKC)    
        BRSUM=BRSUM+BRAT(IDC)   
  160   CONTINUE    
        WRITE(CHTMP,1500) BRSUM 
        IF(ABS(BRSUM).GT.0.0005.AND.ABS(BRSUM-1.).GT.0.0005) CALL   
     &       LUERRM_HIJING(7
     $       ,'(LUUPDA_HIJING:) Sum of branching ratios is '//CHTMP(5:12
     $       )//' for KC ='//CHKC)  
  170   CONTINUE    
        MSTJ(24)=MSTJ24 
    
C...Initialize writing of DATA statements for inclusion in program. 
      ELSEIF(MUPDA.EQ.3) THEN   
        DO 240 IVAR=1,19    
        NDIM=MSTU(6)    
        IF(IVAR.GE.11.AND.IVAR.LE.18) NDIM=MSTU(7)  
        NLIN=1  
        CHLIN=' '   
        CHLIN(7:35)='DATA ('//CHVAR(IVAR)//',I=   1,    )/' 
        LLIN=35 
        CHOLD='START'   
    
C...Loop through variables for conversion to characters.    
        DO 220 IDIM=1,NDIM  
        IF(IVAR.EQ.1) WRITE(CHTMP,1400) KCHG(IDIM,1)    
        IF(IVAR.EQ.2) WRITE(CHTMP,1400) KCHG(IDIM,2)    
        IF(IVAR.EQ.3) WRITE(CHTMP,1400) KCHG(IDIM,3)    
        IF(IVAR.EQ.4) WRITE(CHTMP,1500) PMAS(IDIM,1)    
        IF(IVAR.EQ.5) WRITE(CHTMP,1500) PMAS(IDIM,2)    
        IF(IVAR.EQ.6) WRITE(CHTMP,1500) PMAS(IDIM,3)    
        IF(IVAR.EQ.7) WRITE(CHTMP,1500) PMAS(IDIM,4)    
        IF(IVAR.EQ.8) WRITE(CHTMP,1400) MDCY(IDIM,1)    
        IF(IVAR.EQ.9) WRITE(CHTMP,1400) MDCY(IDIM,2)    
        IF(IVAR.EQ.10) WRITE(CHTMP,1400) MDCY(IDIM,3)   
        IF(IVAR.EQ.11) WRITE(CHTMP,1400) MDME(IDIM,1)   
        IF(IVAR.EQ.12) WRITE(CHTMP,1400) MDME(IDIM,2)   
        IF(IVAR.EQ.13) WRITE(CHTMP,1500) BRAT(IDIM) 
        IF(IVAR.EQ.14) WRITE(CHTMP,1400) KFDP(IDIM,1)   
        IF(IVAR.EQ.15) WRITE(CHTMP,1400) KFDP(IDIM,2)   
        IF(IVAR.EQ.16) WRITE(CHTMP,1400) KFDP(IDIM,3)   
        IF(IVAR.EQ.17) WRITE(CHTMP,1400) KFDP(IDIM,4)   
        IF(IVAR.EQ.18) WRITE(CHTMP,1400) KFDP(IDIM,5)   
        IF(IVAR.EQ.19) CHTMP=CHAF(IDIM) 
    
C...Length of variable, trailing decimal zeros, quotation marks.    
        LLOW=1  
        LHIG=1  
        DO 180 LL=1,12  
        IF(CHTMP(13-LL:13-LL).NE.' ') LLOW=13-LL    
  180   IF(CHTMP(LL:LL).NE.' ') LHIG=LL 
        CHNEW=CHTMP(LLOW:LHIG)//' ' 
        LNEW=1+LHIG-LLOW    
        IF((IVAR.GE.4.AND.IVAR.LE.7).OR.IVAR.EQ.13) THEN    
          LNEW=LNEW+1   
  190     LNEW=LNEW-1   
          IF(CHNEW(LNEW:LNEW).EQ.'0') GOTO 190  
          IF(LNEW.EQ.1) CHNEW(1:2)='0.' 
          IF(LNEW.EQ.1) LNEW=2  
        ELSEIF(IVAR.EQ.19) THEN 
          DO 200 LL=LNEW,1,-1   
          IF(CHNEW(LL:LL).EQ.'''') THEN 
            CHTMP=CHNEW 
            CHNEW=CHTMP(1:LL)//''''//CHTMP(LL+1:11) 
            LNEW=LNEW+1 
          ENDIF 
  200     CONTINUE  
          CHTMP=CHNEW   
          CHNEW(1:LNEW+2)=''''//CHTMP(1:LNEW)//'''' 
          LNEW=LNEW+2   
        ENDIF   
    
C...Form composite character string, often including repetition counter.    
        IF(CHNEW.NE.CHOLD) THEN 
          NRPT=1    
          CHOLD=CHNEW   
          CHCOM=CHNEW   
          LCOM=LNEW 
        ELSE    
          LRPT=LNEW+1   
          IF(NRPT.GE.2) LRPT=LNEW+3 
          IF(NRPT.GE.10) LRPT=LNEW+4    
          IF(NRPT.GE.100) LRPT=LNEW+5   
          IF(NRPT.GE.1000) LRPT=LNEW+6  
          LLIN=LLIN-LRPT    
          NRPT=NRPT+1   
          WRITE(CHTMP,1400) NRPT    
          LRPT=1    
          IF(NRPT.GE.10) LRPT=2 
          IF(NRPT.GE.100) LRPT=3    
          IF(NRPT.GE.1000) LRPT=4   
          CHCOM(1:LRPT+1+LNEW)=CHTMP(13-LRPT:12)//'*'//CHNEW(1:LNEW)    
          LCOM=LRPT+1+LNEW  
        ENDIF   
    
C...Add characters to end of line, to new line (after storing old line),    
C...or to new block of lines (after writing old block). 
        IF(LLIN+LCOM.LE.70) THEN    
          CHLIN(LLIN+1:LLIN+LCOM+1)=CHCOM(1:LCOM)//','  
          LLIN=LLIN+LCOM+1  
        ELSEIF(NLIN.LE.19) THEN 
          CHLIN(LLIN+1:72)=' '  
          CHBLK(NLIN)=CHLIN 
          NLIN=NLIN+1   
          CHLIN(6:6+LCOM+1)='&'//CHCOM(1:LCOM)//',' 
          LLIN=6+LCOM+1 
        ELSE    
          CHLIN(LLIN:72)='/'//' '   
          CHBLK(NLIN)=CHLIN 
          WRITE(CHTMP,1400) IDIM-NRPT   
          CHBLK(1)(30:33)=CHTMP(9:12)   
          DO 210 ILIN=1,NLIN    
  210     WRITE(LFN,1600) CHBLK(ILIN)   
          NLIN=1    
          CHLIN=' ' 
          CHLIN(7:35+LCOM+1)='DATA ('//CHVAR(IVAR)//',I=    ,    )/'//  
     &    CHCOM(1:LCOM)//','    
          WRITE(CHTMP,1400) IDIM-NRPT+1 
          CHLIN(25:28)=CHTMP(9:12)  
          LLIN=35+LCOM+1    
        ENDIF   
  220   CONTINUE    
    
C...Write final block of lines. 
        CHLIN(LLIN:72)='/'//' ' 
        CHBLK(NLIN)=CHLIN   
        WRITE(CHTMP,1400) NDIM  
        CHBLK(1)(30:33)=CHTMP(9:12) 
        DO 230 ILIN=1,NLIN  
  230   WRITE(LFN,1600) CHBLK(ILIN) 
  240   CONTINUE    
      ENDIF 
    
C...Formats for reading and writing particle data.  
 1000 FORMAT(1X,I4,2X,A8,3I3,3F12.5,2X,F12.5,I3)    
 1100 FORMAT(5X,2I5,F12.5,5I8)  
 1200 FORMAT(A80)   
 1300 FORMAT(I4)    
 1400 FORMAT(I12)   
 1500 FORMAT(F12.5) 
 1600 FORMAT(A72)   
    
      RETURN    
      END   
