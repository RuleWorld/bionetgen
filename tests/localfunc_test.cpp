#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// Define realtype explicitly for Sundials v7 compatibility
#include <sundials/sundials_types.h>
typedef sunrealtype realtype;

// Include the mock headers
#include "mex.h"
#include "matrix.h"

// Provide mock implementations
extern "C" {
    double* mxGetPr(const mxArray *pm) { return nullptr; }
    size_t mxGetM(const mxArray *pm) { return 0; }
    size_t mxGetN(const mxArray *pm) { return 0; }
    mxArray* mxCreateDoubleMatrix(size_t m, size_t n, int ComplexFlag) { return nullptr; }
}

// Dummy definitions for missing SUNDIALS V2/V3 macros
#define CVDense(...) 0
#define CVSpgmr(...) 0

// Need to match Sundials values for compiling the older mex code
#ifndef CV_NEWTON
#define CV_NEWTON 0
#endif

#ifndef CV_BDF
#define CV_BDF 2
#endif

#ifndef CV_NORMAL
#define CV_NORMAL 1
#endif

#include <nvector/nvector_serial.h>
#include <cvode/cvode.h>

// For sundials v7, N_VNew_Serial takes a context. We need to define a mock macro
// for the older N_VNew_Serial that takes only one argument.
static SUNContext _global_ctx = nullptr;

// Redefine N_VNew_Serial for the older code
#define N_VNew_Serial(len) (_global_ctx ? N_VNew_Serial(len, _global_ctx) : nullptr)

// Override include guards to skip the missing old sundials headers
#define _CVODE_DENSE_H
#define _CVODE_SPGMR_H

// Make sure the main function doesn't conflict
#define mexFunction mexFunction_ignored

// Because of string literal to char* conversions, we need to declare check_flag
// in a way that C++ accepts, or just define it out in the C file. We'll use a macro above.
// For the math.h time conflict:
#define time _time_mock

// Ignore string constant warnings to allow the char* parameters
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

// Include the source directly
extern "C" {
    #include "../bng2/Validate/DAT_validate/localfunc_mex_cvode.c"
}

#pragma GCC diagnostic pop

// Reset macros for our tests
#undef N_VNew_Serial
#undef time

// --- Test Cases ---

// Setup context for the tests
struct SundialsContext {
    SUNContext ctx;
    SundialsContext() { SUNContext_Create(SUN_COMM_NULL, &ctx); _global_ctx = ctx; }
    ~SundialsContext() { _global_ctx = nullptr; SUNContext_Free(&ctx); }
};

TEST_CASE("calc_expressions sets correctly", "[localfunc]") {
    SundialsContext ctx;

    N_Vector expressions = N_VNew_Serial(__N_EXPRESSIONS__, ctx.ctx);
    double parameters[4] = {1.5, 2.5, 3.5, 4.5};

    calc_expressions(expressions, parameters);

    REQUIRE(NV_Ith_S(expressions, 0) == 1.5);
    REQUIRE(NV_Ith_S(expressions, 1) == 2.5);
    REQUIRE(NV_Ith_S(expressions, 2) == 3.5);
    REQUIRE(NV_Ith_S(expressions, 3) == 4.5);
    REQUIRE(NV_Ith_S(expressions, 4) == 0.0);
    REQUIRE(NV_Ith_S(expressions, 5) == 3.5);
    REQUIRE(NV_Ith_S(expressions, 6) == 14.0);
    REQUIRE(NV_Ith_S(expressions, 7) == 31.5);

    N_VDestroy_Serial(expressions);
}

TEST_CASE("calc_observables calculates correct values", "[localfunc]") {
    SundialsContext ctx;

    N_Vector species = N_VNew_Serial(__N_SPECIES__, ctx.ctx);
    N_Vector observables = N_VNew_Serial(__N_OBSERVABLES__, ctx.ctx);
    N_Vector expressions = N_VNew_Serial(__N_EXPRESSIONS__, ctx.ctx);

    // Initialize species
    NV_Ith_S(species, 0) = 1.0;
    NV_Ith_S(species, 1) = 2.0;
    NV_Ith_S(species, 2) = 3.0;
    NV_Ith_S(species, 3) = 4.0;
    NV_Ith_S(species, 4) = 5.0;
    NV_Ith_S(species, 5) = 6.0;
    NV_Ith_S(species, 6) = 7.0;

    calc_observables(observables, species, expressions);

    REQUIRE(NV_Ith_S(observables, 0) == 1.0 + 5.0 + 6.0 + 7.0);
    REQUIRE(NV_Ith_S(observables, 1) == 2.0 + 5.0 + 2*6.0 + 3*7.0);
    REQUIRE(NV_Ith_S(observables, 2) == 3.0);
    REQUIRE(NV_Ith_S(observables, 3) == 1.0);
    REQUIRE(NV_Ith_S(observables, 4) == 5.0);
    REQUIRE(NV_Ith_S(observables, 5) == 6.0);
    REQUIRE(NV_Ith_S(observables, 6) == 7.0);
    REQUIRE(NV_Ith_S(observables, 7) == 5.0 + 2*6.0 + 3*7.0);

    N_VDestroy_Serial(species);
    N_VDestroy_Serial(observables);
    N_VDestroy_Serial(expressions);
}

