ObjSuf		= o
SrcSuf		= cxx
HeadSuf		= h


ALIHLT_LIBDIR = $(ALICE_ROOT)/HLT/build/lib


#ROOTLIBS	= `root-config --libs --glibs` -lGed -lFTGL -lRGL -lEve -lEG -lGeom -lTreePlayer -lProofPlayer -lProof
#ROOTLIBS	= `root-config --libs --glibs` -lGed -lEG -lGeom -lTreePlayer -lProofPlayer -lProof -lXMLParser
ROOTLIBS	= `root-config --libs --glibs` -lGed -lEG -lGeom -lTreePlayer -lProofPlayer -lProof -lXMLParser -lEve -lRGL

#ALIROOTLIBS	= -L$(ALICE_ROOT)/lib/tgt_$(ALICE_TARGET)  -lHLTbase -lAliHLTHOMER  -lAliHLTEMCAL  -lRAWDatarec \
#		   -lRAWDatabase -lESD -lSTEER -lSTEERBase -lCDB -lMinuit -lVMC -lEMCALbase -lEMCALshuttle -lEMCALUtils \
#                   -lEveBase	

ALIROOTLIBS	= -L$(ALICE_ROOT)/lib/tgt_$(ALICE_TARGET)  -lHLTbase -lAliHLTHOMER  -lAliHLTEMCAL  -lRAWDatarec \
		   -lRAWDatabase -lESD -lSTEER -lSTEERBase -lCDB -lMinuit -lVMC -lEMCALbase  -lEMCALUtils \
                   -lEveBase	


#ALIHLTLIBS	= -L$(ALIHLT_LIBDIR)  -lAliHLTUtil  -lHLTbase -lAliHLTHOMER -lAliHLTEMCAL
ALIHLTLIBS	= -L$(ALIHLT_LIBDIR)   -lHLTbase -lAliHLTHOMER -lAliHLTEMCAL -lAliHLTCalo


# ALIROOTHLTEMCALINCLUDE	= -I$(ALICE_ROOT)/HLT/EMCAL   -I$(ALICE_ROOT)/HLT/PHOS    -I$(ALICE_ROOT)/EMCAL -I$(ALICE_ROOT)/HLT/EMCAL/OnlineDisplay
# ALIROOTHLTINCLUDE	= -I$(ALICE_ROOT)/HLT/BASE/HOMER  -I$(ALICE_ROOT)/HLT/BASE -I$(ALICE_ROOT)/HLT/PHOS 

ALIROOTHLTEMCALINCLUDE	= -I$(ALICE_ROOT)/HLT/EMCAL       -I$(ALICE_ROOT)/EMCAL -I$(ALICE_ROOT)/HLT/EMCAL/OnlineDisplay
ALIROOTHLTINCLUDE	= -I$(ALICE_ROOT)/HLT/BASE/HOMER  -I$(ALICE_ROOT)/HLT/BASE 

ALIHLTINCLUDE		=-I$(ALIHLT_TOPDIR)/BASE/HOMER  -I$(ALIHLT_TOPDIR)/BASE   -I$(ALIHLT_TOPDIR)/CALO
ALIHLTEMCALINCLUDE	=-I$(ALIHLT_TOPDIR)/EMCAL


INCLUDES        = -I$(ROOTSYS)/include $(ALIHLTINCLUDE)  $(ALIHLTEMCALINCLUDE)  $(ALIROOTHLTEMCALINCLUDE) $(ALIROOTHLTINCLUDE)


MAINO					= main.$(ObjSuf)
MAINS					= main.$(SrcSuf)

GETEVENTBUTTONO				=AliHLTEMCALGetEventButton.$(ObjSuf)
GETEVENTBUTTONS				=AliHLTEMCALGetEventButton.$(SrcSuf)
GETEVENTBUTTONH				=AliHLTEMCALGetEventButton.$(HeadSuf)

ALIHLTEMCALONLINEDISPLAYO		=AliHLTEMCALOnlineDisplay.$(ObjSuf)
ALIHLTEMCALONLINEDISPLAYS		=AliHLTEMCALOnlineDisplay.$(SrcSuf)
ALIHLTEMCALONLINEDISPLAYH		=AliHLTEMCALOnlineDisplay.$(HeadSuf)

ALIHLTEMCALONLINEDISPLAYTH2DO		=AliHLTEMCALOnlineDisplayTH2D.$(ObjSuf)
ALIHLTEMCALONLINEDISPLAYTH2DS		=AliHLTEMCALOnlineDisplayTH2D.$(SrcSuf)
ALIHLTEMCALONLINEDISPLAYTH2DH		=AliHLTEMCALOnlineDisplayTH2D.$(HeadSuf)

ALIHLTEMCALONLINEDISPLAYBUTTONO		=AliHLTEMCALOnlineDisplayButton.$(ObjSuf)
ALIHLTEMCALONLINEDISPLAYBUTTONS		=AliHLTEMCALOnlineDisplayButton.$(SrcSuf)
ALIHLTEMCALONLINEDISPLAYBUTTONH		=AliHLTEMCALOnlineDisplayButton.$(HeadSuf)

ALIHLTEMCALONLINEDISPLAYTABO		=AliHLTEMCALOnlineDisplayTab.$(ObjSuf)
ALIHLTEMCALONLINEDISPLAYTABS		=AliHLTEMCALOnlineDisplayTab.$(SrcSuf)
ALIHLTEMCALONLINEDISPLAYTABH		=AliHLTEMCALOnlineDisplayTab.$(HeadSuf)

