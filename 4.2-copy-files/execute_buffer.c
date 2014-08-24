/*>
The execution of the buffer is trivial, but can still fail.
<*/
static bool execute_buffer(char *buffer) {
	return_unless(buffer, false, "no buffer");
	return_unless(!system(buffer), false, "can't execute '%s'", buffer);
	return true;
}
