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

#ifndef DEFS_H
#define DEFS_H

struct command {
	char *name;
	char *cmd;
};
extern const struct command conf_commands[];

enum action {
	LOGIN,
	SHUTDOWN,
	REBOOT
};

#define LOGIN_MAXLEN 1023 /* this limit is arbitrary, but afaik GNU useradd */
                          /* has a limit of 32 chars so this is pretty safe */
struct login_info {
	char username[LOGIN_MAXLEN + 1];
	char password[LOGIN_MAXLEN + 1];
	int command_id;
};

extern void(*header_init_func)(void);

#endif /* ifndef DEFS_H */
