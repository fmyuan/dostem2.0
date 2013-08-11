/*
 * Vegetation.cpp
 *
 * Purpose: Defining vegetation structure
 *
 * History:
 *     June 28, 2011, by F.-M. Yuan:
 *          (1) added for constructing vegetation domain (plant community)
 *
 * Important:
 *     (1) Parameters are read from 'CohortLookup.cpp', and set to 'vegdimpar' (struct:: vegpar_dim)
 *     (2) Here, all functions are on ALL pfts for ONE community. In this way, some complicated PFT interaction
 *      and dynamics/structure changing may be put here in the future.
 *
 */

#include "Vegetation.h"

Vegetation::Vegetation(){
	
};

Vegetation::~Vegetation(){
	
};

// set the bgc parameters from inputs stored in 'chtlu' - reuseable
// Note: here will remove those PFT with no greater than zero 'fpc'
//       and initialize the total actual pft number

void Vegetation::initializeParameter(){
    	if (chtlu->vegcov > 0.){    //this will remove those PFTs with 0 ground coverage. So be cautious the index consistent with 'state' variables

    		vegdimpar.sla      = chtlu->sla;
    		vegdimpar.klai     = chtlu->klai;
  
    		vegdimpar.minleaf = chtlu->minleaf;
    		vegdimpar.aleaf   = chtlu->aleaf;
    		vegdimpar.bleaf   = chtlu->bleaf;
    		vegdimpar.cleaf   = chtlu->cleaf;

    		vegdimpar.kfoliage = chtlu->kfoliage;
    		vegdimpar.cov      = chtlu->cov;
    		vegdimpar.m1 = chtlu->m1;
    		vegdimpar.m2 = chtlu->m2;
    		vegdimpar.m3 = chtlu->m3;
    		vegdimpar.m4 = chtlu->m4;

    	}

};

// set the initial states from inputs
void Vegetation::initializeState(){

	//
	cd->m_veg.vegage  = 0;

	// from 'lookup'
	cd->hasnonvascular = false;
	cd->fnonvascular = 0.;
	cd->m_veg.fnonvascular = 0.;
	double nonvasc = chtlu->nonvascular; // this data input has two parts.
   	if (chtlu->vegcov > 0.){
    		cd->m_veg.vegcov      = chtlu->vegcov;

    		cd->m_veg.ifwoody     = chtlu->ifwoody;
    		cd->m_veg.ifdeciwoody = chtlu->ifdeciwoody;
    		cd->m_veg.ifperenial  = chtlu->ifperenial;

    		cd->m_veg.nonvascular = floor(nonvasc);
    		if (cd->m_veg.nonvascular > 0) {
        		cd->m_veg.fnonvascular = nonvasc-floor(nonvasc);
    			cd->hasnonvascular = true;
    			cd->fnonvascular = cd->m_veg.fnonvascular;
    		}

    		cd->m_veg.lai         = chtlu->lai;

    		for (int il=0; il<MAX_ROT_LAY; il++) {
    			cd->m_veg.frootfrac[il] = chtlu->frootfrac[il]/100.;   //chtlu - in %
    		}

    }

    updateFpc();

    updateFrootfrac();

};

