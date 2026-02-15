#include "symbol_serialize.h"
#include "files/buffer.h"
#include "common.h"
#include "files/helpers.h"

static buffer buf;

void write_serial_table(symbol_table *table, int indent){
    buffer_write(&buf,"TABLE %s\n",sem_rule_to_string(table->table_type));
    for (int i = 0; i < table->symbol_count; i++){
        symbol_t *sym = &table->symbol_table[i];
        if (!sym){
            print("INVALID SYMBOL %i",i);
            return;
        }
        if (sym->name.length){//TODO: hack
            buffer_write(&buf,"SYMBOL = %s TYPE = %v SUBTYPE %v NAME = %v REF = %i\n",sem_rule_to_string(sym->sym_type),sym->type.kind ? token_to_slice(sym->type) : make_string_slice("none", 0, 4),sym->subtype.kind ? token_to_slice(sym->subtype) : make_string_slice("none", 0, 4),sym->name,sym->reference_type);
        }
        if (sym->child){
            write_serial_table(sym->child,indent+1);
        }
    }
    buffer_write(&buf,"END TABLE %s\n",sem_rule_to_string(table->table_type));
}

void serialize_table(symbol_table *table, const char *path){
    buf = buffer_create(0x1000, buffer_can_grow);
    write_serial_table(table, 0);
    write_full_file(path, buf.buffer, buf.buffer_size);
}

#define MAX_STS 64

symbol_table *st_stack[MAX_STS];
u64 st_stack_count;

void read_symbol_line(void *ctx, string_slice line){
    symbol_table *table = ctx;
    Scanner scan = scanner_make(line.data, line.length);
    string_slice v1 = scan_to(&scan, ' ');
    if (*(char*)(v1.data + v1.length - 1) == ' ')
        v1.length--;
    print("Reading line %v",v1);
    if (slice_lit_match(v1, "TABLE", true)){
        symbol_table *nt = new_table();
        table->symbol_table[table->symbol_count++].child = nt;
        print("Found table at %v",line);
        st_stack[st_stack_count++] = table;
        table = nt;
    }
    if (slice_lit_match(v1, "SYMBOL", true)){
        symbol_table *nt = new_table();
        table->symbol_table[table->symbol_count++].name = v1;
    }
    if (slice_lit_match(v1, "END", true)){
        print("Found end of table at %v",line);
        st_stack[--st_stack_count] = table;
    }
}

symbol_table* deserialize_table(const char *path){
    const char *symbols = read_full_file(path, 0);
    symbol_table *table = new_table();
    read_lines((char*)symbols, table, read_symbol_line);
    return table;
}