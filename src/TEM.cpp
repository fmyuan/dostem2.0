/**
 *  TEM.cpp
 *  main program for running DOS-TEM version 2
 *  
 *  It runs at 3 run-mods:
 *      (1) site-specific
 *      (2) regional - time series
 * 		(3) regional - spatially (not yet available)
 * 
 * Authors: Shuhua Yi - the original codes
 * 		    Fengming Yuan - re-designing and re-coding for (1) easily code managing;
 *                                        (2) java interface developing for calibration;
 *                                        (3) stand-alone application of TEM (java-c++)
 *                                        (4) inputs/outputs using netcdf format, have to be modified
 *                                        to fix memory-leaks
 *                                        (5) fix the snow/soil thermal/hydraulic algorithms
 *                                        (6) DVM coupled, but with 1 vegetation type
 *
 * Affilation:
               Climate Change Science Institute and Environmental Science Division
               Oak Ridge National Laboratory
               and,
               Spatial Ecological Laboratory
               Institute of Arctic Institute
               University of Alaska Fairbanks

 * started: 11/01/2010
 * last modified: 03/28/2013

 * Copyright   : August 2013
 * Description : DOS-TEM version 2 in MPI C++
 ============================================================================
 */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <exception>

using namespace std;

#include "assembler/Runner.h"
#include <mpi.h>

// defines the mode of run: Single-site or Multiple-site (regional)
//#define SITERUN
#define REGNRUN

int main(int argc, char* argv[]){

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	#ifdef SITERUN 
		time_t stime;
		time_t etime;
		stime=time(0);
		cout<<"run TEM stand-alone - start @"<<ctime(&stime)<<"\n";

		string controlfile="";
		string chtid = "1";    /* default chtid 1 for siter-runmode  */
		if(argc == 1){   //if there is no control file specified
			controlfile ="config/controlfile_site.txt";
		} else if(argc == 2) { // if only control file specified
			controlfile = argv[1];
		} else if(argc == 3) { // both control file and chtid specified in the order
			controlfile = argv[1];
			chtid = argv[2];
		}

		Runner siter;

		siter.chtid = atoi(chtid.c_str());

		siter.initInput(controlfile, "siter");

		siter.initOutput();

 		siter.setupData();

 		siter.setupIDs();

 		siter.runmode1();
 
 		etime=time(0);
		cout <<"run TEM stand-alone - done @"<<ctime(&etime)<<"\n";
		cout <<"total seconds: "<<difftime(etime, stime)<<"\n";
	#endif

	#ifdef REGNRUN

		MPI::Init();
		int noprocs = MPI::COMM_WORLD.Get_size();
		int nthproc = MPI::COMM_WORLD.Get_rank();

		time_t stime;
		time_t etime;
		stime=time(0);

		if (nthproc == 0)
		cout <<"run TEM regionally - start @"<<ctime(&stime)<<"\n";

		string controlfile="";
		string runmode = "regner1";
		if(argc == 1){ //if there is no control file specified
			controlfile ="config/controlfile_regn.txt";
		} else if(argc == 2) {
			controlfile = argv[1];
		} else if (argc == 3) {   // both control file and runmode specified in order
			controlfile = argv[1];
			runmode     = argv[2];
		}

		Runner regner;

		regner.initInput(controlfile, runmode, nthproc);

		regner.initOutput(nthproc);

		regner.setupData();

		regner.setupIDs(noprocs);

 		if (runmode.compare("regner1")==0) {
 			regner.runmode2(nthproc);
 		} else if (runmode.compare("regner2")==0){
 			regner.runmode3(nthproc);
		} else {
			cout <<"run-mode for TEM regional run on Process: "<<nthproc <<", must be: \n";
			cout <<" EITHER 'regner1' OR 'regner2' \n";
			exit(-1);
		}

 		if (nthproc == 0) {
 			etime=time(0);
 			cout <<"run TEM regionally - done @"<<ctime(&etime)<<"\n";
 			cout <<"total seconds: "<<difftime(etime, stime)<<"\n";
 		}

		MPI::Finalize();

	#endif

	return 0;

};
