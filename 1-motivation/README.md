A vision about writing Software
===============================

To understand a computer program it is often fruitless to
look at the current source code. That is all that is needed
to build the application, but not to understand its internal
structure.

There are multiple ways to improve the understanding of the
code. Donald E. Knuth introduced WEB to mix documentation
and code (first in Pascal, then in C). But the problem
remains: even with thorough documentation, big problems like
TeX or METAfont are very difficult to grasp. And these
projects are small compared with the Apache Web Server or
the Linux kernel.

What is missing is the development of software. Each C
program begins with code around the lines

``` c
int main(int argc, char **argv) {
	return;
}
```

From there on the project evolved to a complex project in
small steps.

If you followed the project from the early start, all the
small steps are easier to digest than the complete final
state.

This project should reformulate the progress of developing
software. The software project should be organized in
small chapters, sections and subsections. Each part describes
a change in the project to build the final product.