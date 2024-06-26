/* Copyright 2024 Alex Lo
 *
 * This file is part of dnlm.
 *
 * dnlm is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * dnlm is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with dnlm. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "auth.h"
#include "config.h"
#include "defs.h"
#include "display.h"
#include "start.h"

void run_in_background(char * const cmd[]);

int main(void)
{
	struct login_info l;

	display_init();
	memset(l.username, '\0', LOGIN_MAXLEN);
	memset(l.password, '\0', LOGIN_MAXLEN);
	l.command_id = 0;

	sleep(1);

	/* exit after logging in because pam_systemd (and pam_elogind) doesn't
	 * actually close the session until the process that opened it terminates
	 * so we are relying on the supervising daemon to restart us
	*/
	switch(display(&l)) {
	case LOGIN:
		/* if it returns 1, PAM has set the error, 0 if success. */
		if (auth_and_start(l.username, l.password,
		    conf_commands[l.command_id].cmd) == -1) {
			display_error("Forking failed");
		}
		logout();
		return 0;
	case SHUTDOWN:
		run_in_background(conf_shutdown_cmd);
	case REBOOT:
		run_in_background(conf_reboot_cmd);
	default:
		abort();
	}
}

void run_in_background(char * const cmd[])
{
	pid_t pid;

	display_login_mode();

	pid = fork();
	if (pid < 0 || pid == 0) {
		/* run in child if it exist but fallback to parent */
		execv(cmd[0], cmd);
	}
	for(;;);
}
