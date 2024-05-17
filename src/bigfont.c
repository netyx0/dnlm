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

#include <ctype.h>
#include <stdio.h>

#include "bigfont.h"

const char *BF_chars[][7] = {
	/* 0x20 space */
	{
		"      ",
		"      ",
		"      ",
		"      ",
		"      ",
		"      ",
		"      "
	},
	/* 0x21 exclaimation point */
	{
		"      ",
		"  |   ",
		"  |   ",
		"  |   ",
		"  .   ",
		"      ",
		"      "
	},
	/* 0x22 double quotes */
	{
		"      ",
		" ||   ",
		"      ",
		"      ",
		"      ",
		"      ",
		"      "
	},
	/* 0x23 hash */
	{
		"      ",
		"_|_|_ ",
		" | |  ",
		"_|_|_ ",
		" | |  ",
		"      ",
		"      "
	},
	/* 0x24 dollar sign */
	{
		"      ",
		" _|_  ",
		"|_|_  ",
		" _|_| ",
		"  |   ",
		"      ",
		"      "
	},
	/* 0x25 percent sign*/
	{
		" _    ",
		"|_| / ",
		"   /  ",
		"  /_  ",
		" /|_| ",
		"      ",
		"      "
	},
	/* 0x26 ampersand */
	{
		"  _   ",
		" / \\  ",
		" \\_/  ",
		"/  \\/ ",
		"\\__/\\ ",
		"      ",
		"      "
	},
	/* 0x27 single quote */
	{
		"      ",
		"  |   ",
		"      ",
		"      ",
		"      ",
		"      ",
		"      "
	},
	/* 0x28 opening parentheses */
	{
		"      ",
		"  /   ",
		" |    ",
		" |    ",
		"  \\   ",
		"      ",
		"      "
	},
	/* 0x29 closing parentheses */
	{
		"      ",
		"  \\   ",
		"   |  ",
		"   |  ",
		"  /   ",
		"      ",
		"      "
	},
	/* 0x2A asterisk */
	{
		"      ",
		" \\|/  ",
		" /|\\  ",
		"      ",
		"      ",
		"      ",
		"      "
	},
	/* 0x2B plus */
	{
		"      ",
		"      ",
		"      ",
		"__|__ ",
		"  |   ",
		"      ",
		"      "
	},
	/* 0x2C comma */
	{
		"      ",
		"      ",
		"      ",
		"      ",
		"  /   ",
		"      ",
		"      "
	},
	/* 0x2D minus */
	{
		"      ",
		"      ",
		"      ",
		" ____ ",
		"      ",
		"      ",
		"      "
	},
	/* 0x2E period */
	{
		"      ",
		"      ",
		"      ",
		"      ",
		"  *   ",
		"      ",
		"      "
	},
	/* 0x2F slash */
	{
		"      ",
		"    / ",
		"  _/  ",
		" /    ",
		"/     ",
		"      ",
		"      "
	},
	/* 0x30 - 0x39 '0' - '9' */
	{
		" ___  ",
		"|   | ",
		"|  /| ",
		"|/  | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		" __   ",
		"/ |   ",
		"  |   ",
		"  |   ",
		"__|__ ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"/   \\ ",
		"  __/ ",
		" /    ",
		"/____ ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"/   \\ ",
		"   _/ ",
		"    \\ ",
		"\\___/ ",
		"      ",
		"      "
	},
	{
		"  _   ",
		" / |  ",
		"/__|_ ",
		"   |  ",
		"   |  ",
		"      ",
		"      "
	},
	{
		" ____ ",
		"|     ",
		"|___  ",
		"    \\ ",
		"\\___/ ",
		"      ",
		"      "
	},
	{
		"      ",
		"  /   ",
		" /__  ",
		"/   \\ ",
		"\\___/ ",
		"      ",
		"      "
	},
	{
		"____  ",
		"    / ",
		" __/_ ",
		"  /   ",
		" /    ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"/   \\ ",
		"\\___/ ",
		"/   \\ ",
		"\\___/ ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"/   \\ ",
		"\\___/ ",
		"   /  ",
		" _/   ",
		"      ",
		"      "
	},
	/* 0x3A colon */
	{
		"      ",
		"      ",
		"  *   ",
		"      ",
		"  *   ",
		"      ",
		"      "
	},
	/* 0x3B semicolon */
	{
		"      ",
		"      ",
		"  *   ",
		"      ",
		"  /   ",
		"      ",
		"      "
	},
	/* 0x3C smaller than */
	{
		"      ",
		"   /  ",
		"  /   ",
		"  \\   ",
		"   \\  ",
		"      ",
		"      "
	},
	/* 0x3D equals */
	{
		"      ",
		"      ",
		" ___  ",
		" ___  ",
		"      ",
		"      ",
		"      "
	},
	/* 0x3E greater than */
	{
		"      ",
		"  \\   ",
		"   \\  ",
		"   /  ",
		"  /   ",
		"      ",
		"      "
	},
	/* 0x3F question mark */
	{
		" ___  ",
		"    | ",
		"   /  ",
		"      ",
		"  *   ",
		"      ",
		"      "
	},
	/* 0x40 at */
	{
		" ___  ",
		"/ _ \\ ",
		"| _|| ",
		"||_|/ ",
		"\\___  ",
		"      ",
		"      "
	},
	/* 0x41 - 0x5A 'A' - 'Z' */
	{
		" ___  ",
        "|   | ",
		"|___| ",
		"|   | ",
		"|   | ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"|   \\ ",
		"|___/ ",
		"|   \\ ",
		"|___/ ",
		"      ",
		"      "
	},
	{
		" ____ ",
		"/     ",
		"|     ",
		"|     ",
		"\\____ ",
		"      ",
		"      "
	},
	{
		" __   ",
		"|  \\  ",
		"|   | ",
		"|   | ",
		"|__/  ",
		"      ",
		"      "
	},
	{
		" ____ ",
		"|     ",
		"|____ ",
		"|     ",
		"|____ ",
		"      ",
		"      "
	},
	{
		" ____ ",
		"|     ",
		"|____ ",
		"|     ",
		"|     ",
		"      ",
		"      "
	},
	{
		" ____ ",
		"|     ",
		"|  _  ",
		"|   | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"|   | ",
		"|___| ",
		"|   | ",
		"|   | ",
		"      ",
		"      "
	},
	{
		"_____ ",
		"  |   ",
		"  |   ",
		"  |   ",
		"__|__ ",
		"      ",
		"      "
	},
	{
		"_____ ",
		"   |  ",
		"   |  ",
		"   |  ",
		"\\_/   ",
		"      ",
		"      "
	},
	{
		"      ",
		"|   / ",
		"|__/  ",
		"|  \\  ",
		"|   \\ ",
		"      ",
		"      "
	},
	{
		"      ",
		"|     ",
		"|     ",
		"|     ",
		"|____ ",
		"      ",
		"      "
	},
	{
		"      ",
		"|\\ /| ",
		"| | | ",
		"|   | ",
		"|   | ",
		"      ",
		"      "
	},
	{
		"      ",
		"|\\  | ",
		"| | | ",
		"| | | ",
		"|  \\| ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"|   | ",
		"|   | ",
		"|   | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"|   | ",
		"|___| ",
		"|     ",
		"|     ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"|   | ",
		"|   | ",
		"|   | ",
		"|__\\| ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"|   | ",
		"|___| ",
		"|\\__  ",
		"|   \\ ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"/   \\ ",
		"\\___  ",
		"    \\ ",
		"\\___/ ",
		"      ",
		"      "
	},
	{
		"_____ ",
		"  |   ",
		"  |   ",
		"  |   ",
		"  |   ",
		"      ",
		"      "
	},
	{
		"      ",
		"|   | ",
		"|   | ",
		"|   | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"|   | ",
		"|   | ",
		" \\ /  ",
		"  V   ",
		"      ",
		"      "
	},
	{
		"      ",
		"|   | ",
		"|   | ",
		"| | | ",
		"|_|_| ",
		"      ",
		"      "
	},
	{
		"      ",
		"\\   / ",
		" \\ /  ",
		" / \\  ",
		"/   \\ ",
		"      ",
		"      "
	},
	{
		"      ",
		"|   | ",
		" \\_/  ",
		"  |   ",
		"  |   ",
		"      ",
		"      "
	},
	{
		" ___  ",
		"    | ",
		"   /  ",
		" /    ",
		"|___  ",
		"      ",
		"      "
	},
	/* 0x5B open sq bracket */
	{
		"  __  ",
		" |    ",
		" |    ",
		" |    ",
		" |__  ",
		"      ",
		"      "
	},
	/* 0x5C backslash */
	{
		"      ",
		"\\     ",
		" \\_   ",
		"   \\  ",
		"    \\ ",
		"      ",
		"      "
	},
	/* 0x5D close sq bracket */
	{
		" __   ",
		"   |  ",
		"   |  ",
		"   |  ",
		" __|  ",
		"      ",
		"      "
	},
	/* 0x5E caret */
	{
		"      ",
		"  A   ",
		" / \\  ",
		"      ",
		"      ",
		"      ",
		"      "
	},
	/* 0x5F underscore */
	{
		"      ",
		"      ",
		"      ",
		"_____ ",
		"      ",
		"      ",
		"      "
	},
	/* 0x60 backtick */
	{
		"      ",
		" \\_   ",
		"      ",
		"      ",
		"      ",
		"      ",
		"      "
	},
	/* 0x61 - 0x7A 'a' - 'z' */
	{
		"      ",
        "      ",
		" ___  ",
		" ___| ",
		"|___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"|     ",
		"|___  ",
		"|   | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" ____ ",
		"|     ",
		"|____ ",
		"      ",
		"      "
	},
	{
		"      ",
		"    | ",
		" ___| ",
		"|   | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" ___  ",
		"|___| ",
		"|___  ",
		"      ",
		"      "
	},
	{
		"   __ ",
		"  |   ",
		" _|__ ",
		"  |   ",
		"  |   ",
		"      ",
		"      "
	} ,
	{
		"      ",
		"      ",
		" ___  ",
		"|   | ",
		"|___| ",
		"    | ",
		" ___| "
	},
	{
		"      ",
		"|     ",
		"|___  ",
		"|   | ",
		"|   | ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" _*   ",
		"  |   ",
		"__|__ ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" __*_ ",
		"   |  ",
		"   |  ",
		"   |  ",
		"\\__|  "
	},
	{
		"      ",
		"|     ",
		"|     ",
		"|___/ ",
		"|   \\ ",
		"      ",
		"      "
	},
	{
		" _    ",
		"  |   ",
		"  |   ",
		"  |   ",
		"__|__ ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" ___  ",
		"| | | ",
		"|   | ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" ___  ",
		"|   | ",
		"|   | ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" ___  ",
		"|   | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" ___  ",
		"|   | ",
		"|___| ",
		"|     ",
		"|     "
	},
	{
		"      ",
		"      ",
		" ___  ",
		"|   | ",
		"|___| ",
		"    | ",
		"    | "
	},
	{
		"      ",
		"      ",
		" ____ ",
		"|     ",
		"|     ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		" ___  ",
		"|___  ",
		" ___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"  |   ",
		"__|__ ",
		"  |   ",
		"  |_| ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		"      ",
		"|   | ",
		"|___| ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		"      ",
		"\\   / ",
		" \\_/  ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		"      ",
		"| | | ",
		"|_|_| ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		"_   _ ",
		" \\_/  ",
		"_/ \\_ ",
		"      ",
		"      "
	},
	{
		"      ",
		"      ",
		"      ",
		"|   | ",
		"|___| ",
		"    | ",
		" __/  "
	},
	{
		"      ",
		"      ",
		" ____ ",
		" ___/ ",
		"/___  ",
		"      ",
		"      "
	},
	/* 0x7B open brace */
	{
		"   _  ",
		"  /   ",
		" _\\   ",
		"  /   ",
		"  \\_  ",
		"      ",
		"      "
	},
	/* 0x7C pipe */
	{
		"      ",
		"  |   ",
		"  |   ",
		"  |   ",
		"  |   ",
		"      ",
		"      "
	},
	/* 0x7D close brace */
	{
		" _    ",
		"  \\   ",
		"  /_  ",
		"  \\   ",
		" _/   ",
		"      ",
		"      "
	},
	/* 0x 7E tilde */
	{
		"      ",
		"      ",
		" _    ",
		"/ \\_/ ",
		"      ",
		"      ",
		"      "
	}
};

void big_print(const char *string) {
	for (int i = 0; string[i]; i++) {
		big_putchar(string[i]);
	}
}

void big_putchar(int c)
{
	if (!isprint(c)) {
		return;
	}
	for (int i = 6; i >= 0; i--) {
		fputs(BF_chars[c - 0x20][i], stdout);
		fputs("\033[1A\033[6D", stdout);
	}
	fputs("\033[7B\033[6C", stdout);
}
