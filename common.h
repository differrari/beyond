#pragma once

extern char *indent;
#define MAX_DEPTH 64
#define indent_by(depth) (indent + (MAX_DEPTH-depth))