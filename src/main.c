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

#include "auth.h"

int main(void)
{
	char username[1024];
	char password[1024];
	int result;
	int c;

	for (;;) {
		printf("username: ");
		c = getchar();
		if (c == EOF) {
			break;
		}
		ungetc(c, stdin);
		scanf("%1023s", username);
		printf("password: ");
		c = getchar();
		if (c == EOF) {
			break;
		}
		ungetc(c, stdin);
		scanf("%1023s", password);

		printf("attempting login with creds:\n[%s:%s]\n", username, password);
		result = login(username, password);

		if (!result) {
			puts("login was successful!");
			logout();
		}
	}
}
