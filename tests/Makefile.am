if HAS_GTEST
check_PROGRAMS = strpercpp_test
strpercpp_test_SOURCES = \
			 test_harness.cc \
			 test_argparse.cc \
			 test_model.cc

strpercpp_test_LDADD = -lgtest -L@GTEST_PATH@/lib -lstrpercpp -L${top_builddir}/src
strpercpp_test_CPPFLAGS = -I@GTEST_PATH@/include

endif
