#ifndef CRYPTOPP_CONFIG_H
#define CRYPTOPP_CONFIG_H

// ***************** Important Settings ********************

// define this if running on a big-endian CPU
#if !defined(IS_LITTLE_ENDIAN) && (defined(__BIG_ENDIAN__) || defined(__sparc) || defined(__sparc__) || defined(__hppa__) || defined(__mips__) || (defined(__MWERKS__) && !defined(__INTEL__)))
#	define IS_BIG_ENDIAN
#endif

// define this if running on a little-endian CPU
// big endian will be assumed if IS_LITTLE_ENDIAN is not defined
#ifndef IS_BIG_ENDIAN
#	define IS_LITTLE_ENDIAN
#endif

// define this if you want to disable all OS-dependent features,
// such as sockets and OS-provided random number generators
// #define NO_OS_DEPENDENCE

// Define this to use features provided by Microsoft's CryptoAPI.
// Currently the only feature used is random number generation.
// This macro will be ignored if NO_OS_DEPENDENCE is defined.
#define USE_MS_CRYPTOAPI

// Define this to 1 to enforce the requirement in FIPS 186-2 Change Notice 1 that only 1024 bit moduli be used
#ifndef DSA_1024_BIT_MODULUS_ONLY
#	define DSA_1024_BIT_MODULUS_ONLY 1
#endif

// ***************** Less Important Settings ***************

// define this to retain (as much as possible) old deprecated function and class names
// #define CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY

#define GZIP_OS_CODE 0

// Try this if your CPU has 256K internal cache or a slow multiply instruction
// and you want a (possibly) faster IDEA implementation using log tables
// #define IDEA_LARGECACHE

// Define this if, for the linear congruential RNG, you want to use
// the original constants as specified in S.K. Park and K.W. Miller's
// CACM paper.
// #define LCRNG_ORIGINAL_NUMBERS

// choose which style of sockets to wrap (mostly useful for cygwin which has both)
#define PREFER_BERKELEY_STYLE_SOCKETS
// #define PREFER_WINDOWS_STYLE_SOCKETS

// set the name of Rijndael cipher, was "Rijndael" before version 5.3
#define CRYPTOPP_RIJNDAEL_NAME "AES"

// ***************** Important Settings Again ********************
// But the defaults should be ok.

// namespace support is now required
#ifdef NO_NAMESPACE
#	error namespace support is now required
#endif

// Define this to workaround a Microsoft CryptoAPI bug where
// each call to CryptAcquireContext causes a 100 KB memory leak.
// Defining this will cause Crypto++ to make only one call to CryptAcquireContext.
#define WORKAROUND_MS_BUG_Q258000

#ifdef CRYPTOPP_DOXYGEN_PROCESSING
// Avoid putting "CryptoPP::" in front of everything in Doxygen output
#	define CryptoPP
#	define NAMESPACE_BEGIN(x)
#	define NAMESPACE_END
// Get Doxygen to generate better documentation for these typedefs
#	define DOCUMENTED_TYPEDEF(x, y) class y : public x {};
#else
#	define NAMESPACE_BEGIN(x) namespace x {
#	define NAMESPACE_END }
#	define DOCUMENTED_TYPEDEF(x, y) typedef x y;
#endif
#define ANONYMOUS_NAMESPACE_BEGIN namespace {
#define USING_NAMESPACE(x) using namespace x;
#define DOCUMENTED_NAMESPACE_BEGIN(x) namespace x {
#define DOCUMENTED_NAMESPACE_END }

// What is the type of the third parameter to bind?
// For Unix, the new standard is ::socklen_t (typically unsigned int), and the old standard is int.
// Unfortunately there is no way to tell whether or not socklen_t is defined.
// To work around this, TYPE_OF_SOCKLEN_T is a macro so that you can change it from the makefile.
#ifndef TYPE_OF_SOCKLEN_T
#	if defined(_WIN32) || defined(__CYGWIN__)
#		define TYPE_OF_SOCKLEN_T int
#	else
#		define TYPE_OF_SOCKLEN_T ::socklen_t
#	endif
#endif

