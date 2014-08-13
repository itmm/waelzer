/*>
To check for a match in a direntry, we have to check the length first.
<*/
static bool entry_matches(struct dirent *entry, const char *name) {
	size_t length = strlen(name);
	return entry->d_namlen == length && !strcmp(entry->d_name, name);
}
/*>
The suffix check contains a grain of magic. We can use good ol' `strcmp`
to peform the check, if we know where to start it. We need to shift the
candidate by its length minus the suffix length.

Of course this only makes sense, if the suffix length is smaller.
<*/
static bool has_suffix(struct dirent *entry, const char *suffix) {
	size_t length = strlen(suffix);
	return entry->d_namlen >= length && !strcmp(entry->d_name + entry->d_namlen - length, suffix);
}
/*>
The reverse comparator is simply the reverse of `strcmp`. We could also
exchange the parameters, but I prefer the negation.
<*/
static int reverse_strcmp(const void*a, const void *b) {
	return -strcmp(a, b);
}
