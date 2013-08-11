/*
 *  Cohort.cpp
 *
 * Purpose: call TEM core processes at community (cohort)-level within a common grid
 *
 * History:
 *     June 28, 2011, by F.-M. Yuan:
 *          (1) Recoding based on DOS-TEM's code;
 *          (2) DVM concepts added
 *
 * Important Notes:
 *     (1) There are two sets of 'ed', 'bd': one set is for 'vegetation' with multiple PFTs;
 *     and another set is the integrated for all. These two is not same for PFTs, but same for 'ground'
 *
 *
 */

#include "Cohort.h"

Cohort::Cohort(){

};

Cohort::~Cohort(){

};

// initialization of pointers used in modules called here
void Cohort::initSubmodules(){

	// for controlling of error messaging in some subroutines
	ground.debugging = md->consoledebug;
	soilenv.tempupdator.debugging = md->consoledebug;
	soilenv.stefan.debugging      = md->consoledebug;
	soilenv.richards.debugging    = md->consoledebug;

 	//atmosphere module pointers
	atm.setCohortData(&cd);
	atm.setEnvData(&ed);

 	// ecosystem domain
	veg.setCohortData(&cd);
	veg.setCohortLookup(&chtlu);

	ground.setCohortLookup(&chtlu);

 	// vegetation module pointers
	veg.setEnvData(&ed);
	veg.setBgcData(&bd);

 	vegenv.setCohortLookup(&chtlu);
 	vegenv.setEnvData(&ed);
 	vegenv.setCohortData(&cd);
 	vegenv.setFirData(fd);

 	vegbgc.setCohortLookup(&chtlu);
 	vegbgc.setCohortData(&cd);
 	vegbgc.setEnvData(&ed);
 	vegbgc.setBgcData(&bd);

	//snow-soil module pointers
 	snowenv.setGround(&ground);
 	snowenv.setCohortLookup(&chtlu);
	snowenv.setCohortData(&cd);
 	snowenv.setEnvData(&ed);

 	soilenv.setGround(&ground);
 	soilenv.setCohortLookup(&chtlu);
	soilenv.setCohortData(&cd);
	soilenv.setEnvData(&ed);

	solprntenv.setGround(&ground);
	solprntenv.setEnvData(&ed);

 	soilbgc.setGround(&ground);
	soilbgc.setCohortLookup(&chtlu);
	soilbgc.setCohortData(&cd);
	soilbgc.setEnvData(&ed);
	soilbgc.setBgcData(&bd);
	soilbgc.setFirData(fd);

    //fire module pointers
	fire.setCohortLookup(&chtlu);
	fire.setCohortData(&cd);
	fire.setAllEnvBgcData(&ed, &bd);
 	fire.setFirData(fd);

	//BGC states change integration module pointers
	vegintegrator.setBgcData(&bd);
	vegintegrator.setVegetation_Bgc(&vegbgc);


	solintegrator.setBgcData(&bd);
 	solintegrator.setSoil_Bgc(&soilbgc);

 	// Output data pointers
 	outbuffer.setDimensionData(&cd);
	outbuffer.setProcessData(&ed, &bd);
	outbuffer.setFireData(fd);

};

// The following 'set...' functions allow initialized data pointers outside be used here
void Cohort::setTime(Timer * timerp){
  	timer = timerp;
};

void Cohort::setModelData(ModelData* mdp){
  	md = mdp;
};

void Cohort::setInputData(RegionData * rdp, GridData * gdp){

  	rd = rdp;
  	gd = gdp;

	cd.rd = rd;
	cd.gd = gd;

};

void Cohort::setProcessData(FirData *fdp){

	fd = fdp;
 	bd.cd = &cd;
 	ed.cd = &cd;

};

