/*>
#Logging
## First define the target

We need a method to log statements like the following:
<*/
#include "log.h"

int main(int argc, char **argv) {
	log("all is %s", "ok");
}
/*>
If that is all, we could use `fprintf` instead. But there is more! There
should be a possibility to disable the logging on a call by call basis.

And the output should include the file and the line number where the log
occured:

```
main.c:4 all is ok
```

While we can do this also with a direct function call, it is tedious to
do so in C. And we want logging to super easy, so it will be used more
often and doesn't cludder the code too much.
<*/
