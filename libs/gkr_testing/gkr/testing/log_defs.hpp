#pragma once

#define FACILITY_GENERAL 0
#define FACILITY_NETWORK 1
#define FACILITY_FILESYS 2
#define FACILITY_SYNCHRO 3

#define COMMON_FACILITIES_INFOS { \
    {"General", FACILITY_GENERAL}, \
    {"Network", FACILITY_NETWORK}, \
    {"FileSys", FACILITY_FILESYS}, \
    {"Synchro", FACILITY_SYNCHRO}, \
    {nullptr  , 0               }}