//re-initializing for a new community of all PFTs sharing same atm/snow-soil domains within a grid
void Cohort::initStatePar() {

	// some variables in 'cd' require reset if 'restart' as initmode
	if (md->initmode>=3) {
		cd.yrsdist = resid.yrsdist;
	}

 	// FOR VEGETATION
	//vegetation dimension/structure
	veg.initializeParameter();
	if(md->initmode<3){      // from 'chtlu' or 'sitein'
		veg.initializeState();
	} else {     // initmode  >=3: restart
		veg.initializeState5restart(&resid);
	}

	//set-up paramters for vegetation processes
		vegenv.initializeParameter();
		vegbgc.initializeParameter();

		if(md->initmode<3){
			vegbgc.initializeState();
			vegenv.initializeState();

		} else {
			vegbgc.initializeState5restart(&resid);
			vegenv.initializeState5restart(&resid);
		}

 	 // initialize dimension/structure for snow-soil
	// first read in the default initial parameter for snow/soil
	ground.initParameter();

 	snowenv.initializeParameter();
  	soilenv.initializeParameter();
 	soilbgc.initializeParameter();

	if(md->initmode < 3){    //lookup or sitein

		ground.initDimension();   //read-in snow/soil structure from 'chtlu'

		// reset the soil texture data from grid-level soil.nc, rather than 'chtlu',
 	    // Note that the mineral layer structure is already defined above
		if (md->runmode==2 || md->runmode==3){  //region-TEM runmode
			float z=0;
			for (int i=0; i<ground.mineral.num; i++){
	 			 z+=ground.mineral.dz[i];
	 			 if (z<=0.30) {   //assuming the grid top-soil texture is for top 30 cm
	 				 ground.mineral.texture[i] = gd->topsoil;
	 			 } else {
	 				 ground.mineral.texture[i] = gd->botsoil;
	 			 }

			}
		}

 		 // then if we have sitein.nc, as specified. In this way, if sitein.nc may not provide
 		 // all data, then model will still be able to use the default.
	    if(md->initmode ==2){ //from sitein.nc specified as md->initialfile
//	    	setSiteStates(&sitein);
	    }

	    // set-up the snow-soil-soilparent structure
	    ground.initLayerStructure(&cd.d_snow, &cd.m_soil);   //snow updated daily, while soil dimension at monthly
	    cd.d_soil = cd.m_soil;

	    // initializing snow/soil/soilparent env state conditions after layerStructure done
	    snowenv.initializeNewSnowState();  //Note: ONE initial snow layer as new snow
	    soilenv.initializeState();
	    solprntenv.initializeState();

	    // initializing soil bgc state conditions
		soilbgc.initializeState();


	} else {    //restart

		// set-up the snow-soil structure from restart data
 		ground.initLayerStructure5restart(&cd.d_snow, &cd.m_soil, &resid);   //snow updated daily, while soil dimension at monthly
	    cd.d_soil = cd.m_soil;

 		// initializing snow/soil env state conditions from restart data
 		snowenv.initializeState5restart(&resid);
 		soilenv.initializeState5restart(&resid);
 		solprntenv.initializeState5restart(&resid);

 		// initializing soil bgc state conditions from restart data
 		soilbgc.initializeState5restart(&resid);

	}

	//integrating the individual 'bd' initial conditions into 'bdall' initial conditions, if veg involved
	getBd4allveg_monthly();
 
	// fire processes
	fd->init();
	if(md->initmode<3){
		fire.initializeState();
	} else {
		fire.initializeState5restart(&resid);
	}
	fire.initializeParameter();


};

void Cohort::prepareAllDrivingData(){

    // climate monthly data for all atm years
    atm.prepareMonthDrivingData();

    //fire driving data (if input) for all years
    if (!md->friderived && !md->runeq) {
        fire.prepareDrivingData();
    }
};

// climate daily data for one year
void Cohort::prepareDayDrivingData(const int & yrindx, const int & usedatmyr){

	//default climate/co2 setting
	bool changeclm = true;
	bool changeco2 = true;
	if (md->runeq) {
		changeclm = false;
		changeco2 = false;
	} else if (md->runsp) {
		changeco2 = false;
	}

	// preparing ONE year daily driving data
	if (timer->eqend) {
		// run the model after eq stage, climate and co2 driver controlled by setting in control file.
		if (md->changeclimate == 1) {
			changeclm = true;
		} else if (md->changeclimate == -1) {
			changeclm = false;
		}

		if (md->changeco2 == 1) {
			changeco2 = true;
		} else if (md->changeco2 == -1) {
			changeco2 = false;
		}

		atm.prepareDayDrivingData(yrindx, usedatmyr, changeclm, changeco2);

	} else {
		// run the model at eq stage, climate and co2 driver not controlled by setting in control file.
		atm.prepareDayDrivingData(yrindx, usedatmyr, false, false);

	}
};

