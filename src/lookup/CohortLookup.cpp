
#include "CohortLookup.h"

CohortLookup::CohortLookup(){
	cmtcode = "CMT00";     // the default community code (5 alphnumerics)
};

CohortLookup::~CohortLookup(){

};

void CohortLookup::init(){
	assignBgcCalpar(dir);

	assignDimension(dir);

	assignEnv(dir);

	assignBgc(dir);

	assignFirePar(dir);
  
};

void CohortLookup::assignBgcCalpar(string & dircmt){

	string parfilecal = dircmt+"cmt_calparbgc.txt";
	ifstream fctrcomm;
	fctrcomm.open(parfilecal.c_str(),ios::in );
	bool isOpen = fctrcomm.is_open();
	if ( !isOpen ){
  		cout << "\nCannot open " << parfilecal << "  \n" ;
  		exit( -1 );
	}

	string str;
	string code;
	int lines = 21;   // total lines of one block of community data/info, except for 2 header lines

	getline(fctrcomm, str);     // community separation line ("//====" or something or empty line)
	getline(fctrcomm, str);     // community code - 'CMTxx' (xx: two digits)
	int pos=str.find("CMT");
	code = str.substr(pos, 5);
	while (code.compare(cmtcode)!=0) {
		for (int il=0; il<lines; il++) getline(fctrcomm, str);   //skip lines
		if (fctrcomm.eof()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecal << "  \n" ;
	  		exit( -1 );
		}

		getline(fctrcomm, str);     // community separation line ("//====" or something or empty line)
		getline(fctrcomm, str);     // community code - 'CMTxx' (xx: two digits)
		if (str.empty()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecal << "  \n" ;
	  		exit( -1 );
		}
		pos=str.find("CMT");
		code = str.substr(pos, 5);
	}

	getline(fctrcomm, str);     //comments in the file
	fctrcomm >> cmax;  getline(fctrcomm, str);
	fctrcomm >> nmax;  getline(fctrcomm, str);

	fctrcomm >> cfall[I_leaf]; getline(fctrcomm, str);
	fctrcomm >> cfall[I_stem]; getline(fctrcomm, str);
	fctrcomm >> cfall[I_root]; getline(fctrcomm, str);

	fctrcomm >> nfall[I_leaf];  getline(fctrcomm, str);
	fctrcomm >> nfall[I_stem];  getline(fctrcomm, str);
	fctrcomm >> nfall[I_root];  getline(fctrcomm, str);

	fctrcomm >> kra; getline(fctrcomm, str);
	fctrcomm >> krb[I_leaf]; getline(fctrcomm, str);
	fctrcomm >> krb[I_stem]; getline(fctrcomm, str);
	fctrcomm >> krb[I_root]; getline(fctrcomm, str);

	fctrcomm >> frg; getline(fctrcomm, str);

	// soil bgc Calibrated parameters
	getline(fctrcomm, str);     //comments in the file
	fctrcomm >> micbnup; getline(fctrcomm, str);
	fctrcomm >> kdcmoss; getline(fctrcomm, str);
	fctrcomm >> kdcrawc; getline(fctrcomm, str);
	fctrcomm >> kdcsoma; getline(fctrcomm, str);
	fctrcomm >> kdcsompr; getline(fctrcomm, str);
	fctrcomm >> kdcsomcr; getline(fctrcomm, str);

	fctrcomm.close();

};

