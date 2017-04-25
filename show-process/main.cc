#include "main.h"

static void usage(void)
{
	fprintf(stderr, "Usage: show-process -p process {-s option}\n\n");
	fprintf(stderr, "Show every non-owned window of a process\n\n");
	fprintf(stderr, " -%c process: %s\n", OPT_PROCESS , "process id");
	fprintf(stderr, " -%c option: %s\n", OPT_SHOW , "what to do with the window (default=SW_SHOWNORMAL:1)");
	fprintf(stderr, " %s\n" , "SW_HIDE:0");
	fprintf(stderr, " %s\n" , "SW_SHOWNORMAL:1");
	fprintf(stderr, " %s\n" , "SW_SHOWMINIMIZED:2");
	fprintf(stderr, " %s\n" , "SW_SHOWMAXIMIZED:3");
	fprintf(stderr, " %s\n" , "SW_SHOWNOACTIVATE:4");
	fprintf(stderr, " %s\n" , "SW_SHOW:5");
	fprintf(stderr, " %s\n" , "SW_MINIMIZE:6");
	fprintf(stderr, " %s\n" , "SW_SHOWMINNOACTIVE:7");
	fprintf(stderr, " %s\n" , "SW_SHOWNA:8");
	fprintf(stderr, " %s\n" , "SW_RESTORE:9");
	fprintf(stderr, " %s\n" , "SW_SHOWDEFAULT:10");
	fprintf(stderr, " %s\n" , "SW_FORCEMINIMIZE:11");

	exit(1);
}

int     
opterr = 1,             /* if error message should be printed */
optind = 1,             /* index into parent argv vector */
optopt,                 /* character checked for validity */
optreset;               /* reset getopt */
char    *optarg;        /* argument associated with option */

#define BADCH   (int)'?'
#define BADARG  (int)':'
#define EMSG    ""

int getopt(int nargc, char * const nargv[], const char *ostr)
{
	static char *place = EMSG;              /* option letter processing */
	const char *oli;                        /* option letter list index */

	if (optreset || !*place) {              /* update scanning pointer */
		optreset = 0;
		if (optind >= nargc || *(place = nargv[optind]) != '-') {
			place = EMSG;
			return (-1);
		}
		if (place[1] && *++place == '-') {      /* found "--" */
			++optind;
			place = EMSG;
			return (-1);
		}
	}                                       /* option letter okay? */
	if ((optopt = (int)*place++) == (int)':' ||
		!(oli = strchr(ostr, optopt))) {
		/*
		* if the user didn't specify '-' as an option,
		* assume it means -1.
		*/
		if (optopt == (int)'-')
			return (-1);
		if (!*place)
			++optind;
		if (opterr && *ostr != ':')
			(void)printf("illegal option -- %c\n", optopt);
		return (BADCH);
	}
	if (*++oli != ':') {                    /* don't need argument */
		optarg = NULL;
		if (!*place)
			++optind;
	}
	else {                                  /* need an argument */
		if (*place)                     /* no white space */
			optarg = place;
		else if (nargc <= ++optind) {   /* no arg */
			place = EMSG;
			if (*ostr == ':')
				return (BADARG);
			if (opterr)
				(void)printf("option requires an argument -- %c\n", optopt);
			return (BADCH);
		}
		else                            /* white space */
			optarg = nargv[optind];
		place = EMSG;
		++optind;
	}
	return (optopt);                        /* dump back option letter */
}

struct UserInfo
{
	DWORD process_id;
	int show_option;
};

BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam)
{
	UserInfo *userInfo = (UserInfo *)lParam;
	
	DWORD process_id = 0;
	DWORD thread_id = GetWindowThreadProcessId(hWnd, &process_id);
	
	if(process_id == userInfo->process_id)
	{
		if(!GetWindow(hWnd, GW_OWNER)) //non-owned
		{	
			GUITHREADINFO threadInfo;
			threadInfo.cbSize = sizeof(GUITHREADINFO);

			if (GetGUIThreadInfo(thread_id, &threadInfo))
			{
				if (
				hWnd == threadInfo.hwndActive ||
				hWnd == threadInfo.hwndFocus ||
				hWnd == threadInfo.hwndCapture ||
				hWnd == threadInfo.hwndMenuOwner ||
				hWnd == threadInfo.hwndMoveSize ||
				hWnd == threadInfo.hwndCaret)
				{
					ShowWindow(hWnd, userInfo->show_option);
				}
			}
		}
	}
	
	return TRUE;
}

int main(int argc, char *argv[])
{

	//default value for arguments
	UserInfo userInfo;
	userInfo.process_id = 0;
	userInfo.show_option = SW_SHOWNORMAL;
	
	int ch;
	
	while ((ch = getopt(argc, argv, OPT_LIST)) != -1){
		switch (ch){
			case OPT_PROCESS:
				userInfo.process_id = (DWORD)atoi(optarg);
				break;
			case OPT_SHOW:
				userInfo.show_option = atoi(optarg);
				break;
			case OPT_HELP:
			default:
				usage();
				break;
		}
	}

	if (!userInfo.process_id)
	{
		usage();
	}

	HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, userInfo.process_id);
	
	if(h)
	{
		EnumWindows(enumWindowsProc, (LPARAM)&userInfo);
		CloseHandle(h);
	}

	return 0;
}