void Cohort::updateMonthly(const int & yrcnt, const int & currmind, const int & dinmcurr){

	//
	if(currmind==0) cd.beginOfYear();
	cd.beginOfMonth();

  	// first, update the water/thermal process to get (bio)physical conditions
 	if(md->envmodule){
  		updateMonthly_Env(currmind, dinmcurr);
  	}

 	// secondly, update the current dimension/structure of veg-snow/soil column (domain)
   	updateMonthly_DIMveg(currmind, md->dvmmodule);

   	updateMonthly_DIMgrd(currmind, md->dslmodule);

   	//thirdly, update the BGC process to get the C/N states and fluxes
  	if(md->bgcmodule){
  		updateMonthly_Bgc(currmind);
  	}

  	// fourthly, run the disturbance module
   	if(md->dsbmodule){
   	   	updateMonthly_Fir(yrcnt, currmind);
   	}

	cd.endOfMonth();
	if(currmind==11) cd.endOfYear();

	////////////////////////////
	// output all data for multple cohorts
	if (md->outRegn) {
		outbuffer.updateRegnOutputBuffer(currmind);
	}

	// always output the restart data (monthly)
	outbuffer.updateRestartOutputBuffer();

};

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//Environment Module Calling at monthly time-step, but involving daily time-step loop
/////////////////////////////////////////////////////////
void Cohort::updateMonthly_Env(const int & currmind, const int & dinmcurr){

//Yuan: note that the Veg-Env module calling is for a few PFTs within ONE cohort
//      1) ed calling is done for each PFTs within the module
//      2) Env-module calling is done for one PFT, so needs loop for vegetation-relevant processes

    // (i) the n factor for soil temperature calculation from Tair

    ed.d_soid.nfactor =1;
    // Yuan: the following has temporarily commentted out - a lot of trouble
/*	if(currmind>=5 && currmind<=9){  //for warm season: Yuan: this will make a BIG jump of soil temperature at 5/9
		if(cd.ifdeciwoody){      //deciduous woody community type
			ed.d_soid.nfactor = 0.94;
		}
		if(cd.ifconiwoody) {
			if(fd->ysf <veg.vegdimpar.matureagemx){
				ed.d_soid.nfactor = 1.1 -(fd->ysf)/veg.vegdimpar.matureagemx * (1.1 -0.66);
			}else{
				ed.d_soid.nfactor =0.66;
		    }
		}
	}
*/

	// (ii)Initialize the yearly/monthly accumulators, which are accumulating at the end of month/day in 'ed'

	if(currmind==0){
		ed.atm_beginOfYear();
		ed.veg_beginOfYear();
		ed.grnd_beginOfYear();
	}
	ed.atm_beginOfMonth();
	ed.veg_beginOfMonth();
	ed.grnd_beginOfMonth();

	// (iii) daily light/water processes at plant canopy
	double tdrv, daylength;
	for(int id =0; id<dinmcurr; id++){

		cd.day = id+1;

		int doy =timer->getDOYIndex(currmind, id);
		daylength = gd->alldaylengths[doy];

		//get the daily atm drivers and the data is in 'edall'
		atm.updateDailyAtm(currmind, id);

		//Initialize some daily variables for 'ground'
		cd.beginOfDay();
		ed.grnd_beginOfDay();

		if (cd.d_veg.vegcov>0.){
					// get the soil moisture controling factor on plant transpiration
					double frootfr[MAX_SOI_LAY];
					for (int i=0; i<MAX_SOI_LAY; i++){
						frootfr[i] = cd.m_soil.frootfrac[i];
					}

					soilenv.getSoilTransFactor(ed.d_soid.fbtran, ground.fstsoill, frootfr);
					ed.d_vegd.btran = 0.;
					for (int il=0; il<MAX_SOI_LAY; il++) {
						ed.d_vegd.btran+=ed.d_soid.fbtran[il];
					}

				// calculate vegetation light/water dynamics at daily timestep
				vegenv.updateRadiation();
				vegenv.updateWaterBalance(daylength);  //daylength in hours

		}

		// integrating daily 'veg' portion in 'ed'
		getEd4allveg_daily();

/*
		if (cd.year==18 && doy==111){
			cout<<"checking";
		}
//*/
		tdrv = ed.d_atms.ta;

		// Snow-soil Env-module: ground/soil temperatur e- moisture dynamics at daily timestep
		// note: hydrology is done separately for snow and soil, but thermal process is done as a continuous column
		//       so, thermal process (including phase changing) is carried out before hydrological process
		soilenv.updateDailyGroundT(tdrv, daylength); // snow-soil temperature, including snow-melting and soil water phase changing

		snowenv.updateDailyM(tdrv);                  // snow water/thickness changing - must be done after 'T' because of melting

		// get the new bottom drainage layer and its depth, which needed for soil moisture calculation
		ground.setDrainL(ground.lstsoill, ed.d_soid.ald, ed.d_sois.watertab);
		soilenv.updateDailySM();  //soil moisture

		// save the variables to daily 'edall' (Note: not PFT specified)
		soilenv.retrieveDailyTM(ground.toplayer, ground.lstsoill);
		solprntenv.retrieveDailyTM(ground.lstsoill);   //assuming rock layer's temperature equal to that of lstsoill

		getEd4land_daily();  // integrating 'veg' and 'ground' into 'land'

		//accumulate daily vars into monthly for 'ed'
		ed.atm_endOfDay(dinmcurr);
		ed.veg_endOfDay(dinmcurr);              //be sure 'getEd4allpfts_daily' called above
		ed.grnd_endOfDay(dinmcurr, doy);

		ground.retrieveSnowDimension(&cd.d_snow);   // update Snow structure at daily timestep (for soil structure at yearly timestep in ::updateMonthly_DIMgrd)
		cd.endOfDay(dinmcurr);

		// accumulate yearly vars at the last day of a month for all pfts
		if(id==dinmcurr-1){
			ed.atm_endOfMonth();
			ed.veg_endOfMonth(currmind);
			ed.grnd_endOfMonth();
		}

		////////////////////////////
		//output data store for daily - because the output is implemented monthly
		if (md->outSiteDay) {
			outbuffer.assignSiteDlyOutputBuffer_Env(cd.d_snow, id);   // '-1' indicates for all-pft integrated datasets
		}
	
	} // end of day loop in a month

};

