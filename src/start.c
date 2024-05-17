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

#include <sys/wait.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "auth.h"
#include "defs.h"

/* this limit is kind arbitrary, but most of the time argv0 aint that long */
#define SHELL_ARGV0_LEN 64

/* returns the name of the shell like the shell util basename but adds a
 * preceeding '-' so it can be passed to argv0 of the login shell
*/
static char *get_shell_argv0(char *shell);

int auth_and_start(const char *username, const char *password,
                   const char *cmd)
{
	struct passwd *pw;
	pid_t child;
	char *shell;
	char **env_from_pam;
	char *path;

	if (login(username, password)) {
		return 1;
	}
	pw = getpwnam(username);
	if (!pw) {
		return -1;
	}

	/* save any environment variables set by pam so to be restored later */
	env_from_pam = get_pam_envs();
	if (!env_from_pam) {
		return -1;
	}

	setenv("HOME", pw->pw_dir, 1);
	setenv("USER", username, 1);
	shell = pw->pw_shell ? pw->pw_shell : "/bin/sh";
	setenv("SHELL", shell, 1);
	if (pw->pw_uid == 0) {
		path = "/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin";
	} else {
		path = "/usr/local/bin:/bin:/usr/bin";
	}
	setenv("PATH", path, 1);
	setenv("LOGNAME", username, 1);
	setenv("MAIL", "placeholder mail", 1);
	setenv("TERM", "linux", 0);

	/* now that we're done messing PAM's environment variables up, fix 'em */
	for (int i = 0; env_from_pam[i]; i++) {
		putenv(env_from_pam[i]);
	}

	display_login_mode();

	child = fork();
	if (child == -1) {
		return -1;
	}
	if (child == 0) {
		initgroups(username, pw->pw_gid);
		setgid(pw->pw_gid);
		setuid(pw->pw_uid);

		chdir(pw->pw_dir);
		if (cmd) {
			execl(shell, get_shell_argv0(shell), "-c", cmd, NULL);
		} else {
			execl(shell, get_shell_argv0(shell), "-i", NULL);
		}
	} else {
		waitpid(child, NULL, 0);
	}

	display_display_mode();

	return 0;
}

static char *get_shell_argv0(char *shell)
{
	static char ret[SHELL_ARGV0_LEN];
	char *c;

	while ((c = strchr(shell, '/')) && *(c + 1)) {
		shell = c + 1;
	}
	sprintf(ret, "-%*s", SHELL_ARGV0_LEN - 2, shell);

	return ret;
}
