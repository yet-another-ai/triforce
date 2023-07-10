#ifndef __TRIFORCE_CPU_DETECTOR_H__
#define __TRIFORCE_CPU_DETECTOR_H__

#if !defined(__x86_64__) && !defined(_M_X64) && !defined(_M_AMD64)
#error libtriforce is for x86_64 architecture ONLY
#endif

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <cpuid.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Macros to store values for CPUID register ordinals
#define TRIFORCE_CPUID_EAX 0
#define TRIFORCE_CPUID_EBX 1
#define TRIFORCE_CPUID_ECX 2
#define TRIFORCE_CPUID_EDX 3

#define TRIFORCE_LEAF_CPUID_BASIC             0x00       // Basic CPUID Information
#define TRIFORCE_LEAF_THERMAL_POWER           0x06       // Thermal and Power Management Leaf
#define TRIFORCE_LEAF_EXTENDED_FEATURE_FLAGS  0x07       // Structured Extended Feature Flags Enumeration Leaf (Output depends on ECX input value)
#define TRIFORCE_LEAF_EXTENDED_STATE          0x0D       // Processor Extended State Enumeration Main Leaf (EAX = 0DH, ECX = 0)
#define TRIFORCE_LEAF_FREQUENCY_INFORMATION   0x16       // Processor Frequency Information Leaf  function 0x16 only works on Skylake or newer.
#define TRIFORCE_LEAF_HYBRID_INFORMATION      0x1A       // Hybrid Information Sub - leaf(EAX = 1AH, ECX = 0)
#define TRIFORCE_LEAF_EXTENDED_INFORMATION_0  0x80000000 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_INFORMATION_1  0x80000001 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_BRAND_STRING_1 0x80000002 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_BRAND_STRING_2 0x80000003 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_BRAND_STRING_3 0x80000004 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_INFORMATION_5  0x80000005 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_INFORMATION_6  0x80000006 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_INFORMATION_7  0x80000007 // Extended Function CPUID Information
#define TRIFORCE_LEAF_EXTENDED_INFORMATION_8  0x80000008 // Extended Function CPUID Information
#define TRIFORCE_LEAF_AMD_HYBRID_INFORMATION  0x80000026 // AMD Hybrid Information Sub - leaf(EAX = 80000026H, ECX = 0)
#define TRIFORCE_LEAF_CPU_FUNCTION_MAX TRIFORCE_LEAF_AMD_HYBRID_INFORMATION

typedef enum TriForceCpuVendor {
  TRIFORCE_CPU_VENDOR_UNKNOWN = 0x00,
  TRIFORCE_CPU_VENDOR_INTEL   = 0x10,
  TRIFORCE_CPU_VENDOR_AMD     = 0x20,
  TRIFORCE_CPU_VENDOR_HYGON   = 0x21,
  TRIFORCE_CPU_VENDOR_VIA     = 0x30,
  TRIFORCE_CPU_VENDOR_CENTAUR = 0x31,
  TRIFORCE_CPU_VENDOR_ZHAOXIN = 0x32,
} TriForceCpuVendor;

enum TriForceCoreType {
  // Performance cores, or homogeneous cores
  // cores with higher performance through higher clock speeds, cache, numbers of execution units, or etc
  // like Intel and AMD Performance cores, or ARM Ultimate-Performance (CXC) cores
  TRIFORCE_CORE_TYPE_PERFORMANCE = 0x00,
   // Efficiency cores
   // cores are tuned to have high performance-per-watt, but lower peak performance
   // like Intel and AMD Efficiency cores, or ARM Performance-Efficient (big) cores
  TRIFORCE_CORE_TYPE_EFFICIENCY = 0x10,
  // Power-saving cores
  // cores designed for system standby, achieving higher efficiency through reducing chip switching (leakage) power
  // like ARM High-Efficiency (LITTLE) cores, and no Intel or AMD equivalent yet
  TRIFORCE_CORE_TYPE_POWERSAVING = 0x20, 
};

typedef struct {
  char vendor_id[13];
  char brand_string[64];
  uint32_t num_groups;
  uint32_t num_numa_nodes;
  uint32_t num_processor_packages;
  uint32_t num_physical_cores;
  uint32_t num_logical_cores;
  bool hybrid;
} triforce_processor_info_t;

inline TriForceCpuVendor get_cpu_vendor(triforce_processor_info_t* processor_info) {
  if (!strcmp("GenuineIntel", processor_info->vendor_id)) return TRIFORCE_CPU_VENDOR_INTEL;
  if (!strcmp("AuthenticAMD", processor_info->vendor_id)) return TRIFORCE_CPU_VENDOR_AMD;
  if (!strcmp("HygonGenuine", processor_info->vendor_id)) return TRIFORCE_CPU_VENDOR_HYGON;
  if (!strcmp("VIA VIA VIA ", processor_info->vendor_id)) return TRIFORCE_CPU_VENDOR_VIA;
  if (!strcmp("CentaurHauls", processor_info->vendor_id)) return TRIFORCE_CPU_VENDOR_CENTAUR;
  if (!strcmp("  Shanghai  ", processor_info->vendor_id)) return TRIFORCE_CPU_VENDOR_ZHAOXIN;
  return TRIFORCE_CPU_VENDOR_UNKNOWN;
}

// cross-platform CPUID implementation
#ifdef _WIN32
#define TRIFORCE_CPUID(registers, function) __cpuid((int*)registers, (int)function);
#define TRIFORCE_CPUIDEX(registers, function, extFunction) __cpuidex((int*)registers, (int)function, (int)extFunction);
#else // *NIX
#define TRIFORCE_CPUID(registers, function) asm volatile ("cpuid" : "=a" (registers[0]), "=b" (registers[1]), "=c" (registers[2]), "=d" (registers[3]) : "a" (function), "c" (0));
#define TRIFORCE_CPUIDEX(registers, function, extFunction) asm volatile ("cpuid" : "=a" (registers[0]), "=b" (registers[1]), "=c" (registers[2]), "=d" (registers[3]) : "a" (function), "c" (extFunction));
#endif

inline bool triforce_cpuid(uint32_t registers[4], uint32_t function, uint32_t extFunction) {
  if (function > TRIFORCE_LEAF_CPU_FUNCTION_MAX) return false;
  TRIFORCE_CPUIDEX(registers, function, extFunction);
  return true;
}

#endif // __TRIFORCE_CPUID_H__
