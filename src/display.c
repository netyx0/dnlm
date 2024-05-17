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

#include <netyx/cinderpelt.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "display.h"
#include "display_contents.h"

struct num_pair {
	int x;
	int y;
};

enum field {
	PROGRAM,
	USERNAME,
	PASSWORD
};

bool insert = false;
static struct num_pair error_field = {0, 0};
static struct num_pair program_field = {0, 0};
static struct num_pair username_field = {0, 0};
static struct num_pair password_field = {0, 0};
enum field current_field;

static int username_index;
static int password_index;
static char *error_message = NULL;

static void print_ui(struct login_info *l);
static void move_to_field(const enum field);
static void handle_key(const CP_key key, struct login_info *l);

static void handle_as_progfield(const CP_key key, struct login_info *l);
static void handle_as_usernamefield(const CP_key key, struct login_info *l);
static void handle_as_passwordfield(const CP_key key, struct login_info *l);
/* this one's here because theres a lot of overlap between usrname & passwd */
static void handle_as_field(const CP_key key, char *field,
                            const enum field next_field,
                            const enum field prev_field,
                            int *index_ptr);

static void write_program(const int command_id);
static void write_username(const char *username);
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

static void print_ui(struct login_info *l)
{
	int start_x;
	int start_y;

	cp_clear();
	cp_move_coords(1, 1);
	fputs("Shutdown: [F1]     Reboot: [F2]", stdout);

	start_x = (cp_max_x - MAIN_LEN) / 2;
	start_y = (cp_max_y - MAIN_HEIGHT) / 2;

	for (int i = 0; MAIN_BOX[i]; i++) {
		cp_move_coords(start_x, start_y + i);
		fputs(MAIN_BOX[i], stdout);
	}

	error_field.x = start_x + ERR_OFFSET_X;
	error_field.y = start_y + ERR_OFFSET_Y;

	program_field.x = start_x + PROG_OFFSET_X;
	program_field.y = start_y + PROG_OFFSET_Y;

	username_field.x = start_x + USR_OFFSET_X;
	username_field.y = start_y + USR_OFFSET_Y;

	password_field.x = start_x + PAS_OFFSET_X;
	password_field.y = start_y + PAS_OFFSET_Y;

	move_to_field(PROGRAM);
	fputs(conf_commands[l->command_id].name, stdout);

	/* move_to_field not used because index might not be 0 */
	cp_move_coords(username_field.x, username_field.y);
	fputs(l->username, stdout);

	write_error();

	fflush(stdout);
}

static void move_to_field(const enum field field)
{
	current_field = field;
	switch(field) {
	case PROGRAM:
		cp_move_coords(program_field.x, program_field.y);
		break;
	case USERNAME:
		cp_move_coords(username_field.x + username_index, username_field.y);
		break;
	case PASSWORD:
		cp_move_coords(password_field.x, password_field.y);
		break;
	}
	putchar('\0');
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
	handle_as_field(key, l->username, PASSWORD, PROGRAM, &username_index);
	write_username(l->username);
	move_to_field(current_field);
}

static void handle_as_passwordfield(const CP_key key, struct login_info *l)
{
	handle_as_field(key, l->password, PASSWORD, USERNAME, &password_index);
}

static void handle_as_field(const CP_key key, char *field,
                            const enum field next_field,
                            const enum field prev_field,
                            int *index_ptr)
{
	switch(key) {
	case CP_KEY_DOWN:
	case CP_KEY_ENTER:
	case CP_KEY_TAB:
		move_to_field(next_field);
		break;
	case CP_KEY_UP:
		move_to_field(prev_field);
		break;
	case CP_KEY_LEFT:
		if (*index_ptr == 0) {
			break;
		}
		(*index_ptr)--;
		cp_move_left(1);
		putchar('\0');
		fflush(stdout);
		break;
	case CP_KEY_RIGHT:
		if (!field[*index_ptr]) {
			break;
		}
		(*index_ptr)++;
		cp_move_right(1);
		putchar('\0');
		fflush(stdout);
		break;
	case CP_KEY_BACKSPACE:
		if (*index_ptr == 0) {
			break;
		}
		memmove(field + *index_ptr - 1,
		        field + *index_ptr,
		        strlen(field + *index_ptr) + 1);
		write_username(field);
		(*index_ptr)--;
		break;
	default:
		if (isprint((char)key)) {
			if (*index_ptr >= LOGIN_MAXLEN) {
				display_error("username too long");
				break;
			}
			if (insert) {
				field[(*index_ptr)++] = key;
			} else {
				memmove(field + *index_ptr + 1,
				        field + *index_ptr,
				        strlen(field + *index_ptr));
				field[(*index_ptr)++] = key;
			}
		}
	}
}

static void write_program(const int command_id)
{
	fflush(stdout);
	move_to_field(PROGRAM);
	for (int i = 0; i < PROG_LEN; i++) {
		putchar(' ');
	}
	move_to_field(PROGRAM);
	fputs(conf_commands[command_id].name, stdout);
	move_to_field(PROGRAM);
	fflush(stdout);
}

static void write_username(const char *name)
{
	cp_move_coords(username_field.x, username_field.y);
	for (int i = 0; i < USR_LEN; i++) {
		putchar(' ');
	}
	cp_move_coords(username_field.x, username_field.y);
	fputs(name, stdout);
	fflush(stdout);
}

static void write_error(void)
{
	cp_move_coords(error_field.x, error_field.y);
	for (int i = 0; i < ERR_LEN; i++) {
		putchar(' ');
	}
	cp_move_coords(error_field.x, error_field.y);
	if (error_message) {
		fputs(error_message, stdout);
		error_message = NULL;
	}
}

void display_error(char *message)
{
	error_message = message;
}