///////////////////////////////////////////////////////////////////////////////////////////
// Biogeochemical Module Calling at monthly timestep
///////////////////////////////////////////////////////////////////////////////////////////
void Cohort::updateMonthly_Bgc(const int & currmind){
	//
	if(currmind==0){		

		if (cd.m_veg.vegcov>0.){
	    		bd.veg_beginOfYear();

	    		bd.soil_beginOfYear();
	    		bd.land_beginOfYear();

		}

		bd.veg_beginOfYear();
		bd.soil_beginOfYear();
		bd.land_beginOfYear();
	}

	// vegetation BGC module calling
   	if (cd.m_veg.vegcov>0.){

    		vegbgc.prepareIntegration(md->nfeed);
		 	vegintegrator.updateMonthlyVbgc();
    		vegbgc.afterIntegration();

    		if(currmind==11){
    			vegbgc.adapt();             // this will evolve C/N ratio with CO2
    		}

	}

	getBd4allveg_monthly();      // integrating the monthly pfts' 'bd' adjusted by 'vegcov'
   	bd.veg_endOfMonth();    // yearly data accumulation
	if(currmind==11){
		bd.veg_endOfYear();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
	// soil BGC module calling
	soilbgc.prepareIntegration(md->nfeed, md->avlnflg, md->baseline);
	solintegrator.updateMonthlySbgc(MAX_SOI_LAY);
    soilbgc.afterIntegration();

	bd.soil_endOfMonth();   // yearly data accumulation
	bd.land_endOfMonth();

};

//fire disturbance module calling
/////////////////////////////////////////////////////////////////////////////////
void Cohort::updateMonthly_Fir(const int & yrind, const int & currmind){

	if(currmind ==0){
		fd->beginOfYear();

		fire.getOccur(yrind, md->friderived);
	}

   	if (yrind==fire.oneyear && currmind==fire.onemonth){
		//fire, C burning for each PFT, and C/N pools updated through 'bd', but not soil structure
   		// soil root fraction also updated through 'cd'
   		fire.burn();

   		// summarize burned veg C/N of individual 'bd' for each PFT above
		if (cd.m_veg.vegcov>0.){

   				for (int i=0; i<NUM_PFT_PART; i++) {
   					bd.m_vegs.c[i]    += bd.m_vegs.c[i] * cd.m_veg.vegcov;
   					bd.m_vegs.strn[i] += bd.m_vegs.strn[i] * cd.m_veg.vegcov;
   				}

   				bd.m_vegs.labn    += bd.m_vegs.labn * cd.m_veg.vegcov;
   				bd.m_vegs.call    += bd.m_vegs.call * cd.m_veg.vegcov;
   				bd.m_vegs.strnall += bd.m_vegs.strnall * cd.m_veg.vegcov;
   				bd.m_vegs.nall    += bd.m_vegs.nall * cd.m_veg.vegcov;

   				bd.m_vegs.deadc   += bd.m_vegs.deadc * cd.m_veg.vegcov;
   				bd.m_vegs.deadn   += bd.m_vegs.deadn * cd.m_veg.vegcov;

   		}

   		// assign the updated soil C/N pools during firing to double-linked layer matrix in 'ground'
   		soilbgc.assignCarbonBd2LayerMonthly();

		// then, adjusting soil structure after fire burning (Don't do this prior to the previous calling)
		ground.adjustSoilAfterburn();

		// and finally save the data back to 'bdall'
		soilbgc.assignCarbonLayer2BdMonthly();

		// update 'cd'
		cd.yrsdist = 0.;
		ground.retrieveSnowDimension(&cd.d_snow);
		ground.retrieveSoilDimension(&cd.m_soil);
		cd.d_soil = cd.m_soil;
		cd.y_soil = cd.m_soil;

 		getSoilFineRootFrac_Monthly();
  	}

};

/////////////////////////////////////////////////////////////////////////////////
//   Dynamical Vegetation Module (DVM) calling
////////////////////////////////////////////////////////////////////////////////
void Cohort::updateMonthly_DIMveg(const int & currmind, const bool & dvmmodule){

	//switch for using LAI read-in (false) or dynamically with vegC
    // the read-in LAI is through the 'chtlu->envlai[12]', i.e., a 12 monthly-LAI
	if (dvmmodule) {
		veg.updateLAI5vegc = md->updatelai;
	} else {
		veg.updateLAI5vegc = false;
	}

	// vegetation standing age
	// tentatively set to a common age from 'ysf' - year since fire - should have more varability based on PFT types
   	if (cd.m_veg.vegcov>0.){
    		cd.m_veg.vegage = cd.yrsdist;
    		if (cd.m_veg.vegage<=0) cd.m_vegd.foliagemx = 0.;
	}

	// update monthly phenological variables (factors used for GPP), and LAI
	veg.phenology(currmind);
	veg.updateLai(currmind);    // this must be done after phenology

    // LAI updated above for each PFT, but FPC (foliage percent cover) may need adjustment
	veg.updateFpc();
	veg.updateVegcov();

	veg.updateFrootfrac();

};

/////////////////////////////////////////////////////////////////////////////////
//   Dynamical Soil Layer Module (DSL)
////////////////////////////////////////////////////////////////////////////////
void Cohort::updateMonthly_DIMgrd(const int & currmind, const bool & dslmodule){

	// re-call the 'bdall' soil C contents and assign them to the double-linked layer matrix
	soilbgc.assignCarbonBd2LayerMonthly();

	//only update the thickness at begin of year, since it is a slow process
	if(dslmodule && currmind==0){
		// calculate the OSL layer thickness from C contents
		ground.updateOslThickness5Carbon(ground.fstsoill);

		// above callings didn't modify the layer matrix structure
		// in case that some layers may be getting too thick or too thin due to C content dynamics
		// then, re-do layer division or combination is necessary for better thermal/hydrological simulation
		if (cd.hasnonvascular && ground.moss.type<=0) {  //
			double prvpft = 0.;
	    	if (cd.m_veg.nonvascular!=I_vascular){
		    		if (cd.m_veg.vegcov>prvpft)
		    			ground.moss.type = cd.d_veg.nonvascular;
			}

		}
	    ground.redivideSoilLayers();

		// and save the bgc data in double-linked structure back to 'bdall'
		soilbgc.assignCarbonLayer2BdMonthly();

	}

	// update soil dimension
	ground.retrieveSoilDimension(&cd.m_soil);
	getSoilFineRootFrac_Monthly();
	cd.d_soil = cd.m_soil;      //soil dimension remains constant in a month

}

//////////////////////////////////////////////////////////////////////////////////////
// adjusting fine root fraction in soil
void Cohort::getSoilFineRootFrac_Monthly(){

	double mossthick = cd.m_soil.mossthick;
	double totfrootc = 0.;   //fine root C summed for all PFTs

	if (cd.m_veg.vegcov>0.){

			double layertop, layerbot;
			// covert PFT 10-layer root fraction to acculative ones for interpolating
			double cumrootfrac[MAX_ROT_LAY];
			cumrootfrac[0] = cd.m_veg.frootfrac[0];
			for (int il=1; il<MAX_ROT_LAY; il++){
				cumrootfrac[il] = cumrootfrac[il-1]+cd.m_veg.frootfrac[il];
			}

			// calculate soil fine root fraction from PFT's 10-rootlayer structure
			// note: at this point, soil fine root fraction ACTUALLY is root biomass C distribution along soil profile
			for (int il=0; il<cd.m_soil.numsl; il++){
				if (cd.m_soil.type[il]>0) {   // non-moss soil layers only
					layertop = cd.m_soil.z[il] - mossthick;
					layerbot = cd.m_soil.z[il]+cd.m_soil.dz[il]-mossthick;

					cd.m_soil.frootfrac[il] = assignSoilLayerRootFrac(layertop, layerbot, cumrootfrac, ROOTTHICK);  //fraction
					cd.m_soil.frootfrac[il] *= bd.m_vegs.c[I_root];  //root C

					totfrootc += cd.m_soil.frootfrac[il];

				}
			}

	}

	// soil fine root fraction - adjusted by both vertical distribution and root biomass
   	if (cd.m_veg.vegcov>0.){
    		for (int il=0; il<cd.m_soil.numsl; il++){
    			if (cd.m_soil.type[il]>0 && cd.m_soil.frootfrac[il]>0.) {   // non-moss soil layers only
    				cd.m_soil.frootfrac[il] /= totfrootc;
    			} else {
    				cd.m_soil.frootfrac[il] = 0.;
    			}

    		}

	}

};

double Cohort::assignSoilLayerRootFrac(const double & topz, const double & botz,
		     const double cumrootfrac[MAX_ROT_LAY], const double dzrotlay[MAX_ROT_LAY]){

	// determine soil layer's location in the root layer matrix
	int indxtop = -1;
	int indxbot = -1;
	double zrotlay[MAX_ROT_LAY];  // root layer top
	zrotlay[0] = 0.;
	for (int i=1; i<MAX_ROT_LAY; i++){
		zrotlay[i] = zrotlay[i-1]+dzrotlay[i];
		if (topz >= zrotlay[i-1] && topz<zrotlay[i]) {
			indxtop = i-1;
		}
		if ((botz >= zrotlay[i-1] && botz<zrotlay[i]) ||
			(i==MAX_ROT_LAY-1 && botz>=zrotlay[i])) {
			indxbot = i-1;
			break;
		}

	}

	//calculating fine root fraction in a layer by linearly interpolation
	double frfrac=0.;

	double sumfractop = 0.0;
	double sumfracbot = 0.0;
	if (indxtop >= 0. && indxbot>=0) {
		if (indxtop == 0) {
			sumfractop = cumrootfrac[indxtop]/dzrotlay[indxtop]*topz;
		} else {
			sumfractop = (cumrootfrac[indxtop]-cumrootfrac[indxtop-1])/dzrotlay[indxtop]
				        * (topz-zrotlay[indxtop])+cumrootfrac[indxtop-1];
		}

		if (indxbot == 0) {
			sumfracbot = cumrootfrac[indxbot]/dzrotlay[indxbot]*botz;
		} else {
			sumfracbot = (cumrootfrac[indxbot]-cumrootfrac[indxbot-1])/dzrotlay[indxbot]
				        * (botz-zrotlay[indxbot])+cumrootfrac[indxbot-1];
		}

		frfrac = sumfracbot - sumfractop;
		if (frfrac<0.0) frfrac = 0.0;
	} else {
		frfrac = 0.;
	}

	return frfrac;

};

// integrating (fpc weighted) 'soid.fbtran'
void Cohort::getSoilTransfactor4all_daily(){

	for (int il=0; il<MAX_SOI_LAY; il++) {
		ed.d_soid.fbtran[il] = 0.;
    	if (cd.d_veg.vegcov>0.){
	    		ed.d_soid.fbtran[il] += ed.d_soid.fbtran[il]  * cd.d_veg.vegcov;
		}
	}
}

// integrating (fpc weighted) 'veg' portion in 'ed'
void Cohort::getEd4allveg_daily(){

   	if (cd.d_veg.vegcov>0.){

		ed.d_vegs.rwater  *= cd.d_veg.vegcov;
		ed.d_vegs.snow    *= cd.d_veg.vegcov;

		ed.d_vegd.rc      *= cd.d_veg.vegcov;
		ed.d_vegd.cc      *= cd.d_veg.vegcov;
		ed.d_vegd.btran   *= cd.d_veg.vegcov;
		ed.d_vegd.m_ppfd  *= cd.d_veg.vegcov;
		ed.d_vegd.m_vpd   *= cd.d_veg.vegcov;

		ed.d_a2v.rnfl     *= cd.d_veg.vegcov;
		ed.d_a2v.rinter   *= cd.d_veg.vegcov;
		ed.d_a2v.snfl     *= cd.d_veg.vegcov;
		ed.d_a2v.sinter   *= cd.d_veg.vegcov;
		ed.d_a2v.swdown   *= cd.d_veg.vegcov;
		ed.d_a2v.swinter  *= cd.d_veg.vegcov;
		ed.d_a2v.pardown  *= cd.d_veg.vegcov;
		ed.d_a2v.parabsorb*= cd.d_veg.vegcov;

		ed.d_v2a.swrefl   *= cd.d_veg.vegcov;
		ed.d_v2a.evap     *= cd.d_veg.vegcov;
		ed.d_v2a.tran     *= cd.d_veg.vegcov;
		ed.d_v2a.evap_pet *= cd.d_veg.vegcov;
		ed.d_v2a.tran_pet *= cd.d_veg.vegcov;
		ed.d_v2a.sublim   *= cd.d_veg.vegcov;

		ed.d_v2g.swthfl   *= cd.d_veg.vegcov;
		ed.d_v2g.rdrip    *= cd.d_veg.vegcov;
		ed.d_v2g.rthfl    *= cd.d_veg.vegcov;
		ed.d_v2g.sdrip    *= cd.d_veg.vegcov;
		ed.d_v2g.sthfl    *= cd.d_veg.vegcov;

	}
}

// integrating 'veg' and 'ground' portion in 'ed' as 'land (community)'
// Note: this 'l2a' is monthly/yearly integrated in 'ed->atm_endofDay/_endofMonth'
void Cohort::getEd4land_daily(){

   	if (cd.d_veg.vegcov>0.){

    		ed.d_l2a.eet = ed.d_v2a.evap + ed.d_v2a.sublim + ed.d_v2a.tran
		                  +ed.d_snw2a.sublim + ed.d_soi2a.evap;

    		ed.d_l2a.pet = ed.d_v2a.evap_pet + ed.d_v2a.sublim + ed.d_v2a.tran_pet
		          +ed.d_snw2a.sublim + ed.d_soi2a.evap_pet;
	}

	//
	ed.d_l2a.eet = ed.d_v2a.evap + ed.d_v2a.sublim + ed.d_v2a.tran
			          +ed.d_snw2a.sublim + ed.d_soi2a.evap;

	ed.d_l2a.pet = ed.d_v2a.evap_pet + ed.d_v2a.sublim + ed.d_v2a.tran_pet
			          +ed.d_snw2a.sublim + ed.d_soi2a.evap_pet;

}

// integrating (vegfrac weighted) 'bd'
void Cohort::getBd4allveg_monthly(){

   	if (cd.m_veg.vegcov>0.){

		for (int i=0; i<NUM_PFT_PART; i++){
			bd.m_vegs.c[i]    *= cd.m_veg.vegcov;
			bd.m_vegs.strn[i] *= cd.m_veg.vegcov;

			bd.m_a2v.ingpp[i] *= cd.m_veg.vegcov;
			bd.m_a2v.innpp[i] *= cd.m_veg.vegcov;
			bd.m_a2v.gpp[i]   *= cd.m_veg.vegcov;
			bd.m_a2v.npp[i]   *= cd.m_veg.vegcov;
			bd.m_v2a.rg[i]    *= cd.m_veg.vegcov;
	 		bd.m_v2a.rm[i]    *= cd.m_veg.vegcov;

	 		bd.m_v2v.nmobil[i]  *= cd.m_veg.vegcov;
	 		bd.m_v2v.nresorb[i] *= cd.m_veg.vegcov;

 			bd.m_v2soi.ltrfalc[i] *= cd.m_veg.vegcov;
 			bd.m_v2soi.ltrfaln[i] *= cd.m_veg.vegcov;

			bd.m_soi2v.snuptake[i] *= cd.m_veg.vegcov;
		}
	 	bd.m_vegs.labn    *= cd.m_veg.vegcov;
		bd.m_vegs.call    *= cd.m_veg.vegcov;
		bd.m_vegs.strnall *= cd.m_veg.vegcov;
		bd.m_vegs.nall    *= cd.m_veg.vegcov;

		bd.m_a2v.ingppall *= cd.m_veg.vegcov;
		bd.m_a2v.innppall *= cd.m_veg.vegcov;
		bd.m_a2v.gppall   *= cd.m_veg.vegcov;
		bd.m_a2v.nppall   *= cd.m_veg.vegcov;
		bd.m_v2a.rgall    *= cd.m_veg.vegcov;
		bd.m_v2a.rmall    *= cd.m_veg.vegcov;

		bd.m_v2soi.ltrfalcall *= cd.m_veg.vegcov;
		bd.m_v2soi.ltrfalnall *= cd.m_veg.vegcov;

		bd.m_v2soi.mossdeathc *= cd.m_veg.vegcov;  //NOTE: non-vascular plants' litterfalling (mortality) is for death moss layer C
		bd.m_v2soi.mossdeathn *= cd.m_veg.vegcov;

		bd.m_v2v.nmobilall  *= cd.m_veg.vegcov;
		bd.m_v2v.nresorball *= cd.m_veg.vegcov;

	  	bd.m_soi2v.innuptake *= cd.m_veg.vegcov;
	  	for (int il=0; il<cd.m_soil.numsl; il++) {
	  		bd.m_soi2v.nextract[il] *= cd.m_veg.vegcov;
	  	}
	 	bd.m_soi2v.lnuptake   *= cd.m_veg.vegcov;
	 	bd.m_soi2v.snuptakeall*= cd.m_veg.vegcov;

    } // end of 'vegcov>0'

	// below litter-fall vertical distribution
	double sumrtltrfall = 0.;
	for (int il=0; il<cd.m_soil.numsl; il++) {
		bd.m_v2soi.rtlfalfrac[il] = 0.;
    	if (cd.m_veg.vegcov>0.){
	    		bd.m_v2soi.rtlfalfrac[il] = cd.m_soil.frootfrac[il] *
	    		                            bd.m_v2soi.ltrfalc[I_root];
		}
		sumrtltrfall +=bd.m_v2soi.rtlfalfrac[il];
	}

	for (int il=0; il<cd.m_soil.numsl; il++) {
		if (sumrtltrfall>0) {
			bd.m_v2soi.rtlfalfrac[il] /=sumrtltrfall;
		} else {
			bd.m_v2soi.rtlfalfrac[il] = 0.;
		}
	}

}