void CohortLookup::assignDimension(string &dircmt){

	string parfilecomm = dircmt+"cmt_dimension.txt";
	ifstream fctrpft;
	fctrpft.open(parfilecomm.c_str(),ios::in );
	bool isOpen = fctrpft.is_open();
	if ( !isOpen ) {
  		cout << "\nCannot open " << parfilecomm << "  \n" ;
  		exit( -1 );
	}

	string str;
	string code;
	int lines = 61;   // total lines of one block of community data/info, except for 2 header lines

	getline(fctrpft, str);     // community separation line ("//====" or something or empty line)
	getline(fctrpft, str);     // community code - 'CMTxx' (xx: two digits)
	int pos=str.find("CMT");
	code = str.substr(pos, 5);
	while (code.compare(cmtcode)!=0) {
		for (int il=0; il<lines; il++) getline(fctrpft, str);   //skip lines
		if (fctrpft.eof()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}

		getline(fctrpft, str);     // community separation line ("//====" or something or empty line)
		getline(fctrpft, str);     // community code - 'CMTxx' (xx: two digits)
		if (str.empty()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}
		pos=str.find("CMT");
		code = str.substr(pos, 5);
	}

	getline(fctrpft,str);     //read comments
	fctrpft >> vegcov;
	getline(fctrpft,str);     // read comments

	fctrpft >> ifwoody;
	getline(fctrpft,str);     // read comments

	fctrpft >> ifdeciwoody;
	getline(fctrpft,str);     // read comments

	fctrpft >> ifperenial;
	getline(fctrpft,str);     // read comments
	fctrpft >> nonvascular;
	getline(fctrpft,str);     // read comments

	fctrpft >> sla;
	getline(fctrpft,str);     // read comments

	fctrpft >> klai;
	getline(fctrpft,str);     // read comments

	fctrpft >> minleaf;
	getline(fctrpft,str);
	fctrpft >> aleaf;
	getline(fctrpft,str);
	fctrpft >> bleaf;
	getline(fctrpft,str);
	fctrpft >> cleaf;
	getline(fctrpft,str);

	fctrpft >> kfoliage;
	getline(fctrpft,str);
	fctrpft >> cov;
	getline(fctrpft,str);
	fctrpft >> m1;
	getline(fctrpft,str);
	fctrpft >> m2;
	getline(fctrpft,str);
	fctrpft >> m3;
	getline(fctrpft,str);
	fctrpft >> m4;
	getline(fctrpft,str);

    for (int il =0; il<MAX_ROT_LAY; il++){
    	fctrpft >> frootfrac[il];
    	getline(fctrpft,str);     //comments in the file
    }

    fctrpft >> lai;
	getline(fctrpft,str);     // read comments

    for (int im =0; im<MINY; im++){
    	fctrpft >> envlai[im];
    	getline(fctrpft,str);     //comments in the file
    }

	//snow
	getline(fctrpft,str);     //comments in the file
	fctrpft >> snwdenmax; getline(fctrpft,str);     //comments in the file
	fctrpft >> snwdennew; getline(fctrpft,str);     //comments in the file
	fctrpft >> initsnwthick;  getline(fctrpft,str);     //comments in the file
	fctrpft >> initsnwdense;  getline(fctrpft,str);     //comments in the file

	//moss
	getline(fctrpft,str);     //comments in the file
	fctrpft >> maxdmossthick; getline(fctrpft,str);   //comments in the file
  	fctrpft >> initdmossthick; getline(fctrpft,str);  //comments in the file
  	fctrpft >> mosstype; getline(fctrpft,str);        //comments in the file
  	fctrpft >> coefmossa; getline(fctrpft,str);       //comments in the file
  	fctrpft >> coefmossb; getline(fctrpft,str);       //comments in the file

  	//soil
	getline(fctrpft,str);     //comments in the file
  	fctrpft >> initfibthick; getline(fctrpft,str);        //comments in the file
  	fctrpft >> inithumthick; getline(fctrpft,str);        //comments in the file
  	fctrpft >> coefshlwa; getline(fctrpft,str);       //comments in the file
  	fctrpft >> coefshlwb; getline(fctrpft,str);       //comments in the file
  	fctrpft >> coefdeepa; getline(fctrpft,str);       //comments in the file
  	fctrpft >> coefdeepb; getline(fctrpft,str);       //comments in the file
  	fctrpft >> coefminea; getline(fctrpft,str);       //comments in the file
  	fctrpft >> coefmineb; getline(fctrpft,str);       //comments in the file

  	// close the file
	fctrpft.close();

};

