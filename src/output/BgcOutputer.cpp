/*! \file 
*/

#include "BgcOutputer.h"

BgcOutputer::BgcOutputer(){
	
};

BgcOutputer::~BgcOutputer(){
 	if(ncfileenv!=NULL){
 		ncfileenv->close();
 		delete ncfileenv;
 	}
};

void BgcOutputer::init(string & dirfile){

	//file
	ncfname = dirfile;

	ncfileenv = new NcFile(ncfname.c_str(), NcFile::Replace);

	//dimension
	timeD    = ncfileenv->add_dim("tstep");
	partD    = ncfileenv->add_dim("vegpart", NUM_PFT_PART);
	soilayerD= ncfileenv->add_dim("soilayer", MAX_SOI_LAY);

    //variables
  	chtidV = ncfileenv->add_var("CHTID", ncInt);
  	errorV = ncfileenv->add_var("ERRORID", ncInt, timeD);
	yearV  = ncfileenv->add_var("YEAR", ncInt, timeD);
	monV   = ncfileenv->add_var("MONTH", ncInt, timeD);

	// veg C/N state variables
   	callV = ncfileenv->add_var("VEGCSUM", ncDouble, timeD);
   	cV = ncfileenv->add_var("VEGCPART", ncDouble, timeD, partD);
   	nallV = ncfileenv->add_var("VEGNSUM", ncDouble, timeD);
   	labnV = ncfileenv->add_var("VEGNLAB", ncDouble, timeD);
   	strnallV = ncfileenv->add_var("VEGNSTRNSUM", ncDouble, timeD);
   	strnV = ncfileenv->add_var("VEGNSTRNPART", ncDouble, timeD, partD);
   	deadcV = ncfileenv->add_var("VEGCDEAD", ncDouble, timeD);
   	deadnV = ncfileenv->add_var("VEGNDEAD", ncDouble, timeD);
   	wdebriscV = ncfileenv->add_var("WDEBRISC", ncDouble, timeD);
   	wdebrisnV = ncfileenv->add_var("WDEBRISN", ncDouble, timeD);

 	//veg C/N fluxes
   	gppftV = ncfileenv->add_var("GPPFTEMP", ncDouble, timeD);
   	gppgvV = ncfileenv->add_var("GPPGV", ncDouble, timeD);
   	gppfnaV = ncfileenv->add_var("GPPFNA", ncDouble, timeD);
   	gppfcaV = ncfileenv->add_var("GPPFCA", ncDouble, timeD);
   	raq10V = ncfileenv->add_var("RAQ10", ncDouble, timeD);
   	rmkrV = ncfileenv->add_var("RMKR", ncDouble, timeD);

	ingppallV = ncfileenv->add_var("INGPPALL", ncDouble, timeD);
	ingppV = ncfileenv->add_var("INGPP", ncDouble, timeD, partD);
	innppallV = ncfileenv->add_var("INNPPALL", ncDouble, timeD);
	innppV = ncfileenv->add_var("INNPP", ncDouble, timeD, partD);
	gppallV = ncfileenv->add_var("GPPALL", ncDouble, timeD);
	gppV = ncfileenv->add_var("GPP", ncDouble, timeD, partD);
	nppallV = ncfileenv->add_var("NPPALL", ncDouble, timeD);
	nppV = ncfileenv->add_var("NPP", ncDouble, timeD, partD);
	rmallV = ncfileenv->add_var("RMALL", ncDouble, timeD);
	rmV = ncfileenv->add_var("RM", ncDouble, timeD, partD);
	rgallV = ncfileenv->add_var("RGALL", ncDouble, timeD);
	rgV = ncfileenv->add_var("RG", ncDouble, timeD, partD);
	ltrfalcallV = ncfileenv->add_var("LTRFALCALL", ncDouble, timeD);
	ltrfalcV = ncfileenv->add_var("LTRFALC", ncDouble, timeD, partD);

	ltrfalnallV = ncfileenv->add_var("LTRFALNALL", ncDouble, timeD);
	ltrfalnV = ncfileenv->add_var("LTRFALN", ncDouble, timeD, partD);
	innuptakeV = ncfileenv->add_var("INNUPTAKE", ncDouble, timeD);
	nrootextractV = ncfileenv->add_var("NROOTEXTRACT", ncDouble, timeD, soilayerD);
	luptakeV = ncfileenv->add_var("NUPTAKEL", ncDouble, timeD);
	suptakeallV = ncfileenv->add_var("NUPTAKESALL", ncDouble, timeD);
	suptakeV = ncfileenv->add_var("NUPTAKES", ncDouble, timeD, partD);
	nmobilallV = ncfileenv->add_var("NMOBILALL", ncDouble, timeD);
	nmobilV = ncfileenv->add_var("NMOBIL", ncDouble, timeD, partD);
	nresorballV = ncfileenv->add_var("NRESOBALL", ncDouble, timeD);
	nresorbV = ncfileenv->add_var("NRESORB", ncDouble, timeD, partD);

   	// soil C/N state variables
   	rawcV = ncfileenv->add_var("RAWC", ncDouble, timeD, soilayerD);
   	somaV = ncfileenv->add_var("SOMA", ncDouble, timeD, soilayerD);
   	somprV = ncfileenv->add_var("SOMPR", ncDouble, timeD, soilayerD);
   	somcrV = ncfileenv->add_var("SOMCR", ncDouble, timeD, soilayerD);
   	orgnV = ncfileenv->add_var("ORGN", ncDouble, timeD, soilayerD);
   	avlnV = ncfileenv->add_var("AVLN", ncDouble, timeD, soilayerD);

   	shlwcV = ncfileenv->add_var("SOMCSHLW", ncDouble, timeD);
   	deepcV = ncfileenv->add_var("SOMCDEEP", ncDouble, timeD);
   	mineacV = ncfileenv->add_var("SOMCMINEA", ncDouble, timeD);
   	minebcV = ncfileenv->add_var("SOMCMINEB", ncDouble, timeD);
   	mineccV = ncfileenv->add_var("SOMCMINEC", ncDouble, timeD);
   	rawcsumV = ncfileenv->add_var("RAWCSUM", ncDouble, timeD);
   	somasumV = ncfileenv->add_var("SOMASUM", ncDouble, timeD);
   	somprsumV = ncfileenv->add_var("SOMPRSUM", ncDouble, timeD);
   	somcrsumV = ncfileenv->add_var("SOMCRSUM", ncDouble, timeD);
   	orgnsumV = ncfileenv->add_var("ORGNSUM", ncDouble, timeD);
   	avlnsumV = ncfileenv->add_var("AVLNSUM", ncDouble, timeD);

   	// soil C/N fluxes
   	rhV  = ncfileenv->add_var("RH", ncDouble, timeD);
   	knmoistV = ncfileenv->add_var("NMITKSOIL", ncDouble, timeD, soilayerD);
   	rhmoistV = ncfileenv->add_var("RHMOIST", ncDouble, timeD, soilayerD);
   	rhq10V = ncfileenv->add_var("RHQ10", ncDouble, timeD, soilayerD);
   	soilltrfcnV = ncfileenv->add_var("SOILLTRFCN", ncDouble, timeD, soilayerD);

   	nepV = ncfileenv->add_var("NEP", ncDouble, timeD);

   	grsnminV = ncfileenv->add_var("GRSNMIN", ncDouble, timeD);
   	netnminV = ncfileenv->add_var("NETNMIN", ncDouble, timeD);

	orgcinputV = ncfileenv->add_var("ORGCINPUT", ncDouble, timeD);
	orgninputV = ncfileenv->add_var("ORGNINPUT", ncDouble, timeD);
	avlninputV = ncfileenv->add_var("AVLNINPUT", ncDouble, timeD);

	doclostV  = ncfileenv->add_var("DOCLOST", ncDouble, timeD);      //DOC lost
	avlnlostV = ncfileenv->add_var("AVLNLOST", ncDouble, timeD);     // N leaching
	orgnlostV = ncfileenv->add_var("ORGNLOST", ncDouble, timeD);     // DON loss

	burnthickV  = ncfileenv->add_var("BURNTHICK", ncDouble, timeD);
   	burnsoicV   = ncfileenv->add_var("BURNSOIC", ncDouble, timeD);
   	burnvegcV   = ncfileenv->add_var("BURNVEGC", ncDouble, timeD);
   	burnsoinV   = ncfileenv->add_var("BURNSOIN", ncDouble, timeD);
   	burnvegnV   = ncfileenv->add_var("BURNVEGN", ncDouble, timeD);
   	burnretaincV= ncfileenv->add_var("BURNRETAINC", ncDouble, timeD);
   	burnretainnV= ncfileenv->add_var("BURNRETAINN", ncDouble, timeD);

}