#if defined(__CYGWIN__) && defined(PREFER_WINDOWS_STYLE_SOCKETS)
#	define __USE_W32_SOCKETS
#endif

typedef unsigned char byte;		// put in global namespace to avoid ambiguity with other byte typedefs

NAMESPACE_BEGIN(CryptoPP)

typedef unsigned short word16;
typedef unsigned int word32;

#if defined(__GNUC__) || defined(__MWERKS__) || defined(__SUNPRO_CC)
	#define WORD64_AVAILABLE
	typedef unsigned long long word64;
	#define W64LIT(x) x##LL
#elif defined(_MSC_VER) || defined(__BORLANDC__)
	#define WORD64_AVAILABLE
	typedef unsigned __int64 word64;
	#define W64LIT(x) x##ui64
#endif

// define largest word type
#ifdef WORD64_AVAILABLE
	typedef word64 lword;
	const lword LWORD_MAX = W64LIT(0)-1;
#else
	typedef word32 lword;
	const lword LWORD_MAX = lword(0)-1;
#endif

#if defined(__alpha__) || defined(__ia64__) || defined(_ARCH_PPC64) || defined(__x86_64__) || defined(__mips64) || defined(_M_X64)
	// These platforms have 64-bit CPU registers. Unfortunately most C++ compilers doesn't
	// allow any way to access the 64-bit by 64-bit multiply instruction without using
	// assembly, so in order to use word64 as word, the assembly instruction must be defined
	// in Dword::Multiply().
	#if defined(__SUNPRO_CC)	// no Dword::Multiply() for these compilers yet
		#define CRYPTOPP_NATIVE_DWORD_AVAILABLE
		typedef word16 hword;
		typedef word32 word;
		typedef word64 dword;
	#else
		typedef word32 hword;
		typedef word64 word;
	#endif
#else
	#define CRYPTOPP_NATIVE_DWORD_AVAILABLE
	#ifdef WORD64_AVAILABLE
		#define CRYPTOPP_SLOW_WORD64 // defined this if your CPU is not 64-bit to use alternative code that avoids word64
		typedef word16 hword;
		typedef word32 word;
		typedef word64 dword;
	#else
		typedef byte hword;
		typedef word16 word;
		typedef word32 dword;
	#endif
#endif

const unsigned int WORD_SIZE = sizeof(word);
const unsigned int WORD_BITS = WORD_SIZE * 8;

#if defined(_MSC_VER) // || defined(__BORLANDC__) intrinsics don't work on BCB 2006
	#define INTEL_INTRINSICS
	#define FAST_ROTATE
#elif defined(__MWERKS__) && TARGET_CPU_PPC
	#define PPC_INTRINSICS
	#define FAST_ROTATE
#elif defined(__GNUC__) && defined(__i386__)
	// GCC does peephole optimizations which should result in using rotate instructions
	#define FAST_ROTATE
#endif

#ifndef CRYPTOPP_L1_CACHE_LINE_SIZE
	// This should be a lower bound on the L1 cache line size. It's used for defense against timing attacks.
	// L1 cache line size is 32 on Pentium III and earlier
	#define CRYPTOPP_L1_CACHE_LINE_SIZE 32
#endif

#ifndef CRYPTOPP_L1_CACHE_ALIGN
	#if defined(_MSC_VER)
		#define CRYPTOPP_L1_CACHE_ALIGN(x) __declspec(align(CRYPTOPP_L1_CACHE_LINE_SIZE)) x
	#elif defined(__GNUC__)
		#define CRYPTOPP_L1_CACHE_ALIGN(x) x __attribute__((aligned(CRYPTOPP_L1_CACHE_LINE_SIZE)))
	#else
		#define CRYPTOPP_L1_CACHE_ALIGN(x) x
	#endif
#endif

NAMESPACE_END

