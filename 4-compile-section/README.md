# Compiling a section

We want to run our own build process for a section.

For each section from the beginning up to the specified (or newest) section all source files must be copied into a build directory. If later sections replace some files, these files must be overwritten.

Then the targets should be build that are specified in the specified section.

As a first step, we need process the section directories in the right order.