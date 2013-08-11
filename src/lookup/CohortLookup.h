#ifndef COHORTLOOKUP_H_
#define COHORTLOOKUP_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
using namespace std;

#include "../inc/cohortconst.h"
#include "../inc/timeconst.h"
#include "../inc/layerconst.h"

class CohortLookup{
  	  public:
		CohortLookup();
		~CohortLookup();

		string dir;
		string cmtcode;

		void init();
        void assignBgcCalpar(string & dirname);

	//calibration related
	    //vegetation
	    double cmax;
	    double nmax;

	    double cfall[NUM_PFT_PART];
	    double nfall[NUM_PFT_PART];

	    double kra;                   // parameter for maintenance resp. (rm)
	    double krb[NUM_PFT_PART];     // parameter for maintenance resp. (rm)
	    double frg;                   // fraction of available NPP (GPP after rm) for growth respiration

	    // soil
	    double micbnup;  // parameter related to N immoblization by soil microbial

	    double kdcmoss;    // calibrated dead moss C material respiration rate (at 0oC)
	    double kdcrawc;    // calibrated soil raw C material respiration rate (at 0oC)
	    double kdcsoma;    // calibrated soil active SOM respiration rate (at 0oC)
	    double kdcsompr;   // calibrated soil physically-resistant SOM respiration rate (at 0oC)
	    double kdcsomcr;   // calibrated soil chemically-resistant SOM respiration rate (at 0oC)

	// canopy dimensions
	  	double sla;         // specific leaf area
	  	double klai;        // a coefficient to convert LAI to FPC (foliage percentage coverage)

	  	double vegcov;      // actual veg. covered fraction, NOTE this is different from 'fpc' - the former is for the whole canopy, while the latter is for foliage
	  	double lai;         // lai
	  	int ifwoody;        // woody (1) or non-woody (0)
	  	int ifdeciwoody;    // deciduous (1) or evergreen (0) woodland (forest or shrubland)
	  	int ifperenial;     // perenial plant (1) or not (0)
	  	double nonvascular; //vascular plant (0), sphagnum (1.??), feathermoss (2.??), others (3.??): the integer is the type, the decimal is the fracion of total veg. coverage

	  	double envlai[12];  // input static monthly lai for a year

    // root distribution
	    double frootfrac[MAX_ROT_LAY];   // percentage

   // snow dimensions
	    double snwdenmax;        // max. snow bulk density: kg snow /m3
	    double snwdennew;        // max. snow bulk density: kg snow /m3

	    double initsnwthick;     // initial snow thickness (m)
	    double initsnwdense;     // initial snow bulk density: kg snow /m3

   // ground/soil dimensions
  	    // moss
	  	double maxdmossthick;
	  	double initdmossthick;
	  	int mosstype;
	  	double coefmossa;//carbon vs thick
	  	double coefmossb;//carbon vs thick

	  	// soils
	  	double initfibthick;
        double inithumthick;

	  	double coefshlwa;//carbon vs thick
	  	double coefshlwb;//carbon vs thick

	    double coefdeepa;//carbon vs thick
	  	double coefdeepb;//carbon vs thick

	  	double coefminea;//carbon density vs ham
	  	double coefmineb;//carbon density vs ham

	  	int minetexture[MAX_MIN_LAY];

	  	// active root depth criteria for determining thawing/freezing-derived growing season and degree-day
	  	double rtdp4gdd;

  //for canopy biometeorological processes

	  	double albvisnir; // canopy radiation albedo for both visible and nir
	  	double er;        // canopy light extinction coefficient

	  	double ircoef;    // canopy interception coeff for rain
	  	double iscoef;    // canopy interception coeff for snow

	  	double glmax;  // maximum leaf conductance m/s
	  	double gl_bl;  // leaf boundary layer conductance m/s
	  	double gl_c;   // leaf cuticular conductance

	  	double vpd_open;  // vpd for starting of stomata open
	  	double vpd_close; // vpd for complete conductance reduction (stomata closure)
	    double ppfd50;    // ppfd for half stomata closure

	    double initvegwater;  // canopy rain water (mm)
	  	double initvegsnow;   // canopy snow water equivalent (mm)