// VC60 workaround: it doesn't allow typename in some places
#if defined(_MSC_VER) && (_MSC_VER < 1300)
#define CPP_TYPENAME
#else
#define CPP_TYPENAME typename
#endif

#ifdef _MSC_VER
#define CRYPTOPP_NO_VTABLE __declspec(novtable)
#else
#define CRYPTOPP_NO_VTABLE
#endif

#ifdef _MSC_VER
	// 4231: nonstandard extension used : 'extern' before template explicit instantiation
	// 4250: dominance
	// 4251: member needs to have dll-interface
	// 4275: base needs to have dll-interface
	// 4660: explicitly instantiating a class that's already implicitly instantiated
	// 4661: no suitable definition provided for explicit template instantiation request
	// 4786: identifer was truncated in debug information
	// 4355: 'this' : used in base member initializer list
#	pragma warning(disable: 4231 4250 4251 4275 4660 4661 4786 4355)
#endif

#ifdef __BORLANDC__
// 8037: non-const function called for const object. needed to work around BCB2006 bug
#	pragma warn -8037
#endif

#if 1 || (defined(_MSC_VER) && _MSC_VER <= 1300) || defined(__MWERKS__) || defined(_STLPORT_VERSION)
#define CRYPTOPP_DISABLE_UNCAUGHT_EXCEPTION
#endif

#ifndef CRYPTOPP_DISABLE_UNCAUGHT_EXCEPTION
#define CRYPTOPP_UNCAUGHT_EXCEPTION_AVAILABLE
#endif

// CodeWarrior defines _MSC_VER
#if !defined(CRYPTOPP_DISABLE_X86ASM) && ((defined(_MSC_VER) && !defined(__MWERKS__) && defined(_M_IX86)) || (defined(__GNUC__) && defined(__i386__)))
#define CRYPTOPP_X86ASM_AVAILABLE
#endif

// how to disable inlining
#if defined(_MSC_VER) && _MSC_VER >= 1300
#	define CRYPTOPP_NOINLINE_DOTDOTDOT
#	define CRYPTOPP_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#	define CRYPTOPP_NOINLINE_DOTDOTDOT
#	define CRYPTOPP_NOINLINE __attribute__((noinline))
#else
#	define CRYPTOPP_NOINLINE_DOTDOTDOT ...
#	define CRYPTOPP_NOINLINE 
#endif

// how to declare class constants
#if defined(_MSC_VER) && _MSC_VER < 1300
#	define CRYPTOPP_CONSTANT(x) enum {x};
#else
#	define CRYPTOPP_CONSTANT(x) static const int x;
#endif

// how to allocate 16-byte aligned memory (for SSE2)
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#	define CRYPTOPP_MALLOC_ALIGNMENT_IS_16
#elif defined(__linux__) || defined(__sun__) || defined(__CYGWIN__)
#	define CRYPTOPP_MEMALIGN_AVAILABLE
#endif

// ***************** determine availability of OS features ********************

#ifndef NO_OS_DEPENDENCE

#if defined(_WIN32) || defined(__CYGWIN__)
#define CRYPTOPP_WIN32_AVAILABLE
#endif

#if defined(__unix__) || defined(__MACH__) || defined(__NetBSD__)
#define CRYPTOPP_UNIX_AVAILABLE
#endif

#if defined(WORD64_AVAILABLE) && (defined(CRYPTOPP_WIN32_AVAILABLE) || defined(CRYPTOPP_UNIX_AVAILABLE))
#	define HIGHRES_TIMER_AVAILABLE
#endif

#ifdef CRYPTOPP_UNIX_AVAILABLE
#	define HAS_BERKELEY_STYLE_SOCKETS
#endif

#ifdef CRYPTOPP_WIN32_AVAILABLE
#	define HAS_WINDOWS_STYLE_SOCKETS
#endif

#if defined(HIGHRES_TIMER_AVAILABLE) && (defined(HAS_BERKELEY_STYLE_SOCKETS) || defined(HAS_WINDOWS_STYLE_SOCKETS))
#	define SOCKETS_AVAILABLE
#endif

