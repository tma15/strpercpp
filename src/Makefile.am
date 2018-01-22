
lib_LTLIBRARIES = libstrpercpp.la
libstrpercpp_la_SOURCES = \
	dictionary.hpp dictionary.cpp \
	corpus.hpp corpus.cpp \
	node.hpp node.cpp \
	viterbi.hpp viterbi.cpp \
	matrix.hpp matrix.cpp \
	feature_template.hpp feature_template.cpp \
	lattice.hpp lattice.cpp \
	util.hpp util.cpp \
	structured_perceptron.hpp structured_perceptron.cpp

pkginclude_HEADERS = \
	argparse.h \
	corpus.hpp \
	dictionary.hpp \
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

test_strpercpp_SOURCES = test_strpercpp.cpp
test_strpercpp_LDADD = libstrpercpp.la

AM_CXXFLAGS = -std=c++0x -O3