//set the initial states from restart inputs:
void Vegetation::initializeState5restart(RestartData *resin){

   		cd->m_veg.vegage      = resin->vegage;

    	cd->m_veg.vegcov      = resin->vegcov;
    	cd->m_veg.ifwoody     = resin->ifwoody;
    	cd->m_veg.ifdeciwoody = resin->ifdeciwoody;
    	cd->m_veg.ifperenial  = resin->ifperenial;
    	cd->m_veg.nonvascular = resin->nonvascular;
    	cd->m_veg.fnonvascular= resin->fnonvascular;
    	cd->m_veg.lai         = resin->lai;

    	for (int il=0; il<MAX_ROT_LAY; il++) {
          cd->m_veg.frootfrac[il] = resin->rootfrac[il];
    	}

    	cd->m_vegd.eetmx        = resin->eetmx;
    	cd->m_vegd.unnormleafmx = resin->unnormleafmx;
    	cd->m_vegd.growingttime = resin->growingttime;
    	cd->m_vegd.topt         = resin->topt;
    	cd->m_vegd.foliagemx    = resin->foliagemx;

   		cd->prveetmxque.clear();
       	for(int i=0; i<10; i++){
    		double eetmxa = resin->eetmxA[i];    //note: older value is in the lower position in the deque
    		if(eetmxa!=MISSING_D){
    			cd->prveetmxque.push_back(eetmxa);
    		}
    	}

		cd->prvunnormleafmxque.clear();
    	for(int i=0; i<10; i++){
    		double unleafmxa = resin->unnormleafmxA[i];    //note: older value is in the lower position in the deque
    		if(unleafmxa!=MISSING_D){
    			cd->prvunnormleafmxque.push_back(unleafmxa);
    		}
    	}

		cd->prvgrowingttimeque.clear();
    	for(int i=0; i<10; i++){
    		double growingttimea = resin->growingttimeA[i];    //note: older value is, lower in the deque
    		if(growingttimea!=MISSING_D){
    			cd->prvgrowingttimeque.push_back(growingttimea);
    		}
    	}

		cd->toptque.clear();
    	for(int i=0; i<10; i++){
    		double topta = resin->toptA[i];    //note: older value is, lower in the deque
    		if(topta!=MISSING_D){
    			cd->toptque.push_back(topta);
    		}

    	}

    	updateFpc();

    	updateFrootfrac();

};

// must be called after 'foliage' and 'leaf' updated
void Vegetation::updateLai(const int &currmind){
    	if (cd->m_veg.vegcov>0.){
    		if(!updateLAI5vegc){
    			cd->m_veg.lai = chtlu->envlai[currmind];     //So, this will give a portal for input LAI

    		}else {
    			if (bd->m_vegs.c[I_leaf] > 0.) {
    				cd->m_veg.lai = vegdimpar.sla * bd->m_vegs.c[I_leaf];
    			} else {
    				if (ed->m_soid.rtdpgrowstart>0 && ed->m_soid.rtdpgrowend<0) {
    					cd->m_veg.lai = 0.001;   // this is needed for leaf emerging
    				}
    			}
    		}
    	}

};

// sum of all PFTs' fpc must be not greater than 1.0
void Vegetation::updateFpc(){
	double fpc;

	if (cd->m_veg.vegcov>0.){

			double ilai = cd->m_veg.lai;
			fpc = 1.0 - exp(-vegdimpar.klai * ilai);
			cd->m_veg.fpc = fpc;

	}

	if (fpc > 1.0) {
		fpc = 1.0;
	}
	cd->m_vegd.fpcsum = fpc;

};

// vegetation coverage update (note - this is not same as FPC)
// and Here it's simply assumed as the max. foliage coverage projected on ground throughout the whole plant lift-time
// shall be more working on this in future
void Vegetation::updateVegcov(){
	double foliagecov = 0.;
	cd->hasnonvascular = false;
	cd->fnonvascular   = 0.0;
	double ilai = cd->m_veg.lai;
	foliagecov = 1.0 - exp(-vegdimpar.klai * ilai);
	if (cd->m_veg.vegcov<foliagecov) {
			cd->m_veg.vegcov=foliagecov;
	}

	if (cd->m_veg.vegcov>1.e-5) {
			cd->m_veg.ifwoody     = chtlu->ifwoody;
			cd->m_veg.ifdeciwoody = chtlu->ifdeciwoody;
			cd->m_veg.ifperenial  = chtlu->ifperenial;
			double nonvasc = chtlu->nonvascular;
    		cd->m_veg.nonvascular = floor(nonvasc);
    		cd->m_veg.fnonvascular= nonvasc-floor(nonvasc);

			if (cd->m_veg.nonvascular > 0) cd->hasnonvascular = true;
	}

};

