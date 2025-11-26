#include <stdio.h>
#include "funcs.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


float get_user_input_custom(const char *variable_name){
    char buf[128];
    int valid_input = 0;
    float value = 0;

        do {
        printf("\nEnter value for %s: ", variable_name);
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }

        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_number(buf)) {              // Custom check if number valid
            printf("Enter a valid number (integer or float)!\n");
            valid_input = 0;
        } else {
            value = strtof(buf, NULL);
            valid_input = 1;
        }

    } while (!valid_input);

    return value;
}

int is_number(const char *s)  // Modified based on is_int template
{
    int seen_digit = 0;     // These prevent illegal inputs (multiple dots, exponent in the wrong place...
    int seen_dot = 0;
    int seen_exp = 0;

    if (!s || !*s) return 0;       // Reject NULL or empty

    /* optional sign */
    if (*s == '+' || *s == '-') s++;    // Allow sign because voltage might be negative (although negative voltage is not typically used)

    while (*s) {
        if (isdigit((unsigned char)*s)) {
            seen_digit = 1;
        }
        else if (*s == '.') {           // only one dot allowed and not after exponent
            if (seen_dot || seen_exp)
                return 0;
            seen_dot = 1;
        }
        else if (*s == 'e' || *s == 'E') {  // exponent only once and only after at least one digit 
            if (seen_exp || !seen_digit)
                return 0;
            seen_exp = 1;
            s++;

            if (*s == '+' || *s == '-') s++;        // sign after exponent eg 1e-3
            if (!isdigit((unsigned char)*s)) return 0;   // at least 1 digit behind exponent
            continue;
        }
        else {
            return 0; 
        }
        s++;
    }

    return seen_digit;
}
