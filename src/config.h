/* edit this file to configure whitestorm */

struct command {
	char *name;
	char *cmd;
};

/* entries in the menu for choosing what program to start on login */
/* 0: name in the menu
 * 1: path to executable
 * 2:
*/
struct command conf_commands[] = {
	/* name   path         */
	{"xinit", "/bin/startx"},
	{"shell", NULL} /* <= NULL means use $SHELL */
};
#define NUM_COMMANDS (sizeof(conf_commands) / sizeof(struct command))