//leaf phenology - moved from 'Vegetation_Bgc.cpp' for easy modification, if needed in the future
void Vegetation::phenology(const int &currmind){

		if (cd->m_veg.vegcov>0.){

			double prvunnormleafmx = 0.;   // previous 10 years' average as below
			deque <double> prvdeque = cd->prvunnormleafmxque;
			int dequeno = prvdeque.size();
			for (int i=0; i<dequeno; i++) {
				prvunnormleafmx +=prvdeque[i]/dequeno;
			}

			double prveetmx=0;
			prvdeque = cd->prveetmxque;
			dequeno = prvdeque.size();
			for (int i=0; i<dequeno; i++) {
				prveetmx +=prvdeque[i]/dequeno;
			}

			// 1) current EET and previous max. EET controlled
			double tempunnormleaf = 0.;;
			double eet = ed->m_v2a.tran;  //originally it's using 'l2a.eet', which includes soil/veg evaporation - that may not relate to leaf phenology
			tempunnormleaf = getUnnormleaf(prveetmx, eet, cd->m_vegd.unnormleaf);
			cd->m_vegd.unnormleaf = tempunnormleaf;  // prior to here, the 'unnormleaf' is from the previous month

			double fleaf = getFleaf(tempunnormleaf, prvunnormleafmx);
			if (cd->m_veg.lai<=0.) fleaf = 0.;
			cd->m_vegd.fleaf = fleaf;

			// set the phenological variables of the year
			if (currmind == 0) {
				cd->m_vegd.eetmx = eet;
				cd->m_vegd.unnormleafmx = tempunnormleaf;
				cd->m_vegd.growingttime = ed->m_soid.rtdpgdd;
				cd->m_vegd.topt = ed->m_atms.ta;

				cd->m_vegd.maxleafc = getYearlyMaxLAI()/vegdimpar.sla;
			} else {
		    	if (eet>cd->m_vegd.eetmx) {
		    		cd->m_vegd.eetmx = eet;
		    	}

		    	if (cd->m_vegd.unnormleafmx < tempunnormleaf) {
					cd->m_vegd.unnormleafmx = tempunnormleaf;
					cd->m_vegd.topt = ed->m_atms.ta;   // it's updating monthly for current year and then update the 'deque', but not used in 'GPP' estimation
				}

				if (cd->m_vegd.growingttime<ed->m_soid.rtdpgdd) {  // here, we take the top root zone degree-days since growing started
					cd->m_vegd.growingttime=ed->m_soid.rtdpgdd;
				}
			}

			// 2) plant size (biomass C) or age controlled foliage fraction rative to the max. leaf C
			cd->m_vegd.ffoliage = getFfoliage(cd->m_veg.ifwoody, cd->m_veg.ifperenial, bd->m_vegs.call);

		} else { // 'vegcov' is 0
			cd->m_vegd.unnormleaf = MISSING_D;
			cd->m_vegd.fleaf      = MISSING_D;

			cd->m_vegd.eetmx        = MISSING_D;
			cd->m_vegd.unnormleafmx = MISSING_D;
			cd->m_vegd.topt         = MISSING_D;
			cd->m_vegd.maxleafc     = MISSING_D;

			cd->m_vegd.growingttime = MISSING_D;

			cd->m_vegd.ffoliage = MISSING_D;
		}

};

// functions for eet adjusted foliage growth index
// 'prvunleaf' is the unnormalized leaf from last time period
// 'prveetmx' is monthly eetmx of previous simulation period (year)

