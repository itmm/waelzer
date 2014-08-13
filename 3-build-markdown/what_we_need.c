/*>
## What we need

As always, we need to build on prior art and don't start from
scratch. In this case we need the logging and the string list
(to keep all our files, that we want to process). The string
list needs `str.h`, but we don't use it directly here, so we
wont include it.
<*/
#include "../2.1-logging/log.h"
#include "../2.4-string-list/str_lst.h"
/*>
The system headers are good old friends, with the exception
of `dirent.h`. We use it to parse the directory, if `content.txt`
is not provided.
<*/
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