void CohortLookup::assignEnv(string &dir){

	string parfilecomm = dir+"cmt_env.txt";
	ifstream fctrpft;
	fctrpft.open(parfilecomm.c_str(),ios::in );
	bool isOpen = fctrpft.is_open();
	if ( !isOpen ) {
  		cout << "\nCannot open " << parfilecomm << "  \n" ;
  		exit( -1 );
	}

	string str;
	string code;
	int lines = 41;   // total lines of one block of community data/info, except for 2 header lines

	getline(fctrpft, str);     // community separation line ("//====" or something or empty line)
	getline(fctrpft, str);     // community code - 'CMTxx' (xx: two digits)
	int pos=str.find("CMT");
	code = str.substr(pos, 5);
	while (code.compare(cmtcode)!=0) {
		for (int il=0; il<lines; il++) getline(fctrpft, str);   //skip lines
		if (fctrpft.eof()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}

		getline(fctrpft, str);     // community separation line ("//====" or something or empty line)
		getline(fctrpft, str);     // community code - 'CMTxx' (xx: two digits)
		if (str.empty()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}
		pos=str.find("CMT");
		code = str.substr(pos, 5);
	}

	getline(fctrpft,str);     //PFT name/code comments in the file

	fctrpft >> albvisnir;
	getline(fctrpft,str);

	fctrpft >> er;
	getline(fctrpft,str);

	fctrpft >> ircoef;
	getline(fctrpft,str);
	fctrpft >> iscoef;
	getline(fctrpft,str);

	fctrpft >> glmax;
	getline(fctrpft,str);
	fctrpft >> gl_bl;
	getline(fctrpft,str);
	fctrpft >> gl_c;
	getline(fctrpft,str);

	fctrpft >> vpd_open;
	getline(fctrpft,str);
	fctrpft >> vpd_close;
	getline(fctrpft,str);
	fctrpft >> ppfd50;
	getline(fctrpft,str);

   	// initial values
	fctrpft >> initvegwater;
	getline(fctrpft,str);
	fctrpft >> initvegsnow;
	getline(fctrpft,str);

	//snow-ground-soil
	getline(fctrpft,str);     //ground-soil name comments in the file

		fctrpft >> snwalbmax; getline(fctrpft,str);     //comments in the file
		fctrpft >> snwalbmin; getline(fctrpft,str);     //comments in the file

		fctrpft >> psimax;  getline(fctrpft,str);     //comments in the file
		fctrpft >> evapmin;  getline(fctrpft,str);     //comments in the file
		fctrpft >> drainmax;  getline(fctrpft,str);     //comments in the file

	  	fctrpft >> rtdp4gdd; getline(fctrpft,str);     //comments in the file

		fctrpft >> initsnwtem;  getline(fctrpft,str);     //comments in the file

		for (int il=0; il<10; il++){
			fctrpft >> initts[il];
			getline(fctrpft,str);     //comments in the file
		}
		for (int il=0; il<10; il++){
			fctrpft >> initvwc[il];
			getline(fctrpft,str);     //comments in the file
		}

	//
	fctrpft.close();
};

void CohortLookup::assignBgc(string &dircmt){

	string parfilecomm = dircmt+"cmt_bgc.txt";

	ifstream fctrcomm;
	fctrcomm.open(parfilecomm.c_str(),ios::in );
	bool isOpen = fctrcomm.is_open();
	if ( !isOpen ){
  		cout << "\nCannot open " << parfilecomm << "  \n" ;
  		exit( -1 );
	}

	string str;
	string code;
	int lines = 54;   // total lines of one block of community data/info, except for 2 header lines

	getline(fctrcomm, str);     // community separation line ("//====" or something or empty line)
	getline(fctrcomm, str);     // community code - 'CMTxx' (xx: two digits)
	int pos=str.find("CMT");
	code = str.substr(pos, 5);
	while (code.compare(cmtcode)!=0) {
		for (int il=0; il<lines; il++) getline(fctrcomm, str);   //skip lines
		if (fctrcomm.eof()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}

		getline(fctrcomm, str);     // community separation line ("//====" or something or empty line)
		getline(fctrcomm, str);     // community code - 'CMTxx' (xx: two digits)
		if (str.empty()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}
		pos=str.find("CMT");
		code = str.substr(pos, 5);
	}

	// vegetation C/N parameters
	getline(fctrcomm,str);     //comments in the file

	fctrcomm >> kc;
	getline(fctrcomm,str);

	fctrcomm >> ki;
	getline(fctrcomm,str);

	fctrcomm >> tmin;
	getline(fctrcomm,str);
	fctrcomm >> toptmin;
	getline(fctrcomm,str);
	fctrcomm >> toptmax;
	getline(fctrcomm,str);
	fctrcomm >> tmax;
	getline(fctrcomm,str);

	fctrcomm >> raq10a0;
	getline(fctrcomm,str);
	fctrcomm >> raq10a1;
	getline(fctrcomm,str);
	fctrcomm >> raq10a2;
	getline(fctrcomm,str);
	fctrcomm >> raq10a3;
	getline(fctrcomm,str);

	fctrcomm >> knuptake;
	getline(fctrcomm,str);

	for (int i=0; i<NUM_PFT_PART; i++){
		fctrcomm >> cpart[i];
		getline(fctrcomm,str);
	}

	for (int i=0; i<NUM_PFT_PART; i++){
		fctrcomm >> initc2neven[i];
		getline(fctrcomm,str);
	}

	for (int i=0; i<NUM_PFT_PART; i++){
		fctrcomm >> c2nb[i];
		getline(fctrcomm,str);
	}

	for (int i=0; i<NUM_PFT_PART; i++){
		fctrcomm >> c2nmin[i];
		getline(fctrcomm,str);
	}

	fctrcomm >> c2na;
	getline(fctrcomm,str);

	fctrcomm >> labncon;
	getline(fctrcomm,str);

   	// initial values
   	for (int i=0; i<NUM_PFT_PART; i++){
		fctrcomm >> initvegc[i];
		getline(fctrcomm,str);
   	}
	for (int i=0; i<NUM_PFT_PART; i++){
		fctrcomm >> initvegn[i];
		getline(fctrcomm,str);
	}

	fctrcomm >> initdeadc;
	getline(fctrcomm,str);
	fctrcomm >> initdeadn;
	getline(fctrcomm,str);

	// soil bgc
	getline(fctrcomm, str);

	fctrcomm >> rhq10; getline(fctrcomm,str);
	fctrcomm >> moistmin; getline(fctrcomm,str);
	fctrcomm >> moistopt; getline(fctrcomm,str);
	fctrcomm >> moistmax; getline(fctrcomm,str);

	fctrcomm >> lcclnc; getline(fctrcomm,str);

	fctrcomm >> fsoma; getline(fctrcomm,str);
	fctrcomm >> fsompr; getline(fctrcomm,str);
	fctrcomm >> fsomcr; getline(fctrcomm,str);
	fctrcomm >> som2co2; getline(fctrcomm,str);

	fctrcomm >> kn2;   getline(fctrcomm,str);
	fctrcomm >> nmincnsoil; getline(fctrcomm,str);
	fctrcomm >> propftos; getline(fctrcomm,str);

	fctrcomm >> fnloss; getline(fctrcomm,str);

	fctrcomm >> initdmossc; getline(fctrcomm,str);
	fctrcomm >> initshlwc; getline(fctrcomm,str);
	fctrcomm >> initdeepc; getline(fctrcomm,str);
	fctrcomm >> initminec; getline(fctrcomm,str);
	fctrcomm >> initsoln; getline(fctrcomm,str);
	fctrcomm >> initavln; getline(fctrcomm,str);

	//
	fctrcomm.close();

};


