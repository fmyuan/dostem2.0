# Basic dos-tem Makefile

MPI_PATH=/projects/cesm/devtools/mpich-3.0.4-gcc4.8.1
CXX=$(MPI_PATH)/bin/mpic++
CXXFLAGS= -c -O0 -g3 -Wall -fmessage-length=0 -fpermissive -fPIC
NETCDF_ROOT=/projects/cesm/devtools/netcdf-4.1.3-gcc4.8.1-mpich3.0.4
LDFLAGS=-L$(NETCDF_ROOT)/lib64 -lnetcdf_c++ -lnetcdf -L$(MPI_PATH)/lib
INCLUDES=-I$(NETCDF_ROOT)/include -I$(MPI_PATH)/include

SOURCES= 	src/TEM.o \
		src/assembler/RunCohort.o \
		src/assembler/RunGrid.o \
		src/assembler/Runner.o \
		src/assembler/RunRegion.o \
		src/atmosphere/Atmosphere.o \
		src/atmosphere/AtmosUtil.o \
		src/data/BgcData.o \
		src/data/CohortData.o \
		src/data/EnvData.o \
		src/data/EnvDataDly.o \
		src/data/FirData.o \
		src/data/GridData.o \
		src/data/OutDataRegn.o \
		src/data/RegionData.o \
		src/data/RestartData.o \
		src/disturb/WildFire.o \
		src/ecodomain/DoubleLinkedList.o \
		src/ecodomain/Ground.o \
		src/ecodomain/horizon/Mineral.o \
		src/ecodomain/horizon/Moss.o \
		src/ecodomain/horizon/Organic.o \
		src/ecodomain/horizon/Snow.o \
		src/ecodomain/horizon/SoilParent.o \
		src/ecodomain/Vegetation.o \
		src/input/CohortInputer.o \
		src/input/GridInputer.o \
		src/input/RegionInputer.o \
		src/input/RestartInputer.o \
		src/lookup/CohortLookup.o \
		src/lookup/SoilLookup.o \
		src/output/BgcOutputer.o \
		src/output/ChtOutputer.o \
		src/output/EnvOutputer.o \
		src/output/RegnOutputer.o \
		src/output/RestartOutputer.o \
		src/runmodule/Cohort.o \
		src/runmodule/Controller.o \
		src/runmodule/Grid.o \
		src/runmodule/Integrator.o \
		src/runmodule/ModelData.o \
		src/runmodule/OutRetrive.o \
		src/runmodule/Region.o \
		src/runmodule/Timer.o \
		src/snowsoil/Richards.o \
		src/snowsoil/Snow_Env.o \
		src/snowsoil/Soil_Bgc.o \
		src/snowsoil/Soil_Env.o \
		src/snowsoil/SoilParent_Env.o \
		src/snowsoil/Stefan.o \
		src/snowsoil/TemperatureUpdator.o \
		src/util/CrankNicholson.o \
		src/util/Interpolator.o \
		src/vegetation/Vegetation_Bgc.o \
		src/vegetation/Vegetation_Env.o \
		src/ecodomain/layer/Layer.o \
		src/ecodomain/layer/MineralLayer.o \
		src/ecodomain/layer/MossLayer.o \
		src/ecodomain/layer/OrganicLayer.o \
		src/ecodomain/layer/ParentLayer.o \
		src/ecodomain/layer/SnowLayer.o \
		src/ecodomain/layer/SoilLayer.o
OBJECTS =	RunCohort.o \
		RunGrid.o \
		Runner.o \
		RunRegion.o \
		Atmosphere.o \
		AtmosUtil.o \
		BgcData.o \
		CohortData.o \
		EnvData.o \
		EnvDataDly.o \
		FirData.o \
		GridData.o \
		OutDataRegn.o \
		RegionData.o \
		RestartData.o \
		WildFire.o \
		DoubleLinkedList.o \
		Ground.o \
		Mineral.o \
		Moss.o \
		Organic.o \
		Snow.o \
		SoilParent.o \
		Vegetation.o \
		CohortInputer.o \
		GridInputer.o \
		RegionInputer.o \
		RestartInputer.o \
		CohortLookup.o \
		SoilLookup.o \
		BgcOutputer.o \
		ChtOutputer.o \
		EnvOutputer.o \
		RegnOutputer.o \
		RestartOutputer.o \
		Cohort.o \
		Controller.o \
		Grid.o \
		Integrator.o \
		ModelData.o \
		OutRetrive.o \
		Region.o \
		Timer.o \
		Richards.o \
		Snow_Env.o \
		Soil_Bgc.o \
		Soil_Env.o \
		SoilParent_Env.o \
		Stefan.o \
		CrankNicholson.o \
		Interpolator.o \
		Vegetation_Bgc.o \
		Vegetation_Env.o \
		Layer.o \
		MineralLayer.o \
		MossLayer.o \
		OrganicLayer.o \
		ParentLayer.o \
		SnowLayer.o \
		SoilLayer.o \
		TemperatureUpdator.o 
TEMOBJ=	TEM.o


tem: $(SOURCES) $(TEMOBJ)
	$(CXX) -o DOSTEM $(OBJECTS) $(TEMOBJ) $(LDFLAGS)

lib: $(SOURCES) 
	$(CXX) -o libTEM.so -shared $(INCLUDES) $(OBJECTS) $(LDFLAGS)

.cpp.o:  
	$(CXX) $(INCLUDES) $(CXXFLAGS) $<

clean:
	rm -f $(OBJECTS) DOSTEM TEM.o libTEM.so* *~
