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

#include <stdlib.h>
#include <string.h>

#include "auth.h"
#include "display.h"

static pam_handle_t *handle;
static int status;

#define do_pam_thing(func, flag) \
	((status = (func)(handle, (flag))) != PAM_SUCCESS)
static void on_pam_error(void);
static int convfunc(int num_msg, const struct pam_message **msg,
                    struct pam_response **resp, void *appdata_ptr);

/* returns 0 if login was successful and 1 if it failed */
int login(const char *username, const char *passwd)
{
	const char *credentials[2] = {
		username,
		passwd
	};

	const struct pam_conv convstruct = {
		convfunc, credentials
	};

	status = pam_start("whitestorm", username, &convstruct, &handle);
	if (status != PAM_SUCCESS) {
		display_error(pam_strerror(handle, status));
		return 1;
	}

	if (do_pam_thing(pam_authenticate, 0) ||
	    do_pam_thing(pam_acct_mgmt, 0) ||
		do_pam_thing(pam_setcred, PAM_ESTABLISH_CRED)
	) {
		on_pam_error();
		return 1;
	}

	if (do_pam_thing(pam_open_session, 0)) {
		status = pam_setcred(handle, PAM_DELETE_CRED);
		on_pam_error();
		return 1;
	}

	return 0;
}

void logout(void)
{
	if(do_pam_thing(pam_setcred, PAM_DELETE_CRED));

	pam_end(handle, status);
}

static void on_pam_error()
{
	display_error(pam_strerror(handle, status));
	pam_end(handle, status);
}

static int convfunc(int num_msg, const struct pam_message **msg,
                    struct pam_response **resp, void *appdata_ptr)
{
	*resp = malloc(num_msg * sizeof(struct pam_response));
	if (!*resp) {
		return PAM_BUF_ERR;
	}

	for (int i = 0; i < num_msg; i++) {
		(*resp)[i].resp_retcode = 0;
		switch(msg[i]->msg_style) {
		case PAM_PROMPT_ECHO_ON:
			/* asking for the username */
			(*resp)[i].resp = strdup(((char **)appdata_ptr)[0]);
			break;
		case PAM_PROMPT_ECHO_OFF:
			/* password */
			(*resp)[i].resp = strdup(((char **)appdata_ptr)[1]);
			break;
		case PAM_ERROR_MSG:
			/* todo: do something with the message maybe? */
			for (int j = 0; j < i; j++) {
				free((*resp)[j].resp);
			}
			free(resp);
			return PAM_CONV_ERR;
			break;
		case PAM_TEXT_INFO:
			/* todo: add handler */
			(*resp)[i].resp = NULL;
			break;
		}
	}
	return PAM_SUCCESS;
}
