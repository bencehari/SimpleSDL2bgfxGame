#ifndef KE_CONSC_H
#define KE_CONSC_H

// AC stands for ANSI_COLOR
#define AC_RED     "\x1b[31m"
#define AC_GREEN   "\x1b[32m"
#define AC_YELLOW  "\x1b[33m"
#define AC_BLUE    "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN    "\x1b[36m"
#define AC_RESET   "\x1b[0m"

// usage example:
// printf(AC_RED "ERROR!\n" AC_RESET);
// printf(AC_GREEN "[DONE]" AC_RESET " %d model(s) loaded.\n", loadedModelCount);

#endif // KE_CONSC_H
