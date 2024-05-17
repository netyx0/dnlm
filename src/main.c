/*
 * This file is part of whitestorm.
 *
 * whitestorm is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * whitestorm is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with whitestorm. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

#include "auth.h"
#include "config.h"
#include "start.h"

/* this struct is just setting us up for ace lmao */
struct login_info {
	char username[1024];
	char password[1024];
	char *cmd;
};
struct login_info *display_menu(void);

int main(void)
{
	int result;
	struct login_info *l;

	/* main loop */
	for (;;) {
		l = display_menu();

		printf("attempting login with creds [%s:%s]\n", l->username, l->password);

		result = auth_and_start(l->username, l->password, l->cmd);
		printf("process returned %d\n", result);
		if (result <= 0) {
			logout();
		}
	}
}

/* wow this is a mess. dont worry though, this is a placeholder */
struct login_info *display_menu(void)
{
	static struct login_info l;
	int entry;
	int c;

	// clear
//	printf("\033[2J\033[0;0H");
	for (int i = 0; i < NUM_COMMANDS; i++) {
		printf("%d: %s\n", i, conf_commands[i].name);
	}
	printf("select an entry[0-%ld]: ", NUM_COMMANDS - 1);
	scanf(" %d", &entry);
	if (entry < 0 || entry >= NUM_COMMANDS) {
		puts("bad selection");
		return display_menu();
	}
	printf("username: ");
	c = getchar();
	if (c == EOF) {
		return NULL;
	}
	ungetc(c, stdin);
	scanf("%1023s", l.username);
	printf("password: ");
	c = getchar();
	if (c == EOF) {
		return NULL;
	}
	ungetc(c, stdin);
	scanf("%1023s", l.password);

	l.cmd = conf_commands[entry].cmd;
	return &l;
}
