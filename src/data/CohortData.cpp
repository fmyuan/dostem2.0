#include "CohortData.h"

CohortData::CohortData(){

};

CohortData::~CohortData(){

};

// initialize CohortData class explicitly
void CohortData::clear(){

	chtid = MISSING_I;
 	year  = MISSING_I;
 	month = MISSING_I;
 	day   = MISSING_I;

	cmttype = MISSING_I;
	yrsdist = MISSING_I;

    hasnonvascular = false;
    fnonvascular = MISSING_D;

	act_vegset  = MISSING_I;
	fill_n(vegyear, MAX_VEG_SET, MISSING_I);
	fill_n(vegtype, MAX_VEG_SET, MISSING_I);
	fill_n(vegfrac, MAX_VEG_SET, MISSING_I);

	act_fireset  = MISSING_I;
	fill_n(fireyear, MAX_FIR_OCRNUM, MISSING_I);
	fill_n(fireseason, MAX_FIR_OCRNUM, MISSING_I);
	fill_n(firesize, MAX_FIR_OCRNUM, MISSING_I);
	fill_n(fireseverity, MAX_FIR_OCRNUM, MISSING_I);

    act_atm_drv_yr  = MISSING_I;
    fill_n(tair, MAX_ATM_DRV_YR*12, MISSING_F);
    fill_n(prec, MAX_ATM_DRV_YR*12, MISSING_F);
    fill_n(nirr, MAX_ATM_DRV_YR*12, MISSING_F);
    fill_n(vapo, MAX_ATM_DRV_YR*12, MISSING_F);

  	// community dimension
    d_veg = vegstate_dim();
    m_veg = vegstate_dim();
    y_veg = vegstate_dim();

	m_vegd = vegdiag_dim();
	y_vegd = vegdiag_dim();

	d_snow = snwstate_dim();
	m_snow = snwstate_dim();
	y_snow = snwstate_dim();

	d_soil = soistate_dim();
	m_soil = soistate_dim();
	y_soil = soistate_dim();

		if(!prveetmxque.empty()) prveetmxque.clear();
		if(!prvunnormleafmxque.empty()) prvunnormleafmxque.clear();
		if(!prvgrowingttimeque.empty()) prvgrowingttimeque.clear();
		if(!toptque.empty()) toptque.clear();

};

//accumulators for yearly-averaged/-summed variables from the monthly ones
void CohortData::beginOfYear(){

	// At first, we set the yealy to the monthly,
	// so that if not varies within a year, set them same as the monthly all the time
	// this will avoid non-data just in case, although essentiall all data operating is at monthly
	// ALSO be sure the initialization was done on monthly data sets (i.e., m_veg, m_snow, m_soil)
	y_veg  = m_veg;
	y_snow = m_snow;
	y_soil = m_soil;

	// then, initialize the accumulators ONLY for those varies within a year

	// 1) for vegetation dimension/structure variables
	y_vegd.fpcsum = 0.;

		y_veg.lai  = 0.;
		y_veg.fpc  = 0.;

		for (int il=0; il<MAX_ROT_LAY; il++){
			y_veg.frootfrac[il] = 0.;
		}

		y_vegd.fleaf        = 0.;
		y_vegd.ffoliage     = 0.;
		y_vegd.eetmx        = 0.;
		y_vegd.growingttime = 0.;
		y_vegd.topt         = 0.;
		y_vegd.unnormleafmx = 0.;

	// 2) snow
	y_snow.numsnwl = MISSING_I;
	y_snow.thick = 0.;
	y_snow.dense = 0.;
	y_snow.extramass = 0.;
	for (int i=0; i<MAX_SNW_LAY; i++) {
		y_snow.age[i] = MISSING_D;          //yearly layered data make no sense
		y_snow.dz[i]  = MISSING_D;
		y_snow.por[i] = MISSING_D;
		y_snow.rho[i] = MISSING_D;
	}

	// 3) soil
	for (int il=0; il<MAX_SOI_LAY; il++){
			y_soil.frootfrac[il] = 0.;
	}

	y_soil.mossthick  = 0.;
	y_soil.shlwthick  = 0.;
	y_soil.deepthick  = 0.;
	y_soil.totthick   = 0.;

}