double Vegetation::getUnnormleaf(double &prveetmx, const double & eet, const double & prvunleaf){
  	double normeet;
  	double unnormleaf;

  	if (prveetmx <= 0.0) {
  		prveetmx = 1.0;
  	}

  	normeet = eet/prveetmx;
  	if(normeet>1) normeet =1;

  	unnormleaf = (vegdimpar.aleaf * normeet)
  			    +(vegdimpar.bleaf * prvunleaf)
                +vegdimpar.cleaf;

  	if (unnormleaf < (0.1 * vegdimpar.minleaf)) {
    	unnormleaf = 0.1 * vegdimpar.minleaf;
  	}

  	return unnormleaf;
};

//fleaf is normalized EET and previous EET determined phenology index 0~1
//i.e., f(phenology) in gpp calculation
double Vegetation::getFleaf(const double & unnormleaf, const double &prvunnormleafmx){
  	double fleaf;

  	if (prvunnormleafmx <= 0.0) {
  	 	fleaf = 0.0;
  	} else {
  	 	fleaf= unnormleaf/prvunnormleafmx;
   	}

  	if (fleaf < vegdimpar.minleaf ){
    	fleaf = vegdimpar.minleaf;
  	} else  if (fleaf > 1.0 ) {
   		fleaf = 1.0;
   	}

  	return fleaf;
};

// function for biomass C adjusted foliage growth index (0 - 1.0)
double Vegetation::getFfoliage(const bool & ifwoody, const bool &ifperenial, const double &vegc){

	double ffoliage =0;

  	if(!ifwoody){
  		if (!ifperenial) {
  			ffoliage = 1.0;    //annual: yearly max. not controlled by current plant C biomass (because it dies every year)
  		} else {
  			ffoliage = 1.0/(1.0 + vegdimpar.kfoliage * exp(vegdimpar.cov* vegc));
  		}

 	} else {

 		//from Zhuang et al., 2003
 		double m1 = vegdimpar.m1;
 		double m2 = vegdimpar.m2;
 		double m3 = vegdimpar.m3;
 		double m4 = vegdimpar.m4;

 		double fcv = m3*vegc /(1+m4*vegc);
 		ffoliage =  1./(1+m1*exp(m2*sqrt(fcv)));
	}

    //it is assumed that foliage will not go down during a growth cycle
  	if(ffoliage>cd->m_vegd.foliagemx){
  		cd->m_vegd.foliagemx = ffoliage;
  	}else{
 		ffoliage = cd->m_vegd.foliagemx;
  	}

  	return ffoliage;
};

// plant max. LAI function derived from biomass C
double Vegetation::getYearlyMaxLAI(){

	double laimax = 0.;

	for (int im=0; im<12; im++) {   // taking the max. of input 'envlai[12]' adjusted by 'vegcov'
		double covlai = chtlu->envlai[im]*cd->m_veg.vegcov;
		if (laimax<=covlai) laimax = covlai;
	}
	laimax *= cd->m_vegd.ffoliage;

  	return laimax;
};


// the following can be developed further for dynamical fine root distribution
// currently, it's only do some checking
void Vegetation::updateFrootfrac(){

    	if (cd->m_veg.vegcov>0.){

    		double totrootfrac = 0.;
    		for (int il=0; il<MAX_ROT_LAY; il++){
    			if (cd->m_veg.frootfrac[il]>0.) {
    				totrootfrac+=cd->m_veg.frootfrac[il];
    			}
    		}

		//
    		if (totrootfrac>0.) {
    			for (int il=0; il<MAX_ROT_LAY; il++){
    				cd->m_veg.frootfrac[il] /= totrootfrac;
    			}

    		} else {
    			for (int il=1; il<MAX_ROT_LAY; il++){
    				cd->m_veg.frootfrac[il] = 0.;
    			}

    		}

    	} // end of 'vegcov>0'

};


void Vegetation::setCohortLookup(CohortLookup* chtlup){
  	 chtlu = chtlup;
};

void Vegetation::setCohortData(CohortData* cdp){
  	 cd = cdp;
};

void Vegetation::setEnvData(EnvData* edp){
  	 ed = edp;
};

void Vegetation::setBgcData(BgcData* bdp){
	 bd = bdp;
};
