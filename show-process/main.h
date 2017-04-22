#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define OPT_PROCESS			'p'
#define OPT_SHOW			's'
#define OPT_HELP			'h'
#define OPT_LIST			"p:s:h"

extern char *optarg;
extern int optind, opterr, optopt;

#endif  /* __MAIN_H__ */