//accumulators for those monthly-averaged/-summed variables from the daily ones
void CohortData::beginOfMonth(){

	m_snow.thick = 0.;
	m_snow.dense = 0.;
	m_snow.extramass = 0.;

	m_snow.numsnwl = MISSING_I;
	for (int i=0; i<MAX_SNW_LAY; i++) {
		m_snow.age[i] = MISSING_D;          //monthly layered data make no sense
		m_snow.dz[i]  = MISSING_D;
		m_snow.por[i] = MISSING_D;
		m_snow.rho[i] = MISSING_D;
	}

}

// set the daily dimension variables for veg/soil
void CohortData::beginOfDay(){

	d_veg  = m_veg;       // daily veg dimension will not change within a month, and 'm_veg' will be always set-up or updated
	d_soil = m_soil;      // daily soil dimension will not change within a month, and 'm_soil' will be always set-up or updated

}

// accumulating monthly variables from the daily ones after the daily process is done
void CohortData::endOfDay(const int & dinm){

	// only 'snow' dimension changes at daily timestep ('veg' updaates monthly, while 'soil' yearly)
	m_snow.thick += d_snow.thick/dinm;
	m_snow.dense += d_snow.dense/dinm;
	m_snow.extramass += d_snow.extramass/dinm;
	m_snow.olds = d_snow.olds;

}

// this is called when monthly calculation is done
// then, accumulating the yearly variables from the monthly
void CohortData::endOfMonth(){

	// 1) for vegetation dimension/structure variables
	y_vegd.fpcsum += m_vegd.fpcsum/12.;
   	if (m_veg.vegcov>0.){

    		y_veg.lai += m_veg.lai/12.;
    		y_veg.fpc += m_veg.fpc/12.;
    		for (int il=0; il<MAX_ROT_LAY; il++){
    			y_veg.frootfrac[il] += m_veg.frootfrac[il]/12.;
    		}

    		y_vegd.growingttime  = m_vegd.growingttime;

    		y_vegd.unnormleaf   += m_vegd.unnormleaf/12.;
    		y_vegd.fleaf        += m_vegd.fleaf/12.;
    		y_vegd.ffoliage     += m_vegd.ffoliage/12.;

    		y_vegd.eetmx         = m_vegd.eetmx;
    		y_vegd.unnormleafmx  = m_vegd.unnormleafmx;
    		y_vegd.topt          = m_vegd.topt;
    		y_vegd.foliagemx     = m_vegd.foliagemx;
    		y_vegd.maxleafc      = m_vegd.maxleafc;

	}

	// 2) snow
	y_snow.thick += m_snow.thick/12.;
	y_snow.dense += m_snow.dense/12.;
	y_snow.extramass += m_snow.extramass/12.;

	// 3) soil: constant within a year, except for the root distribution
	y_soil = m_soil;
	for (int il=0; il<MAX_SOI_LAY; il++){
			y_soil.frootfrac[il] += m_soil.frootfrac[il]/12.;    // need to update 'rootfrac' in soil monthly
	}

};

void CohortData::endOfYear(){

	yrsdist++;

	// save the yearly max. 'unnormaleaf', 'growing thermal time', and 'topt' into the deque
		double tmpeetmx = y_vegd.eetmx;
		prveetmxque.push_front(tmpeetmx);
		if (prveetmxque.size()>10) {
			prveetmxque.pop_back();
		}

		double tmpmx = y_vegd.unnormleafmx;
		prvunnormleafmxque.push_front(tmpmx);
		if (prvunnormleafmxque.size()>10) {
			prvunnormleafmxque.pop_back();
		}

		double tmpttimex = y_vegd.growingttime;
		prvgrowingttimeque.push_front(tmpttimex);
		if (prvgrowingttimeque.size()>10) {
			prvgrowingttimeque.pop_back();
		}

		double tmptopt = y_vegd.topt;
		toptque.push_front(tmptopt);
		if (toptque.size()>10) {
			toptque.pop_back();
		}

};

