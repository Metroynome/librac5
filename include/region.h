#ifndef LIBRAC5_REGION_H
#define LIBRAC5_REGION_H
#if (defined(RAC5_NTSCU) + defined(RAC5_PAL) + defined(RAC5_NTSCJ) + defined(RAC5_AUTO)) > 1
#error Select only one RAC5 region
#endif
/* These select the archive/build label. Use rac5GetBuild for runtime identification. */
#ifdef __cplusplus
extern "C" {
#endif
const char *rac5BuildRegion(void);
typedef enum Rac5Region {
    RAC5_REGION_UNKNOWN = 0, RAC5_REGION_NTSCU = 1,
    RAC5_REGION_PAL = 2, RAC5_REGION_NTSCJ = 3
} Rac5Region;
typedef struct Rac5BuildInfo {
    Rac5Region region;
    const char *name;
    const char *serial;
    const char *version;
    unsigned int entryPoint, symbolTable, symbolCount;
} Rac5BuildInfo;
#ifdef __cplusplus
}
#endif
#endif
