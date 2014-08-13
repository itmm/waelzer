#if !defined(str_lst_h)
#define str_lst_h

	#include <stdbool.h>

	typedef struct str_lst str_lst;

	str_lst *str_lst_create(int min_capacity);
	void str_lst_free(str_lst *sl);

	int str_lst_count(str_lst *sl);
	char **str_lst_begin(str_lst *sl);
	char **str_lst_end(str_lst *sl);

	bool str_lst_add(str_lst *sl, const char *str);
	bool str_lst_add_and_consume(str_lst *sl, char *str);

#endif
