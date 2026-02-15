#pragma once

#include "sem_analysis.h"

void serialize_table(symbol_table *table, const char *path);
symbol_table* deserialize_table(const char *path);