void BgcOutputer::outputCohortBgcVars_mly(BgcData *bd, FirData *fd, const int &calyr, const int &calmon, const int & tstepcnt){
 	NcError err(NcError::verbose_nonfatal);

		if (tstepcnt==0) chtidV->put(&bd->cd->chtid);
		yearV->put_rec(&calyr, tstepcnt);
		monV->put_rec(&calmon, tstepcnt);

		// soil C/N state variables for all PFTs
   		wdebriscV->put_rec(&bd->m_sois.wdebrisc, tstepcnt);
   		wdebrisnV->put_rec(&bd->m_sois.wdebrisn, tstepcnt);
   		rawcV->put_rec(&bd->m_sois.rawc[0], tstepcnt);
   		somaV->put_rec(&bd->m_sois.soma[0], tstepcnt);
   		somprV->put_rec(&bd->m_sois.sompr[0], tstepcnt);
   		somcrV->put_rec(&bd->m_sois.somcr[0], tstepcnt);
   		orgnV->put_rec(&bd->m_sois.orgn[0], tstepcnt);
   		avlnV->put_rec(&bd->m_sois.avln[0], tstepcnt);

   		shlwcV->put_rec(&bd->m_soid.shlwc, tstepcnt);
   		deepcV->put_rec(&bd->m_soid.deepc, tstepcnt);
   		mineacV->put_rec(&bd->m_soid.mineac, tstepcnt);
   		minebcV->put_rec(&bd->m_soid.minebc, tstepcnt);
   		mineccV->put_rec(&bd->m_soid.minecc, tstepcnt);
   		rawcsumV->put_rec(&bd->m_soid.rawcsum, tstepcnt);
   		somasumV->put_rec(&bd->m_soid.somasum, tstepcnt);
   		somprsumV->put_rec(&bd->m_soid.somprsum, tstepcnt);
   		somcrsumV->put_rec(&bd->m_soid.somcrsum, tstepcnt);
   		orgnsumV->put_rec(&bd->m_soid.orgnsum, tstepcnt);
   		avlnsumV->put_rec(&bd->m_soid.avlnsum, tstepcnt);

   		//soil C/N fluxes

   	 	rhV->put_rec(&bd->m_soi2a.rhtot, tstepcnt);
		knmoistV->put_rec(&bd->m_soid.knmoist[0], tstepcnt);
   		rhmoistV->put_rec(&bd->m_soid.rhmoist[0], tstepcnt);

   		rhq10V->put_rec(&bd->m_soid.rhq10[0], tstepcnt);
   		soilltrfcnV->put_rec(&bd->m_soid.ltrfcn[0], tstepcnt);

   		nepV->put_rec(&bd->m_l2a.nep, tstepcnt);

   		orgcinputV->put_rec(&bd->m_a2soi.orgcinput, tstepcnt);
   		orgninputV->put_rec(&bd->m_a2soi.orgninput, tstepcnt);
   		avlninputV->put_rec(&bd->m_a2soi.avlninput, tstepcnt);

   		doclostV->put_rec(&bd->m_soi2l.doclost, tstepcnt);      //DOC lost
   		avlnlostV->put_rec(&bd->m_soi2l.avlnlost, tstepcnt);     // N leaching
   		orgnlostV->put_rec(&bd->m_soi2l.orgnlost, tstepcnt);     // DON loss

   		//
   		burnthickV->put_rec(&fd->fire_soid.burnthick, tstepcnt);
   		burnsoicV->put_rec(&fd->fire_soi2a.orgc, tstepcnt);
   		burnvegcV->put_rec(&fd->fire_v2a.orgc, tstepcnt);
   		burnsoinV->put_rec(&fd->fire_soi2a.orgn, tstepcnt);
   		burnvegnV->put_rec(&fd->fire_v2a.orgn, tstepcnt);
   		double ftmp1 = fd->fire_v2soi.abvc+fd->fire_v2soi.blwc;
   		burnretaincV->put_rec(&ftmp1, tstepcnt);
   		double ftmp2 = fd->fire_v2soi.abvn+fd->fire_v2soi.blwn;
   		burnretainnV->put_rec(&ftmp2, tstepcnt);

	// veg C/N state variables - output for each PFT
 	callV->set_cur(tstepcnt);
   	callV->put(&bd->m_vegs.call,1);

 	cV->set_cur(tstepcnt, 0);
 	cV->put(&bd->m_vegs.c[0],1, NUM_PFT_PART);

 	nallV->set_cur(tstepcnt);
   	nallV->put(&bd->m_vegs.nall,1);

   	labnV->set_cur(tstepcnt);
   	labnV->put(&bd->m_vegs.labn,1);

   	strnallV->set_cur(tstepcnt);
   	strnallV->put(&bd->m_vegs.strnall,1);

   	strnV->set_cur(tstepcnt, 0);
   	strnV->put(&bd->m_vegs.strn[0],1, NUM_PFT_PART);

   	deadcV->set_cur(tstepcnt);
   	deadcV->put(&bd->m_vegs.deadc,1);

   	callV->set_cur(tstepcnt);
   	deadnV->put(&bd->m_vegs.deadn,1);

 	//veg C/N fluxes for each PFT
 	gppftV->set_cur(tstepcnt);
   	gppftV->put(&bd->m_vegd.ftemp,1);

   	gppgvV->set_cur(tstepcnt);
   	gppgvV->put(&bd->m_vegd.gv,1);

   	gppfnaV->set_cur(tstepcnt);
   	gppfnaV->put(&bd->m_vegd.fna,1);

   	gppfcaV->set_cur(tstepcnt);
   	gppfcaV->put(&bd->m_vegd.fca,1);

   	raq10V->set_cur(tstepcnt);
   	raq10V->put(&bd->m_vegd.raq10,1);

   	rmkrV->set_cur(tstepcnt, 0);
   	rmkrV->put(&bd->m_vegd.kr[0],1, NUM_PFT_PART);

   	// C/N fluxes
 	ingppallV->set_cur(tstepcnt);
	ingppallV->put(&bd->m_a2v.ingppall,1);

	ingppV->set_cur(tstepcnt, 0);
	ingppV->put(&bd->m_a2v.ingpp[0],1, NUM_PFT_PART);

	innppallV->set_cur(tstepcnt);
	innppallV->put(&bd->m_a2v.innppall,1);

	innppV->set_cur(tstepcnt, 0);
	innppV->put(&bd->m_a2v.innpp[0],1, NUM_PFT_PART);

	gppallV->set_cur(tstepcnt);
	gppallV->put(&bd->m_a2v.gppall,1);

	gppV->set_cur(tstepcnt, 0);
	gppV->put(&bd->m_a2v.gpp[0],1, NUM_PFT_PART);

	nppallV->set_cur(tstepcnt);
	nppallV->put(&bd->m_a2v.nppall,1);

	nppV->set_cur(tstepcnt, 0);
	nppV->put(&bd->m_a2v.npp[0],1, NUM_PFT_PART);

	rmallV->set_cur(tstepcnt);
	rmallV->put(&bd->m_v2a.rmall, 1);

	rmV->set_cur(tstepcnt, 0);
	rmV->put(&bd->m_v2a.rm[0],1, NUM_PFT_PART);

 	rgallV->set_cur(tstepcnt);
	rgallV->put(&bd->m_v2a.rgall, 1);

 	rgV->set_cur(tstepcnt, 0);
 	rgV->put(&bd->m_v2a.rg[0],1, NUM_PFT_PART);

 	ltrfalcallV->set_cur(tstepcnt);
 	if (bd->cd->m_veg.nonvascular>0) {
 		ltrfalcallV->put(&bd->m_v2soi.mossdeathc, 1);
 	} else {
 		ltrfalcallV->put(&bd->m_v2soi.ltrfalcall, 1);
 	}

	ltrfalcV->set_cur(tstepcnt, 0);
	ltrfalcV->put(&bd->m_v2soi.ltrfalc[0],1, NUM_PFT_PART);

	//
 	ltrfalnallV->set_cur(tstepcnt);
 	if (bd->cd->m_veg.nonvascular>0) {
 		ltrfalnallV->put(&bd->m_v2soi.mossdeathn, 1);
 	} else {
 		ltrfalnallV->put(&bd->m_v2soi.ltrfalnall, 1);
 	}

	ltrfalnV->set_cur(tstepcnt, 0);
	ltrfalnV->put(&bd->m_v2soi.ltrfaln[0],1, NUM_PFT_PART);

	innuptakeV->set_cur(tstepcnt);
	innuptakeV->put(&bd->m_soi2v.innuptake, 1);

	nrootextractV->set_cur(tstepcnt, 0);
	nrootextractV->put(&bd->m_soi2v.nextract[0],1, MAX_SOI_LAY);

	luptakeV->set_cur(tstepcnt);
	luptakeV->put(&bd->m_soi2v.lnuptake, 1);

	suptakeallV->set_cur(tstepcnt);
	suptakeallV->put(&bd->m_soi2v.snuptakeall, 1);

	suptakeV->set_cur(tstepcnt);
	suptakeV->put(&bd->m_soi2v.snuptake[0],1, NUM_PFT_PART);

	nmobilallV->set_cur(tstepcnt);
	nmobilallV->put(&bd->m_v2v.nmobilall,1);

	nmobilV->set_cur(tstepcnt);
	nmobilV->put(&bd->m_v2v.nmobil[0],1, NUM_PFT_PART);

	nresorballV->set_cur(tstepcnt);
	nresorballV->put(&bd->m_v2v.nresorball,1);

	nresorbV->set_cur(tstepcnt);
	nresorbV->put(&bd->m_v2v.nresorb[0],1, NUM_PFT_PART);

}

