PROG_ROOT = ../..

AM_CFLAGS = -g -DTEST_FUNCTION -I${PROG_ROOT}/ -I${PROG_ROOT}/${PBS_MACH} --coverage

lib_LTLIBRARIES = libenc_QueueJob.la

AM_LDFLAGS = @CHECK_LIBS@ ${lib_LTLIBRARIES}

check_PROGRAMS = test_enc_QueueJob

libenc_QueueJob_la_SOURCES = scaffolding.c ${PROG_ROOT}/enc_QueueJob.c
libenc_QueueJob_la_LDFLAGS = @CHECK_LIBS@ -shared -lgcov

test_enc_QueueJob_SOURCES = test_enc_QueueJob.c

check_SCRIPTS = coverage_run.sh

TESTS = ${check_PROGRAMS} coverage_run.sh

coverage_run.sh:
	echo 'cp -p .libs/enc_QueueJob.gc* . >/dev/null 2>&1' > $@
	echo 'RESULTS=($$(gcov enc_QueueJob.gcda))' >> $@
	echo 'PARSED_RESULT="TOTALCOV -- $${RESULTS[1]}: Lines($${RESULTS[5]})- $${RESULTS[3]}"' >> $@
	echo 'echo -e "\033[40m\033[1;33m$$PARSED_RESULT\033[0m"' >> $@
	chmod +x $@

CLEANFILES = coverage_run.sh *.gcno *.gcda *.gcov core *.lo
