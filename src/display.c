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

#include <netyx/cursedpotato.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "bigfont.h"
#include "defs.h"
#include "display.h"
#include "display_contents.h"

enum field {
	PROGRAM,
	USERNAME,
	PASSWORD
};

bool insert = false;
enum field current_field;

static int username_index;
static int username_display_index;
static int password_index;
static char *error_message = NULL;

static void print_ui(struct login_info *l);
static void move_to_field(const enum field);
static void handle_key(const CP_key key, struct login_info *l);

static void handle_as_progfield(const CP_key key, struct login_info *l);
static void handle_as_usernamefield(const CP_key key, struct login_info *l);
static void handle_as_passwordfield(const CP_key key, struct login_info *l);
static void write_program(const int command_id);
static void write_username(const char *username, int offset, int len);
static void write_error(void);

void display_init(void)
{
	cp_init();
	cp_uncook();
}

enum action display(struct login_info *l)
{
	CP_key key;

	username_index = strlen(l->username);
	if (strlen(l->username) > USR_LEN) {
		username_display_index = strlen(l->username) - USR_LEN;
	} else {
		username_display_index = 0;
	}
	password_index = 0;

	print_ui(l);
	move_to_field(USERNAME);
	for (;;) {
		key = cp_read_key();
		switch(key) {
		case CP_KEY_F1:
			return SHUTDOWN;
		case CP_KEY_F2:
			return REBOOT;
		case CP_KEY_ENTER:
			if (current_field == PASSWORD) {
				display_error("Logging in...");
				write_error();
				move_to_field(PASSWORD); /* this flushes stdout */
				return LOGIN;
			} /* fallthrough! */
		default:
			handle_key(key, l);
		}
	}
}

/* this cooks the terminal and resets everything for the logged in process */
void display_login_mode(void)
{
	cp_clear();
	cp_cook();
}

/* this does the opposite */
void display_display_mode(void)
{
	cp_uncook();
}

static void print_ui(struct login_info *l)
{
	cp_clear();
	fputs(conf_colour_string, stdout);
	cp_move_coords(1, 1);
	fputs(DISPLAY_BASE, stdout);

	init_header();
	init_footer();

	move_to_field(PROGRAM);
	big_print(conf_commands[l->command_id].name);

	/* move_to_field not used because index might not be 0 */
	cp_move_coords(USR_FIELD_X, USR_FIELD_Y);
	big_print(l->username);

	write_error();
	fflush(stdout);
}

static void move_to_field(const enum field field)
{
	current_field = field;
	switch(field) {
	case PROGRAM:
		cp_move_coords(PROG_FIELD_X, PROG_FIELD_Y);
		break;
	case USERNAME:
		cp_move_coords(
			USR_FIELD_X + (username_index - username_display_index)*6 + 3,
		    USR_FIELD_Y);
		break;
	case PASSWORD:
		cp_move_coords(PAS_FIELD_X, PAS_FIELD_Y);
		break;
	}
	fflush(stdout);
}

static void handle_key(const CP_key key, struct login_info *l)
{
	const void (*key_funcs[])(CP_key, struct login_info *) = {
		handle_as_progfield,
		handle_as_usernamefield,
		handle_as_passwordfield
	};

	if (key == CP_KEY_INSERT) {
		insert = !insert;
	} else {
		key_funcs[current_field](key, l);
	}
}

static void handle_as_progfield(const CP_key key, struct login_info *l)
{
	switch(key) {
	case CP_KEY_DOWN:
	case CP_KEY_ENTER:
		move_to_field(USERNAME);
		break;
	case CP_KEY_RIGHT:
		if (conf_commands[l->command_id + 1].name) {
			l->command_id++;
		} else {
			l->command_id = 0;
		}
		write_program(l->command_id);
		break;
	case CP_KEY_LEFT:
		if (l->command_id > 0) {
			l->command_id--;
		} else {
			while(conf_commands[l->command_id + 1].name) {
				l->command_id++;
			}
		}
		write_program(l->command_id);
		break;
	default:
		/* do nothing */
	}
}

