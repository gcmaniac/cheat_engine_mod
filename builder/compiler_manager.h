#ifndef COMPILER_MANAGER_H
#define COMPILER_MANAGER_H

#include "compiler_config.h"

#ifdef _WIN32
#include <windows.h>
#else
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#endif

typedef struct {
    char mingw32_gcc[MAX_PATH];
    char mingw64_gcc[MAX_PATH];
    int has_mingw32;
    int has_mingw64;
} CompilerEnvironment;

/* Detect installed toolchains */
int compiler_detect_environment(CompilerEnvironment* env);

/* Print all available targets and descriptions */
void compiler_print_targets(void);

/* Compile a specific target by enum ID */
int compiler_build_target(const CompilerEnvironment* env, OSTargetID target_id);

/* Compile all 5 OS targets */
int compiler_build_all(const CompilerEnvironment* env);

#endif /* COMPILER_MANAGER_H */
