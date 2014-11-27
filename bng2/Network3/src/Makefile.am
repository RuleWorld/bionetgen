# tell automake to put objects in srcdir
AUTOMAKE_OPTIONS = subdir-objects

# define library source directories
SUNDIALS_DIR  = ../cvode-2.6.0
MUPARSER_DIR  = ../muparser_v2_2_3
MATHUTILS_DIR = util/mathutils

# this is the program we're building
bin_PROGRAMS = run_network

# mathutils convenience library
noinst_LTLIBRARIES = libmathutils.la

# sources for mathutils library
libmathutils_la_SOURCES = util/mathutils/allocate.c util/mathutils/cabs.c util/mathutils/cadd.c util/mathutils/cconj.c util/mathutils/cmult.c util/mathutils/complex_zero.c util/mathutils/copy_vector.c util/mathutils/cprob.c util/mathutils/csub.c util/mathutils/dotp.c util/mathutils/eigensystem.c util/mathutils/expi.c util/mathutils/gram_schmidt.c util/mathutils/herm_matrix_mult.c util/mathutils/herm_matrix_vector_mult.c util/mathutils/icmult.c util/mathutils/init.c util/mathutils/integration.c util/mathutils/linsolve.c util/mathutils/matherr.c util/mathutils/mathutils.h util/mathutils/matrix_add.c util/mathutils/matrix_elt.c util/mathutils/matrix_mult.c util/mathutils/matrix_sub.c util/mathutils/matrix_transpose.c util/mathutils/matrix_vector_mult.c util/mathutils/matrixexp.c util/mathutils/minimum.c util/mathutils/norm.c util/mathutils/normsq.c util/mathutils/project.c util/mathutils/random.c util/mathutils/rcmult.c util/mathutils/rect_transpose.c util/mathutils/roots.c util/mathutils/runge-kutta.c util/mathutils/scale_vector.c util/mathutils/splines.c util/mathutils/sum_vector.c util/mathutils/svd.c util/mathutils/sym_linsolve.c util/mathutils/sym_matrix_mult.c util/mathutils/sym_matrix_vector_mult.c util/mathutils/sympos_linsolve.c util/mathutils/test_transpose.c util/mathutils/testdummy.c util/mathutils/testgram.c util/mathutils/testspline.c util/mathutils/testsvd.c util/mathutils/testutils.c util/mathutils/vector_add.c util/mathutils/vector_dist.c util/mathutils/zcopy_vector.c util/mathutils/zdotc.c util/mathutils/zdotu.c util/mathutils/zmatrix_add.c util/mathutils/zmatrix_mult.c util/mathutils/zmatrix_sub.c util/mathutils/zmatrix_transpose.c util/mathutils/zmatrix_vector_mult.c util/mathutils/znorm.c util/mathutils/znormsq.c util/mathutils/zrscale_vector.c util/mathutils/zscale_vector.c util/mathutils/zsum_vector.c util/mathutils/zsym_linsolve.c

# where to look for include files
run_network_CPPFLAGS = -I${includedir} -I${MUPARSER_DIR}/include -I$(SUNDIALS_DIR)/include -I$(MATHUTILS_DIR)

# sources for run_network (add any new source files and headers here)
run_network_SOURCES = network3.cpp network.cpp run_network.cpp model/function.cpp model/observable.cpp model/rateExpression.cpp model/reaction.cpp model/simpleSpecies.cpp model/rateExpressions/rateElementary.cpp model/rateExpressions/rateHill.cpp model/rateExpressions/rateMM.cpp model/rateExpressions/rateMuParser.cpp model/rateExpressions/rateSaturation.cpp model/reactions/bioNetGenRxn.cpp model/reactions/elementaryRxn.cpp model/reactions/functionalRxn.cpp model/reactions/hillRxn.cpp model/reactions/michaelisMentenRxn.cpp model/reactions/saturationRxn.cpp pla/PLA.cpp pla/base/firingGenerator.cpp pla/base/postleapChecker.cpp pla/base/rxnClassifier.cpp pla/base/tauCalculator.cpp pla/eRungeKutta/eRungeKutta_postTC_RC_FG_rbPL.cpp pla/eRungeKutta/eRungeKutta_postTC_RC_FG_sbPL.cpp pla/eRungeKutta/eRungeKutta_preTC_RC_FG_negPL.cpp pla/eRungeKutta/eRungeKutta_preTC_RC_FG_rbPL.cpp pla/eRungeKutta/eRungeKutta_preTC_RC_FG_sbPL.cpp pla/eRungeKutta/base/eRungeKutta_TC_RC_FG_PL.cpp pla/eRungeKutta/base/eRungeKutta_TC_RC_FG_rbPL.cpp pla/eRungeKutta/base/eRungeKutta_TC_RC_FG_sbPL.cpp pla/eRungeKutta/util/aEff_Calculator.cpp pla/eRungeKutta/util/binomialCorrector_RK.cpp pla/eRungeKutta/util/butcherTableau.cpp pla/fEuler/fEuler_FG.cpp pla/fEuler/fEulerPreleapRB_TC.cpp pla/fEuler/fEulerPreleapSB_TC.cpp pla/fEuler/fEulerRB_PL.cpp pla/fEuler/fEulerRB_TC_PL.cpp pla/fEuler/fEuler_RC.cpp pla/fEuler/fEulerSB_PL.cpp pla/fEuler/fEulerSB_TC_PL.cpp pla/util/g_Getter.cpp pla/util/negPopChecker.cpp pla/util/preleap_TC.cpp pla/util/rbChecker.cpp pla/util/sbChecker.cpp util/conversion.cpp util/rand.cpp util/MTrand/mtrand.cpp util/rand2/rand2.cpp util/misc.cpp

# link to these static libraries
run_network_LDADD = libmathutils.la ${MUPARSER_DIR}/lib/libmuparser.a ${SUNDIALS_DIR}/src/nvec_ser/libsundials_nvecserial.la ${SUNDIALS_DIR}/src/cvode/libsundials_cvode.la