TEST_CASE("calc_ratelaws calculates correct values", "[localfunc]") {
    SundialsContext ctx;

    N_Vector species = N_VNew_Serial(__N_SPECIES__, ctx.ctx);
    N_Vector observables = N_VNew_Serial(__N_OBSERVABLES__, ctx.ctx);
    N_Vector expressions = N_VNew_Serial(__N_EXPRESSIONS__, ctx.ctx);
    N_Vector ratelaws = N_VNew_Serial(__N_RATELAWS__, ctx.ctx);

    // Initialize values
    for(int i = 0; i < __N_SPECIES__; i++) NV_Ith_S(species, i) = (double)(i + 1);
    for(int i = 0; i < __N_EXPRESSIONS__; i++) NV_Ith_S(expressions, i) = 0.5 * (i + 1);

    calc_ratelaws(ratelaws, species, expressions, observables);

    REQUIRE_THAT(NV_Ith_S(ratelaws, 0), Catch::Matchers::WithinRel(2.5 * 1.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 1), Catch::Matchers::WithinRel(3.0 * 0.5 * 1.0 * 2.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 2), Catch::Matchers::WithinRel(2.0 * 3.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 3), Catch::Matchers::WithinRel(3.0 * 5.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 4), Catch::Matchers::WithinRel(2.0 * 0.5 * 5.0 * 2.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 5), Catch::Matchers::WithinRel(1.0 * 5.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 6), Catch::Matchers::WithinRel(3.5 * 6.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 7), Catch::Matchers::WithinRel(0.5 * 6.0 * 2.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 8), Catch::Matchers::WithinRel(2.0 * 1.0 * 6.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 9), Catch::Matchers::WithinRel(4.0 * 7.0));
    REQUIRE_THAT(NV_Ith_S(ratelaws, 10), Catch::Matchers::WithinRel(3.0 * 1.0 * 7.0));

    N_VDestroy_Serial(species);
    N_VDestroy_Serial(observables);
    N_VDestroy_Serial(expressions);
    N_VDestroy_Serial(ratelaws);
}

TEST_CASE("calc_species_deriv calculates correct derivatives", "[localfunc]") {
    SundialsContext ctx;

    N_Vector species = N_VNew_Serial(__N_SPECIES__, ctx.ctx);
    N_Vector Dspecies = N_VNew_Serial(__N_SPECIES__, ctx.ctx);
    N_Vector expressions = N_VNew_Serial(__N_EXPRESSIONS__, ctx.ctx);
    N_Vector observables = N_VNew_Serial(__N_OBSERVABLES__, ctx.ctx);
    N_Vector ratelaws = N_VNew_Serial(__N_RATELAWS__, ctx.ctx);

    N_Vector temp_data[3] = {expressions, observables, ratelaws};

    // Initialize values
    for(int i = 0; i < __N_SPECIES__; i++) NV_Ith_S(species, i) = 1.0;
    for(int i = 0; i < __N_EXPRESSIONS__; i++) NV_Ith_S(expressions, i) = 1.0;

    calc_species_deriv(0.0, species, Dspecies, temp_data);

    // Calculate expected ratelaws manually
    // r0 = e4*s0 = 1*1 = 1
    // r1 = 3*e0*s0*s1 = 3*1*1*1 = 3
    // r2 = e3*s2 = 1*1 = 1
    // r3 = e5*s4 = 1*1 = 1
    // r4 = 2*e0*s4*s1 = 2*1*1*1 = 2
    // r5 = e1*s4 = 1*1 = 1
    // r6 = e6*s5 = 1*1 = 1
    // r7 = e0*s5*s1 = 1*1*1 = 1
    // r8 = 2*e1*s5 = 2*1*1 = 2
    // r9 = e7*s6 = 1*1 = 1
    // r10 = 3*e1*s6 = 3*1*1 = 3

    REQUIRE_THAT(NV_Ith_S(Dspecies, 0), Catch::Matchers::WithinRel(-3.0 + 1.0));
    REQUIRE_THAT(NV_Ith_S(Dspecies, 1), Catch::Matchers::WithinRel(-3.0 - 2.0 + 1.0 - 1.0 + 2.0 + 3.0));
    REQUIRE_THAT(NV_Ith_S(Dspecies, 2), Catch::Matchers::WithinRel(1.0 - 1.0 + 1.0 + 1.0 + 1.0));
    REQUIRE_THAT(NV_Ith_S(Dspecies, 3), Catch::Matchers::WithinRel(0.0));
    REQUIRE_THAT(NV_Ith_S(Dspecies, 4), Catch::Matchers::WithinRel(3.0 - 2.0 - 1.0 + 2.0));
    REQUIRE_THAT(NV_Ith_S(Dspecies, 5), Catch::Matchers::WithinRel(2.0 - 1.0 - 2.0 + 3.0));
    REQUIRE_THAT(NV_Ith_S(Dspecies, 6), Catch::Matchers::WithinRel(1.0 - 3.0));

    N_VDestroy_Serial(species);
    N_VDestroy_Serial(Dspecies);
    N_VDestroy_Serial(expressions);
    N_VDestroy_Serial(observables);
    N_VDestroy_Serial(ratelaws);
}
