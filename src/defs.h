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

#ifndef DEFS_H
#define DEFS_H

struct command {
	char *name;
	char *cmd;
};
extern const struct command conf_commands[];
extern const char *conf_colour_string;
void init_header(void);
void init_footer(void);

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

void display_login_mode(void);
void display_display_mode(void);


#endif /* ifndef DEFS_H */