ALIHLTEMCALONLINEDISPLAYEVENTTABO	=AliHLTEMCALOnlineDisplayEventTab.$(ObjSuf)
ALIHLTEMCALONLINEDISPLAYEVENTTABS	=AliHLTEMCALOnlineDisplayEventTab.$(SrcSuf)
ALIHLTEMCALONLINEDISPLAYEVENTTABH	=AliHLTEMCALOnlineDisplayEventTab.$(HeadSuf)

ALIHLTEMCALONLINEDISPLAYFOURIERTABO	=AliHLTEMCALOnlineDisplayFourierTab.$(ObjSuf)
ALIHLTEMCALONLINEDISPLAYFOURIERTABS	=AliHLTEMCALOnlineDisplayFourierTab.$(SrcSuf)
ALIHLTEMCALONLINEDISPLAYFOURIERTABH	=AliHLTEMCALOnlineDisplayFourierTab.$(HeadSuf)

ALIHLTEMCALONLINEDISPLAYCALIBTABO	=AliHLTEMCALOnlineDisplayCalibTab.$(ObjSuf)
ALIHLTEMCALONLINEDISPLAYCALIBTABS	=AliHLTEMCALOnlineDisplayCalibTab.$(SrcSuf)
ALIHLTEMCALONLINEDISPLAYCALIBTABH	=AliHLTEMCALOnlineDisplayCalibTab.$(HeadSuf)


OBJS	= $(MAINO)  $(ALIHLTEMCALONLINEDISPLAYO)  $(ALIHLTEMCALONLINEDISPLAYTH2DO) $(ALIHLTEMCALONLINEDISPLAYTABO)\
	$(ALIHLTEMCALONLINEDISPLAYEVENTTABO)  	$(ALIHLTEMCALONLINEDISPLAYFOURIERTABO)  $(ALIHLTEMCALONLINEDISPLAYCALIBTABO) \
	$(GETEVENTBUTTONO) $(ALIHLTEMCALONLINEDISPLAYNUMBERENTRYO) $(ALIHLTEMCALONLINEDISPLAYBUTTONO)


pftest:$(OBJS)
	g++ -ggdb -g $(ROOTLIBS) $(ALIROOTLIBS) $(HLTLIBS)  $(OBJS) -o onlinedisplay
#HLT:	$(OBJS)	
#	g++ -ggdb -g $(ROOTLIBS) $(ALIHLTLIBS) $(ALIROOTLIBS) $(HLTLIBS)  $(OBJS) -o onlinedisplay	
HLT:	$(OBJS)	
	g++ -ggdb -g $(ROOTLIBS) $(ALIHLTLIBS) $(HLTLIBS)  $(OBJS) -o onlinedisplay	

$(MAINO):$(MAINS)
	g++ -ggdb -g $(INCLUDES)  -c $(MAINS)

$(ALIHLTEMCALONLINEDISPLAYO):$(ALIHLTEMCALONLINEDISPLAYS) $(ALIHLTEMCALONLINEDISPLAYH)
	g++ -ggdb -g $(INCLUDES)  -c $(ALIHLTEMCALONLINEDISPLAYS)

$(ALIHLTEMCALONLINEDISPLAYTH2DO):$(ALIHLTEMCALONLINEDISPLAYTH2DS) $(ALIHLTEMCALONLINEDISPLAYTH2DH)
	g++ -ggdb -g $(INCLUDES)  -c $(ALIHLTEMCALONLINEDISPLAYTH2DS)

$(ALIHLTEMCALONLINEDISPLAYBUTTONO):$(ALIHLTEMCALONLINEDISPLAYBUTTONS) $(ALIHLTEMCALONLINEDISPLAYBUTTONH)
	g++ -ggdb -g $(INCLUDES)  -c $(ALIHLTEMCALONLINEDISPLAYBUTTONS)

$(ALIHLTEMCALONLINEDISPLAYTABO):$(ALIHLTEMCALONLINEDISPLAYTABS) $(ALIHLTEMCALONLINEDISPLAYTABH)
	g++ -ggdb -g $(INCLUDES)  -c $(ALIHLTEMCALONLINEDISPLAYTABS)

$(ALIHLTEMCALONLINEDISPLAYEVENTTABO):$(ALIHLTEMCALONLINEDISPLAYEVENTTABS) $(ALIHLTEMCALONLINEDISPLAYEVENTTABH)
	g++ -ggdb -g $(INCLUDES)  -c $(ALIHLTEMCALONLINEDISPLAYEVENTTABS)

$(ALIHLTEMCALONLINEDISPLAYFOURIERTABO):$(ALIHLTEMCALONLINEDISPLAYFOURIERTABS) $(ALIHLTEMCALONLINEDISPLAYFOURIERTABH)
	g++ -ggdb -g $(INCLUDES)  -c $(ALIHLTEMCALONLINEDISPLAYFOURIERTABS)

$(ALIHLTEMCALONLINEDISPLAYCALIBTABO):$(ALIHLTEMCALONLINEDISPLAYCALIBTABS) $(ALIHLTEMCALONLINEDISPLAYCALIBTABH)
	g++ -ggdb -g $(INCLUDES)  -c $(ALIHLTEMCALONLINEDISPLAYCALIBTABS)

$(GETEVENTBUTTONO):$(GETEVENTBUTTONS) $(GETEVENTBUTTONH)
	g++ -ggdb -g $(INCLUDES)  -c $(GETEVENTBUTTONS)

clean:
	@rm -f *o *~ *# pftest