void CohortLookup::assignFirePar(string &dircmt){

	string parfilecomm = dircmt+"cmt_firepar.txt";
	ifstream fctrcomm;
	fctrcomm.open(parfilecomm.c_str(),ios::in );
	bool isOpen = fctrcomm.is_open();
	if ( !isOpen ) {
  		cout << "\nCannot open " << parfilecomm << "  \n" ;
  		exit( -1 );
	}

	string str;
	string code;
	int lines = 21;   // total lines of one block of community data/info, except for 2 header lines

	getline(fctrcomm, str);     // community separation line ("//====" or something or empty line)
	getline(fctrcomm, str);     // community code - 'CMTxx' (xx: two digits)
	int pos=str.find("CMT");
	code = str.substr(pos, 5);
	while (code.compare(cmtcode)!=0) {
		for (int il=0; il<lines; il++) getline(fctrcomm, str);   //skip lines
		if (fctrcomm.eof()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}

		getline(fctrcomm, str);     // community separation line ("//====" or something or empty line)
		getline(fctrcomm, str);     // community code - 'CMTxx' (xx: two digits)
		if (str.empty()) {
	  		cout << "Cannot find community type: " << cmtcode << " in file: " <<parfilecomm << "  \n" ;
	  		exit( -1 );
		}
		pos=str.find("CMT");
		code = str.substr(pos, 5);
	}

	getline(fctrcomm,str);     // PFT code/name comments in the file

	for(int i=0; i<NUM_FSEVR; i++){
		fctrcomm >> fvcombust[i];
		getline(fctrcomm,str);
	}

	for(int i=0; i<NUM_FSEVR; i++){
		fctrcomm >> fvslash[i];
		getline(fctrcomm,str);
	}

	getline(fctrcomm,str);     //comments in the file
	for(int i=0; i<NUM_FSEVR; i++) {
		fctrcomm >> foslburn[i];
		getline(fctrcomm,str);
	}

	getline(fctrcomm,str);     //comments in the file
	fctrcomm >> vsmburn;      getline(fctrcomm,str);
  	fctrcomm >> r_retain_c;   getline(fctrcomm,str);
  	fctrcomm >> r_retain_n;   getline(fctrcomm,str);

	fctrcomm.close();

};