static void handle_as_usernamefield(const CP_key key, struct login_info *l)
{
	switch(key) {
	case CP_KEY_DOWN:
	case CP_KEY_ENTER:
	case CP_KEY_TAB:
		move_to_field(PASSWORD);
		break;
	case CP_KEY_UP:
		move_to_field(PROGRAM);
		break;
	case CP_KEY_LEFT:
		if (username_index == 0) {
			/* do nothing */
		} else {
			username_index--;
			if (username_display_index > 0 &&
			    username_display_index == username_index) {
				username_display_index--;
			}
		}
		break;
	case CP_KEY_RIGHT:
		if (!l->username[username_index]) {
			break;
		}
		username_index++;
		if (username_index - username_display_index + 1 == USR_LEN &&
		    username_display_index < strlen(l->username) - USR_LEN) {
			username_display_index++;
		}
		break;
	case CP_KEY_BACKSPACE:
		if (username_index == 0) {
			break;
		}
		memmove(l->username + username_index - 1,
		        l->username + username_index,
		        strlen(l->username + username_index) + 1);
		username_index--;
		if (username_display_index > 0) {
			username_display_index--;
		}
		break;
	case CP_KEY_DEL:
		memmove(l->username + username_index,
		        l->username + username_index + 1,
		        strlen(l->username + username_index + 1) + 1);
		if (username_display_index + USR_LEN > strlen(l->username) &&
		    username_display_index > 0) {
			username_display_index--;
		}
		break;
	default:
		if (isprint((char)key)) {
			if (username_index >= LOGIN_MAXLEN) {
				display_error("username too long");
				write_error();
				break;
			}
			if (insert) {
				l->username[username_index++] = key;
			} else {
				memmove(l->username + username_index + 1,
				        l->username + username_index,
				        strlen(l->username + username_index));
				l->username[username_index++] = key;
				if (strlen(l->username) > USR_LEN) {
					username_display_index++;
				}
			}
		}
	}
	write_username(l->username, username_display_index, USR_LEN);
	move_to_field(current_field);
}

static void handle_as_passwordfield(const CP_key key, struct login_info *l)
{
	switch(key) {
	case CP_KEY_DOWN:
	case CP_KEY_ENTER:
	case CP_KEY_TAB:
		move_to_field(PROGRAM);
		break;
	case CP_KEY_UP:
		move_to_field(USERNAME);
		break;
	case CP_KEY_LEFT:
		if (password_index == 0) {
			break;
		}
		password_index--;
		break;
	case CP_KEY_RIGHT:
		if (!l->password[password_index]) {
			break;
		}
		password_index++;
		break;
	case CP_KEY_BACKSPACE:
		if (password_index == 0) {
			break;
		}
		memmove(l->password + password_index - 1,
		        l->password + password_index,
		        strlen(l->password + password_index) + 1);
		password_index--;
		break;
	case CP_KEY_DEL:
		if (l->username[username_index]) {
			memmove(l->username + username_index,
			        l->username + username_index + 1,
			        strlen(l->username + username_index + 1) + 1);
		}
		break;
	default:
		if (isprint((char)key)) {
			if (password_index >= LOGIN_MAXLEN) {
				display_error("password too long");
				write_error();
				break;
			}
			if (insert) {
				l->password[password_index++] = key;
			} else {
				memmove(l->password + password_index + 1,
				        l->password + password_index,
				        strlen(l->password + password_index));
				l->password[password_index++] = key;
			}
		}
	}
}

static void write_program(const int command_id)
{
	fflush(stdout);
	move_to_field(PROGRAM);
	for (int i = 0; i < PROG_LEN; i++) {
		big_print(" ");
	}
	move_to_field(PROGRAM);
	big_print(conf_commands[command_id].name);
	move_to_field(PROGRAM);
	fflush(stdout);
}

static void write_username(const char *username, int offset, int len)
{
	cp_move_coords(USR_FIELD_X, USR_FIELD_Y);
	for (int i = 0; i < USR_LEN; i++) {
		big_print(" ");
	}
	cp_move_coords(USR_FIELD_X, USR_FIELD_Y);
	for (int i = offset; i < offset + len; i++) {
		big_putchar(username[i]);
	}
	fflush(stdout);
}

static void write_error(void)
{
	cp_move_coords(ERR_FIELD_X, ERR_FIELD_Y);
	for (int i = 0; i < ERR_LEN; i++) {
		big_print(" ");
	}

	if (error_message) {
		cp_move_coords(ERR_FIELD_X, ERR_FIELD_Y);
		big_print(error_message);
		error_message = NULL;
	}
}

void display_error(char *message)
{
	error_message = message;
}