#if defined(HAS_WINDOWS_STYLE_SOCKETS) && (!defined(HAS_BERKELEY_STYLE_SOCKETS) || defined(PREFER_WINDOWS_STYLE_SOCKETS))
#	define USE_WINDOWS_STYLE_SOCKETS
#else
#	define USE_BERKELEY_STYLE_SOCKETS
#endif

#if defined(HIGHRES_TIMER_AVAILABLE) && defined(CRYPTOPP_WIN32_AVAILABLE) && !defined(USE_BERKELEY_STYLE_SOCKETS)
#	define WINDOWS_PIPES_AVAILABLE
#endif

#if defined(CRYPTOPP_WIN32_AVAILABLE) && defined(USE_MS_CRYPTOAPI)
#	define NONBLOCKING_RNG_AVAILABLE
#	define OS_RNG_AVAILABLE
#endif

#if defined(CRYPTOPP_UNIX_AVAILABLE) || defined(CRYPTOPP_DOXYGEN_PROCESSING)
#	define NONBLOCKING_RNG_AVAILABLE
#	define BLOCKING_RNG_AVAILABLE
#	define OS_RNG_AVAILABLE
#	define HAS_PTHREADS
#	define THREADS_AVAILABLE
#endif

#ifdef CRYPTOPP_WIN32_AVAILABLE
#	define HAS_WINTHREADS
#	define THREADS_AVAILABLE
#endif

#endif	// NO_OS_DEPENDENCE

// ***************** DLL related ********************

#ifdef CRYPTOPP_WIN32_AVAILABLE

#ifdef CRYPTOPP_EXPORTS
#define CRYPTOPP_IS_DLL
#define CRYPTOPP_DLL __declspec(dllexport)
//#elif defined(CRYPTOPP_IMPORTS)
#else
#define CRYPTOPP_IS_DLL
#define CRYPTOPP_DLL __declspec(dllimport)
//#else
//#define CRYPTOPP_DLL
#endif

#define CRYPTOPP_API __cdecl

#else	// CRYPTOPP_WIN32_AVAILABLE

#define CRYPTOPP_DLL
#define CRYPTOPP_API

#endif	// CRYPTOPP_WIN32_AVAILABLE

#if defined(__MWERKS__)
#define CRYPTOPP_EXTERN_DLL_TEMPLATE_CLASS extern class CRYPTOPP_DLL
#elif defined(__BORLANDC__) || defined(__SUNPRO_CC)
#define CRYPTOPP_EXTERN_DLL_TEMPLATE_CLASS template class CRYPTOPP_DLL
#else
#define CRYPTOPP_EXTERN_DLL_TEMPLATE_CLASS template class CRYPTOPP_DLL
#endif

#if defined(CRYPTOPP_MANUALLY_INSTANTIATE_TEMPLATES) && !defined(CRYPTOPP_IMPORTS)
#define CRYPTOPP_DLL_TEMPLATE_CLASS template class CRYPTOPP_DLL
#else
#define CRYPTOPP_DLL_TEMPLATE_CLASS CRYPTOPP_EXTERN_DLL_TEMPLATE_CLASS
#endif

#if defined(__MWERKS__)
#define CRYPTOPP_EXTERN_STATIC_TEMPLATE_CLASS extern class
#elif defined(__BORLANDC__) || defined(__SUNPRO_CC)
#define CRYPTOPP_EXTERN_STATIC_TEMPLATE_CLASS template class
#else
#define CRYPTOPP_EXTERN_STATIC_TEMPLATE_CLASS extern template class
#endif

#if defined(CRYPTOPP_MANUALLY_INSTANTIATE_TEMPLATES) && !defined(CRYPTOPP_EXPORTS)
#define CRYPTOPP_STATIC_TEMPLATE_CLASS template class
#else
#define CRYPTOPP_STATIC_TEMPLATE_CLASS CRYPTOPP_EXTERN_STATIC_TEMPLATE_CLASS
#endif

#endif
