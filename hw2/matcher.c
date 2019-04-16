/**
 * As in every CS skeleton, the contents of this file are
 * only a suggestion. Please feel free to change anything.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "matcher.h"

#define ANY -1
#define REPEAT_FLAG '+'
#define OPTIONAL_FLAG '?'
#define ESCAPE_FLAG '\\'
#define WILDCARD '.'
#define TRUE 1
#define FALSE 0

enum regex_state_type {
    START,
    ACCEPT,
    REPEAT, // '+'
    OPTIONAL, // '?'
    MATCH
};

struct regex_state {
    enum regex_state_type type;
    int c;
    struct regex_state *successor;
};

struct regex_pattern {
    struct regex_state *start;
};

static struct regex_pattern *create_pattern(const char *str);

static void free_pattern(struct regex_pattern *pattern);

static int is_next_repeat(const char *str, int len ,int next_index);

static int is_next_optional(const char *str, int len, int current_index);

static struct regex_state *create_state(enum regex_state_type type, int c,
    struct regex_state *succesor);

static void fatal(const char *message);

static int match_pattern(const char *line, const struct regex_pattern *pattern);

static int match_state(const char *line, const struct regex_state *state);

static int match(int actual, int expected);

/**
 * Parses pattern for next character and sets
 * necessary parameters that are referenced inside/outside
 * of this function.
 *
 * Add any additional inputs to function signature if necessary.
 */
char *parse_char(char *partial_pattern, char *target) {
    // You can use this recommended helper function, or not.
    
    return '\0';
}

/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {
  // You can use this recommended helper function, or not.

    return 0;
}

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 * 
 * This function is REQUIRED and is not a suggestion.
 */
int rgrep_matches(char *line, char *pattern) {

    //
    // TODO: put your code here.
    //

    if (strlen(line) == 0 && strlen(pattern) == 0) {
        return TRUE;
    }

    struct regex_pattern *p = create_pattern(pattern);
    int ret = FALSE;
    char *current = line;
    while (*current != '\0') {
        if (match_pattern(current, p)) {
            ret = TRUE;
            break;
        }
        current++;
    }

    free_pattern(p);
    return ret;
}

static struct regex_pattern *create_pattern(const char *str) {
    struct regex_pattern *pattern = malloc(sizeof(struct regex_pattern));
    if (pattern == NULL) {
        fatal("Unable to allocate memory");
    }

    struct regex_state *current_state = create_state(START, ANY, NULL);
    pattern->start = current_state;

    size_t len = strlen(str); // without line feed.
    for (size_t i = 0; i < len;) {
        int current_char = str[i];
        int effective_char;
        struct regex_state *next_state;
        if (current_char == REPEAT_FLAG || current_char == OPTIONAL_FLAG) {
            free_pattern(pattern);
            return NULL;
        } else if (current_char == ESCAPE_FLAG) {
            if (i + 1 >= len) {
                free_pattern(pattern);
                return NULL;
            } else {
                effective_char = *(str + i + 1);
                i++;
            }
        } else {
            effective_char = current_char == WILDCARD ? ANY : current_char;
        }

        if (is_next_optional(str, len, i + 1)) {
            next_state = create_state(OPTIONAL,
                effective_char, NULL);
            i += 2;
        } else if (is_next_repeat(str, len, i + 1)) {
            next_state = create_state(MATCH, effective_char, NULL);
            current_state->successor = next_state;
            current_state = next_state;
            next_state = create_state(REPEAT, effective_char, NULL);
            i += 2;
        } else {
            next_state = create_state(MATCH, effective_char, NULL);
            i++;
        }

        current_state->successor = next_state;
        current_state = next_state;
    }

    current_state->successor = create_state(ACCEPT, ANY, NULL);
    return pattern;
}

int match_pattern(const char *line, const struct regex_pattern *pattern) {
    return match_state(line, pattern->start);
}

int match_state(const char *line, const struct regex_state *state) {
    if (state->type == ACCEPT) {
        return TRUE;
    }
    if (*line == '\0') {
        return FALSE;
    }

    switch (state->type) {
        case START:
            return match_state(line, state->successor);
        case MATCH:
            if (match(*line, state->c)) {
                return match_state(line + 1, state->successor);
            } else {
                return FALSE;
            }
        case REPEAT:
            if (match(*line, state->c) && match_state(line + 1, state)) {
                return TRUE;
            } else {
                return match_state(line, state->successor);
            }
        case OPTIONAL:
            if (match(*line, state->c) && match_state(line + 1, state->successor)) {
                return TRUE;
            } else {
                return match_state(line, state->successor);
            }
        default:
            return FALSE;
    }
}

static void free_pattern(struct regex_pattern *pattern) {
    if (pattern == NULL) {
        return;
    }

    struct regex_state *current = pattern->start;
    while (current != NULL) {
        struct regex_state *next = current->successor;
        free(current);
        current = next;
    }
    free(pattern);
}

static int is_next_repeat(const char *str, int len, int next_index) {
    if (next_index < len && str[next_index] == REPEAT_FLAG) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static int is_next_optional(const char *str, int len, int next_index) {
    if (next_index < len && str[next_index] == OPTIONAL_FLAG) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static struct regex_state *create_state(enum regex_state_type type, int c,
        struct regex_state *successor) {
    struct regex_state *state = malloc(sizeof(struct regex_state));
    if (state == NULL) {
        fatal("Unable to allocate memory when parsing pattern");
    }

    state->c = c;
    state->successor = successor;
    state->type = type;

    return state;
}

static void fatal(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

static int match(int actual, int expected) {
    if (actual == expected || expected == ANY) {
        return TRUE;
    } else {
        return FALSE;
    }
}