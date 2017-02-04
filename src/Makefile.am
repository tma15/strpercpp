
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
# libstrpercpp_CPPFLAGS = -std=c++0x -O3

bin_PROGRAMS = train_strpercpp test_strpercpp
train_strpercpp_SOURCES = train_strpercpp.cpp
train_strpercpp_LDADD = libstrpercpp.la

test_strpercpp_SOURCES = test_strpercpp.cpp
test_strpercpp_LDADD = libstrpercpp.la

# train_strpercpp_CPPFLAGS = -std=c++0x -O3

# AM_CXXFLAGS = -g -pg
AM_CXXFLAGS = -std=c++0x -O3
