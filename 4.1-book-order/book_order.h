/*>
# Find the Section Order

All sections start with a sequence of numbers separated by dots followed by
a hyphen. The order should compare by number, not by digit, e.g.
`2.6` < `10.2` < `10.3`.

## Interface

The function `book_order` sorts a string list according to this order.
<*/
#if !defined(book_order_h)
#define book_order_h

	#if !defined(str_lst_h)
		#include "../2.4-string-list/str_lst.h"
	#endif

	#include <stdbool.h>

	bool book_order(str_lst *strs);

#endif
