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
