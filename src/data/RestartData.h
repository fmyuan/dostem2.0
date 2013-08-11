#ifndef RESTARTDATA_H_
#define RESTARTDATA_H_

#include "../inc/errorcode.h"
#include "../inc/cohortconst.h"
#include "../inc/layerconst.h"

class RestartData {
	public :
		RestartData();
		~RestartData();
		void reinitValue();
		
		int chtid;
		
	    // atm
		int dsr;
	    double firea2sorgn;

		//vegegetation
	    int yrsdist;

	    int ifwoody;                  // - 'veg_dim'
	    int ifdeciwoody;
	    int ifperenial;
	    int nonvascular;
	    double fnonvascular;

	    int vegage;
	    double vegcov;
	    double lai;
	    double rootfrac[MAX_ROT_LAY];

	    double vegwater;             //canopy water - 'vegs_env'
	    double vegsnow;              //canopy snow  - 'vegs_env'

	    double vegc[NUM_PFT_PART];   // - 'vegs_bgc'
	    double labn;
	    double strn[NUM_PFT_PART];
	    double deadc;
	    double deadn;

	    double topt;            // yearly-evolved 'topt'
	    double eetmx;           // yearly max. month 'eet'
	    double unnormleafmx;    // yearly max. unnormalized 'fleaf'
	    double growingttime;    // yearly growthing t-time
	    double foliagemx;        // this is for f(foliage) in GPP to be sure f(foliage) not going down

	    double toptA[10];           // this is for f(temp) in GPP to calculate the mean of the 10 previous values
	    double eetmxA[10];           // this is for f(phenology) in GPP to calculate the mean of the 10 previous values
	    double unnormleafmxA[10];
	    double growingttimeA[10];

	    //snow
	    int numsnwl;
	    double snwextramass;
	    double TSsnow[MAX_SNW_LAY];
	    double DZsnow[MAX_SNW_LAY]; 
	    double LIQsnow[MAX_SNW_LAY];
	    double RHOsnow[MAX_SNW_LAY]; 
	    double ICEsnow[MAX_SNW_LAY]; 
	    double AGEsnow[MAX_SNW_LAY];

	    //ground-soil
	    int numsl;
	    double monthsfrozen;
		int rtfrozendays;
		int rtunfrozendays;
	    double watertab;

	    double DZsoil[MAX_SOI_LAY];
	    int TYPEsoil[MAX_SOI_LAY];
	    int AGEsoil[MAX_SOI_LAY];
	    double TSsoil[MAX_SOI_LAY]; 
	    double LIQsoil[MAX_SOI_LAY]; 
	    double ICEsoil[MAX_SOI_LAY];
	    double FROZENFRACsoil[MAX_SOI_LAY];
	    int FROZENsoil[MAX_SOI_LAY];
	    int TEXTUREsoil[MAX_SOI_LAY];

	    double TSrock[MAX_ROC_LAY]; 
	    double DZrock[MAX_ROC_LAY];

	    double frontZ[MAX_NUM_FNT];
	    int frontFT[MAX_NUM_FNT];
	     
	    double wdebrisc;
	    double dmossc;
	    double rawc[MAX_SOI_LAY];
	    double soma[MAX_SOI_LAY];
	    double sompr[MAX_SOI_LAY];
	    double somcr[MAX_SOI_LAY];

	    double wdebrisn;
	    double dmossn;
	    double orgn[MAX_SOI_LAY];
	    double avln[MAX_SOI_LAY];

	  	double prvltrfcnA[12][MAX_SOI_LAY];   //previous 12-month litterfall (root death) input C/N ratios in each soil layer for adjusting 'kd'
   	
};

#endif /*RESTARTDATA_H_*/
