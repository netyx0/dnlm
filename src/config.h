#include "defs.h"

/* edit this file to configure whitestorm */

/* entries in the menu for choosing what program to start on login */
/* 0: name in the menu
 * 1: path to executable
 * 2:
*/
const struct command conf_commands[] = {
	/* name   path         */
	{"xinit", "/bin/startx"},
	{"shell", NULL}, /* <= NULL means use $SHELL */
	{NULL, NULL}
};
#define NUM_COMMANDS (sizeof(conf_commands) / sizeof(struct command))

char * const conf_shutdown_cmd[] = {"/sbin/poweroff", NULL};
char * const conf_reboot_cmd[] = {"/sbin/reboot", NULL};

/* this function will be called at startup at position (2, 2), which is the
 * header area. it is 22 rows tall and 238 columns long. the position where
 * the cursor is when it returns doesn't matter.
*/
void init_header(void)
{
}