  // ground/soil biophysical processes
	    // snow physical processes
	    double snwalbmax;        //max. albedo of snow
	    double snwalbmin;        //min. albedo of snow
	    // parameters for evapotranspiration
	    double psimax;        // max. soil matrical potential for transpiration (root water uptake)
	    double evapmin;       // min. reduction of potential soil evaporation after rainfall (that's why 'dsr' is needed)

	    // parameter for soil drainage
	    double drainmax;

	    // inital thermal/water states of snow/soil
	    double initsnwtem;  // 1 snow input: initial snow temperature (note: initial water of snow can be from initial thickness and density)
	    double initvwc[10]; // 10 soil initial input for each 10 cm thickness of layers
	    double initts[10];

  // for vegetation	BGC
	    //parameters for f(phenology) in GPP
	   	double minleaf;   //EET determined leaf phenology parameters
	 	double aleaf;
	  	double bleaf;
	  	double cleaf;

	  	//parameters for f(foliage) in GPP
	   	double kfoliage;    //for non-woody plants
	  	double cov;
	  	double m1;        //for woody plants
	  	double m2;
	  	double m3;
	  	double m4;

	  	//parameter for f(CO2) in GPP
	    double kc;

	    //parameter for f(par) in GPP
	    double ki;

	    //parameter for f(tair) in GPP
	    double tmin;
	    double toptmin;
	    double toptmax;
	    double tmax;

	    //parameter for f(tair) in nuptake
	    double raq10a0;
	    double raq10a1;
	    double raq10a2;
	    double raq10a3;

	    // parameter for root-zone water/avln in nuptake
	    double knuptake;

	    // parameters for GPP/NPP allocations in tissues
	    double cpart[NUM_PFT_PART];   //biomass C partioning fraction

	    // parameters for tissue C/N ratios at optimal states ('cneven')
	    double initc2neven[NUM_PFT_PART];
	    double c2nmin[NUM_PFT_PART];        // these 3 are for adjusting 'cneven' with atm. CO2
	    double c2na;
	    double c2nb[NUM_PFT_PART];

	    // a parameter to estimate plant labile N uptake
	    double labncon;
  
  // for soil mcrobial processes

	    // Q10 and soil moisture factors for Rh
	    double rhq10;
	    double moistmin;
	    double moistopt;
	    double moistmax;

	    // litterfall C/N ratio criteria to adjust soil C decomposition rates annually
	    double lcclnc;

	    // parameters for SOM C transformation
	    double fsoma;     // fraction of SOMA production during respiration
	    double fsompr;    // fraction of SOMPR production during respiration
	    double fsomcr;    // fraction of SOMCR production during respiration
	    double som2co2;   // ratio of total SOM production and CO2 release during respiration

	    // a parameter for soil N immobilization
	    double kn2;

	    // parameters for soil net N mineralization
	    double propftos;
	    double nmincnsoil;     // soil C/N ratio at which net mineralization occurs

	    // fraction of avln in soil leaching out with drainage water
	    double fnloss;


  //init values
	    double initvegc[NUM_PFT_PART];     //or, target value for calibration
	    double initvegn[NUM_PFT_PART];
	    double initdeadc;
	    double initdeadn;
  
	    double initdmossc;
	    double initshlwc;
	    double initdeepc;
	    double initminec;
	    double initsoln;    // total soil organic N
	    double initavln;    // total soil available N

//fire related parameters
	    double fvcombust[NUM_FSEVR]; // fraction of burned above-ground vegetation
	    double fvslash[NUM_FSEVR];   // fraction of slashed above-ground vegetation

	    double foslburn[NUM_FSEVR];        // fire severity based organic soil layer burning fraction
	    double vsmburn;            // a threshold value of VWC for burn organic layers

	    double r_retain_c;         // 0.1 calculated from Harden et al., 2003 (ATHarden42003a)
	    double r_retain_n;         // 0.3 calculated from Harden et al., 2003 (ATHarden42003a)

  private:

	    void assignDimension(string & dir);

	    void assignEnv(string & dir);

	    void assignBgc(string & dir);

	    void assignFirePar(string & dir);
   
};

#endif /*COHORTLOOKUP_H_*/
