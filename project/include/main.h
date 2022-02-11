#ifndef PROJECT_INCLUDE_MAIN_H_
#define PROJECT_INCLUDE_MAIN_H_

typedef enum {
	STATE_BEGIN = 0,
	STATE_FROM,
	STATE_TO,
	STATE_DATE,
	STATE_NOTHING,
	STATE_COUNT,
	STATE_END,
	STATE_ERROR
} state_t;

typedef enum {
	LEXEM_FROM = 0,
	LEXEM_TO,
	LEXEM_DATE,
	LEXEM_CONTENT_TYPE,
	LEXEM_NOTHING,
	LEXEM_COUNT
} lexem_t;

typedef struct {
	char *from;
	char *to;
	char *date;
	int part_count;
} data_t;

typedef int (*action_t)(const char *str, const unsigned *offset, data_t *data);

typedef struct {
	state_t state;
	action_t action;
} rule_t;

#endif  // PROJECT_INCLUDE_MAIN_H_


