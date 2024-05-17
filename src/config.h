#include "defs.h"

/* edit this file to configure whitestorm */

/* entries in the menu for choosing what program to start on login */
/* 0: name in the menu
 * 1: path to executable
 * 2:
*/
const struct command conf_commands[] = {
	/* name   path         */
	{"X11", "/bin/startx"},
	{"shell", NULL}, /* <= NULL means use $SHELL */
	{NULL, NULL}
};
#define NUM_COMMANDS (sizeof(conf_commands) / sizeof(struct command))

char * const conf_shutdown_cmd[] = {"/sbin/poweroff", NULL};
char * const conf_reboot_cmd[] = {"/sbin/reboot", NULL};

/* this function will be called at startup. it is intended to initialise the
 * header but tbh you can do whatever you want with it. the header starts at
 * (2, 2) and is 22 rows tall and 238 columns across.
*/
void init_header(void)
{
}

/* this function will be called at startup as well. it is intended to
 * initialise the footer.  the footer starts at (2, 57) and is 10 columns
 * tall and 238 columns across. if you want to do other things on startup,
 * doing that in this function is going to break things. put that in the
 * init_header function instead.
*/
void init_footer(void)
{
}