void BgcOutputer::outputCohortBgcVars_yly(BgcData *bgcod, FirData *fd, const int &calyr, const int & tstepcnt){
 	NcError err(NcError::verbose_nonfatal);

		if (tstepcnt==0){
			chtidV->put(&bgcod->cd->chtid);
		}

		yearV->put_rec(&calyr, tstepcnt);
		monV->put_rec(&MISSING_I, tstepcnt);

		// soil C/N state variables
   		wdebriscV->put_rec(&bgcod->y_sois.wdebrisc, tstepcnt);
   		wdebrisnV->put_rec(&bgcod->y_sois.wdebrisn, tstepcnt);
   		rawcV->put_rec(&bgcod->y_sois.rawc[0], tstepcnt);
   		somaV->put_rec(&bgcod->y_sois.soma[0], tstepcnt);
   		somprV->put_rec(&bgcod->y_sois.sompr[0], tstepcnt);
   		somcrV->put_rec(&bgcod->y_sois.somcr[0], tstepcnt);
   		orgnV->put_rec(&bgcod->y_sois.orgn[0], tstepcnt);
   		avlnV->put_rec(&bgcod->y_sois.avln[0], tstepcnt);

   		shlwcV->put_rec(&bgcod->y_soid.shlwc, tstepcnt);
   		deepcV->put_rec(&bgcod->y_soid.deepc, tstepcnt);
   		mineacV->put_rec(&bgcod->y_soid.mineac, tstepcnt);
   		minebcV->put_rec(&bgcod->y_soid.minebc, tstepcnt);
   		mineccV->put_rec(&bgcod->y_soid.minecc, tstepcnt);
   		rawcsumV->put_rec(&bgcod->y_soid.rawcsum, tstepcnt);
   		somasumV->put_rec(&bgcod->y_soid.somasum, tstepcnt);
   		somprsumV->put_rec(&bgcod->y_soid.somprsum, tstepcnt);
   		somcrsumV->put_rec(&bgcod->y_soid.somcrsum, tstepcnt);
   		orgnsumV->put_rec(&bgcod->y_soid.orgnsum, tstepcnt);
   		avlnsumV->put_rec(&bgcod->y_soid.avlnsum, tstepcnt);

   		//soil C/N fluxes

   		rhV->put_rec(&bgcod->y_soi2a.rhtot, tstepcnt);
   		knmoistV->put_rec(&bgcod->y_soid.knmoist[0], tstepcnt);
   		rhmoistV->put_rec(&bgcod->y_soid.rhmoist[0], tstepcnt);
   		rhq10V->put_rec(&bgcod->y_soid.rhq10[0], tstepcnt);
   		soilltrfcnV->put_rec(&bgcod->y_soid.ltrfcn[0], tstepcnt);

   		nepV->put_rec(&bgcod->y_l2a.nep, tstepcnt);

   		// land
   		orgcinputV->put_rec(&bgcod->y_a2soi.orgcinput, tstepcnt);
   		orgninputV->put_rec(&bgcod->y_a2soi.orgninput, tstepcnt);
   		avlninputV->put_rec(&bgcod->y_a2soi.avlninput, tstepcnt);
		doclostV->put_rec(&bgcod->y_soi2l.doclost, tstepcnt);      //DOC lost
		avlnlostV->put_rec(&bgcod->y_soi2l.avlnlost, tstepcnt);     // N leaching
		orgnlostV->put_rec(&bgcod->y_soi2l.orgnlost, tstepcnt);     // DON loss

   		burnthickV->put_rec(&fd->fire_soid.burnthick, tstepcnt);
   		burnsoicV->put_rec(&fd->fire_soi2a.orgc, tstepcnt);
   		burnvegcV->put_rec(&fd->fire_v2a.orgc, tstepcnt);
   		burnsoinV->put_rec(&fd->fire_soi2a.orgn, tstepcnt);
   		burnvegnV->put_rec(&fd->fire_v2a.orgn, tstepcnt);
   		double ftmp1 = fd->fire_v2soi.abvc+fd->fire_v2soi.blwc;
   		burnretaincV->put_rec(&ftmp1, tstepcnt);
   		double ftmp2 = fd->fire_v2soi.abvn+fd->fire_v2soi.blwn;
   		burnretainnV->put_rec(&ftmp2, tstepcnt);


 		// veg C/N state variables for each PFT
 		callV->set_cur(tstepcnt);
 		callV->put(&bgcod->y_vegs.call, 1);

 		cV->set_cur(tstepcnt, 0);
 		cV->put(&bgcod->y_vegs.c[0], 1, NUM_PFT_PART);

 		nallV->set_cur(tstepcnt);
 		nallV->put(&bgcod->y_vegs.nall, 1);

 		labnV->set_cur(tstepcnt);
 		labnV->put(&bgcod->y_vegs.labn, 1);

 		strnallV->set_cur(tstepcnt);
 		strnallV->put(&bgcod->y_vegs.strnall, 1);

 		strnV->set_cur(tstepcnt, 0);
 		strnV->put(&bgcod->y_vegs.strn[0], 1, NUM_PFT_PART);

 		deadcV->set_cur(tstepcnt);
 		deadcV->put(&bgcod->y_vegs.deadc, 1);

 		callV->set_cur(tstepcnt);
 		deadnV->put(&bgcod->y_vegs.deadn, 1);

 		//veg C/N fluxes for each PFT

 		gppftV->set_cur(tstepcnt);
 		gppftV->put(&bgcod->y_vegd.ftemp, 1);

 		gppgvV->set_cur(tstepcnt);
 		gppgvV->put(&bgcod->y_vegd.gv, 1);

 		gppfnaV->set_cur(tstepcnt);
 		gppfnaV->put(&bgcod->y_vegd.fna, 1);

 		gppfcaV->set_cur(tstepcnt);
 		gppfcaV->put(&bgcod->y_vegd.fca, 1);

 		raq10V->set_cur(tstepcnt);
 		raq10V->put(&bgcod->y_vegd.raq10, 1);

 		rmkrV->set_cur(tstepcnt, 0);
 		rmkrV->put(&bgcod->y_vegd.kr[0], 1, NUM_PFT_PART);

 		ingppallV->set_cur(tstepcnt);
 		ingppallV->put(&bgcod->y_a2v.ingppall, 1);

 		ingppV->set_cur(tstepcnt, 0);
 		ingppV->put(&bgcod->y_a2v.ingpp[0], 1, NUM_PFT_PART);

 		innppallV->set_cur(tstepcnt);
 		innppallV->put(&bgcod->y_a2v.innppall, 1);

 		innppV->set_cur(tstepcnt, 0);
 		innppV->put(&bgcod->y_a2v.innpp[0], 1, NUM_PFT_PART);

 		gppallV->set_cur(tstepcnt);
 		gppallV->put(&bgcod->y_a2v.gppall, 1);

 		gppV->set_cur(tstepcnt, 0);
 		gppV->put(&bgcod->y_a2v.gpp[0], 1, NUM_PFT_PART);

 		nppallV->set_cur(tstepcnt);
 		nppallV->put(&bgcod->y_a2v.nppall, 1);

 		nppV->set_cur(tstepcnt, 0);
 		nppV->put(&bgcod->y_a2v.npp[0], 1, NUM_PFT_PART);

 		rmallV->set_cur(tstepcnt);
 		rmallV->put(&bgcod->y_v2a.rmall, 1);

 		rmV->set_cur(tstepcnt, 0);
 		rmV->put(&bgcod->y_v2a.rm[0], 1, NUM_PFT_PART);

 		rgallV->set_cur(tstepcnt);
 		rgallV->put(&bgcod->y_v2a.rgall, 1);

 		rgV->set_cur(tstepcnt, 0);
 		rgV->put(&bgcod->y_v2a.rg[0], 1, NUM_PFT_PART);

 		ltrfalcallV->set_cur(tstepcnt);
 		ltrfalcallV->put(&bgcod->y_v2soi.ltrfalcall, 1);

 		ltrfalcV->set_cur(tstepcnt, 0);
 		ltrfalcV->put(&bgcod->y_v2soi.ltrfalc[0], 1, NUM_PFT_PART);

 		ltrfalnallV->set_cur(tstepcnt);
 		ltrfalnallV->put(&bgcod->y_v2soi.ltrfalnall, 1);

 		ltrfalnV->set_cur(tstepcnt, 0);
 		ltrfalnV->put(&bgcod->y_v2soi.ltrfaln[0], 1, NUM_PFT_PART);

 		innuptakeV->set_cur(tstepcnt);
 		innuptakeV->put(&bgcod->y_soi2v.innuptake, 1);

 		nrootextractV->set_cur(tstepcnt, 0);
 		nrootextractV->put(&bgcod->y_soi2v.nextract[0], 1, MAX_SOI_LAY);

 		luptakeV->set_cur(tstepcnt);
 		luptakeV->put(&bgcod->y_soi2v.lnuptake, 1);

 		suptakeallV->set_cur(tstepcnt);
 		suptakeallV->put(&bgcod->y_soi2v.snuptakeall, 1);

 		suptakeV->set_cur(tstepcnt);
 		suptakeV->put(&bgcod->y_soi2v.snuptake[0],1, NUM_PFT_PART);

 		nmobilallV->set_cur(tstepcnt);
 		nmobilallV->put(&bgcod->y_v2v.nmobilall,1);

 		nmobilV->set_cur(tstepcnt);
 		nmobilV->put(&bgcod->y_v2v.nmobil[0],1, NUM_PFT_PART);

 		nresorballV->set_cur(tstepcnt);
 		nresorballV->put(&bgcod->y_v2v.nresorball,1);

 		nresorbV->set_cur(tstepcnt);
 		nresorbV->put(&bgcod->y_v2v.nresorb[0],1, NUM_PFT_PART);

}
