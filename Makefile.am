bin_PROGRAMS = abc
abc_SOURCES = rng.cpp \
		         configuration.cpp \
			 foodSource.cpp \
			 problem.cpp \
			 numsup.cc \
			 localSearch.cpp \
			 mtsls1.cpp \
			 powell.cpp \
			 ABC.cpp \
			 originalABC.cpp \
			 chaoticmaps.cpp \
			 utils.cpp \
			 icmaesils.cc\
			 cmaes.c\
			 main.cpp 

# local linux :
abc_CPPFLAGS = -I. -I tclap-1.2.0/include # linux için geçerli satır
abc_LDADD = -lm -lgsl -lgslcblas # linux için geçerli satır

# truba : 

# abc_CPPFLAGS = -I. -I tclap-1.2.0/include -I/truba/home/gyavuz/bin/gsl/include
# abc_LDADD = -lm -L/truba/home/gyavuz/bin/gsl/lib -lgsl -lgslcblas

# mac :
#abc_CPPFLAGS = -I. -I tclap-1.2.0/include -I/usr/local/Cellar/gsl/1.16/include # mac için geçerli satır
#abc_LDADD = -lm  -L/usr/local/Cellar/gsl/1.16/lib -lgsl -lgslcblas # mac için geçerli satır
