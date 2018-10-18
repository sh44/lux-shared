#pragma once

#include <cstdio>
#include <cstdlib>

///zero-overhead defer mechanism in c++, neat
struct DeferDummy {};
template<typename F>
struct Deferrer
{
    F f;
    ~Deferrer() { f(); }
};
template<typename F>
Deferrer<F> operator*(DeferDummy, F f) { return {f}; }
#define LUX_DEFER_0(LINE) zz_defer##LINE
#define LUX_DEFER_1(LINE) LUX_DEFER_0(LINE)
#define LUX_DEFER auto LUX_DEFER_1(__LINE__) = DeferDummy { } *[&]()

enum LuxRval : bool {
    LUX_FAIL = false,
    LUX_OK   = true,
};

#define LUX_LOG(fmt, ...) { \
    std::printf("%s(): " fmt "\n", __func__ __VA_OPT__(,) __VA_ARGS__); }

#define LUX_LOG_ERR(fmt, ...) { \
    std::printf("ERROR %s(): " fmt "\n", __func__ __VA_OPT__(,) __VA_ARGS__); }

#define LUX_LOG_DBG(fmt, ...) { \
    std::printf("DEBUG %s(): " fmt "\n", __func__ __VA_OPT__(,) __VA_ARGS__); }

#define LUX_FATAL(fmt, ...) { \
    std::fprintf(stderr, "FATAL %s(): " fmt "\n", \
            __func__ __VA_OPT__(,) __VA_ARGS__); \
    std::quick_exit(EXIT_FAILURE); }

#define LUX_PANIC(fmt, ...) { \
    std::fprintf(stderr, "PANIC %s(): " fmt "\n", \
            __func__ __VA_OPT__(,) __VA_ARGS__); \
    std::abort(); }

#define LUX_MAY_FAIL [[nodiscard]] LuxRval
#define LUX_RETHROW(expr, ...) { \
    if((expr) != LUX_OK) { \
        LUX_LOG_ERR(__VA_ARGS__); \
        return LUX_FAIL; \
    } }

#ifndef NDEBUG
    #define LUX_ASSERT(expr) { \
        if(!(expr)) LUX_PANIC("assertion `" #expr "` failed"); }
#else
    #define LUX_ASSERT(expr) do {} while(false)
#endif

#define LUX_UNREACHABLE()   LUX_ASSERT(false)
#define LUX_UNIMPLEMENTED() LUX_LOG("unimplemented")

#define LUX_SIGN_REPR_UNKNOWN            0
#define LUX_SIGN_REPR_SIGN_AND_MAGNITUDE 1
#define LUX_SIGN_REPR_ONES_COMPLEMENT    2
#define LUX_SIGN_REPR_TWOS_COMPLEMENT    3

#define LUX_SIGN_REPR (-1 & 3)

#if LUX_SIGN_REPR != LUX_SIGN_REPR_SIGN_AND_MAGNITUDE &&  \
    LUX_SIGN_REPR != LUX_SIGN_REPR_ONES_COMPLEMENT    &&  \
    LUX_SIGN_REPR != LUX_SIGN_REPR_TWOS_COMPLEMENT

    #warning "unknown signed representation"
    #undef LUX_SIGN_REPR
    #define LUX_SIGN_REPR LUX_SIGN_REPR_UNKNOWN
#endif

#if defined(_WIN32) || defined(_WIN64)
    #if _WIN64
        #define LUX_64BIT
    #else
        #define LUX_32BIT
    #endif
#elif defined(__x86_64__) || defined(__ppc64__)
    #if __x86_64__
        #define LUX_64BIT
    #else
        #define LUX_32BIT
    #endif
#endif

#if !defined(LUX_32BIT) && !defined(LUX_64BIT)
    #error "neither LUX_32BIT, neither LUX_64BIT is defined"
#endif
