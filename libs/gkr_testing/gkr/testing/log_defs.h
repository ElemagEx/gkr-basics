#pragma once

#define SEVERITY_FATAL   0
#define SEVERITY_ERROR   1
#define SEVERITY_WARNING 2
#define SEVERITY_INFO    3
#define SEVERITY_VERBOSE 4

#define FACILITY_GENERAL 0
#define FACILITY_NETWORK 1
#define FACILITY_FILESYS 2
#define FACILITY_SYNCHRO 3

#define COMMON_SEVERITIES { \
    {"Fatal"  , SEVERITY_FATAL  }, \
    {"Error"  , SEVERITY_ERROR  }, \
    {"Warning", SEVERITY_WARNING}, \
    {"Info"   , SEVERITY_INFO   }, \
    {"Verbose", SEVERITY_VERBOSE}, \
    {nullptr  , 0               }}

#define COMMON_FACILITIES { \
    {"General", FACILITY_GENERAL}, \
    {"Network", FACILITY_NETWORK}, \
    {"FileSys", FACILITY_FILESYS}, \
    {"Synchro", FACILITY_SYNCHRO}, \
    {nullptr  , 0               }}

