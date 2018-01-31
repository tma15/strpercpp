
lib_LTLIBRARIES = libstrpercpp.la
libstrpercpp_la_SOURCES = \
	dictionary.hpp dictionary.cpp \
	early_update_perceptron.hpp early_update_perceptron.cpp \
	corpus.hpp corpus.cpp \
	node.hpp node.cpp \
	viterbi.hpp viterbi.cpp \
	matrix.hpp matrix.cpp \
	feature_template.hpp feature_template.cpp \
	lattice.hpp lattice.cpp \
	util.hpp util.cpp \
	structured_perceptron.hpp structured_perceptron.cpp

if HAS_GPERF
libstrpercpp_la_LDFLAGS = -g -lprofiler -L@GPERF_PATH@/lib
else
libstrpercpp_la_LDFLAGS =
endif

pkginclude_HEADERS = \
	argparse.h \
	corpus.hpp \
	dictionary.hpp \
	early_update_perceptron.hpp \
	feature_template.hpp \
	lattice.hpp \
	matrix.hpp \
	node.hpp \
	structured_perceptron.hpp \
	util.hpp \
	viterbi.hpp 

bin_PROGRAMS = train_strpercpp test_strpercpp
train_strpercpp_SOURCES = train_strpercpp.cpp
train_strpercpp_LDADD = libstrpercpp.la

if HAS_GPERF
train_strpercpp_LDFLAGS = -lprofiler -L@GPERF_PATH@/lib
train_strpercpp_CXXFLAGS = -I@GPERF_PATH@/include 
else
train_strpercpp_LDFLAGS = 
train_strpercpp_CXXFLAGS = 
endif

test_strpercpp_SOURCES = test_strpercpp.cpp
test_strpercpp_LDADD = libstrpercpp.la

AM_CXXFLAGS = -std=c++0x -O3
