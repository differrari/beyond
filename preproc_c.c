       
       
// typedef unsigned int uint32_t;
// typedef signed __int128 int128_t;
// typedef unsigned __int128 uint128_t;
// typedef unsigned long int size_t;
// typedef unsigned long long uint64_t;
// typedef unsigned long long uintptr_t;
// typedef unsigned short uint16_t;
// typedef unsigned char uint8_t;
// typedef signed int int32_t;
// typedef long long int64_t;
// typedef long long intptr_t;
// typedef signed short int16_t;
// typedef signed char int8_t;
// typedef unsigned char bool;
// typedef struct sizedptr {
//     uintptr_t ptr;
//     size_t size;
// } sizedptr;
// typedef signed char tern;
// typedef uint32_t u32;
// typedef uint64_t u64;
// typedef int128_t i128;
// typedef uint128_t u128;
// typedef uint16_t u16;
// typedef uint8_t u8;
// typedef uintptr_t uptr;
// typedef uint64_t paddr_t;
// typedef uint64_t uaddr_t;
// typedef uint64_t kaddr_t;
// typedef char* literal;
// typedef char** strarr;
// typedef size_t usize;
// typedef int32_t i32;
// typedef int64_t i64;
// typedef int16_t i16;
// typedef int8_t i8;
// typedef struct {
//     uptr start;
//     size_t size;
// } range_t;
       
       
       
// typedef void* (*page_allocator)(size_t size);
// void* allocate(void* page, size_t size, page_allocator fallback);
// void* zalloc(size_t size);
// void release(void* ptr);
// void* reallocate(void* ptr, size_t new_size);
// bool test_zalloc();
// typedef u32 color;
// typedef union argbcolor {
//     struct {
//         u32 blue: 8;
//         u32 green: 8;
//         u32 red: 8;
//         u32 alpha: 8;
//     };
//     u32 color;
// } argbcolor;
// typedef struct {
//     i32 x;
//     i32 y;
// } gpu_point;
// typedef struct {
//     i32 x;
//     i32 y;
// } int_point;
// typedef struct {
//     u32 width;
//     u32 height;
// } gpu_size;
// typedef struct {
//     gpu_point point;
//     gpu_size size;
// } gpu_rect;
// typedef struct draw_ctx {
//     gpu_rect dirty_rects[64];
//     uint32_t* fb;
//     uint32_t stride;
//     uint32_t width;
//     uint32_t height;
//     uint32_t dirty_count;
//     bool full_redraw;
// } draw_ctx;
// static inline draw_ctx buffer_to_draw_ctx(void *buf, i32 width, i32 height){
//     return (draw_ctx){
//         .dirty_rects = {},
//         .fb = (u32*)buf,
//         .stride = (u32)width * (u32)sizeof(color),
//         .width = (u32)width,
//         .height = (u32)height,
//         .dirty_count = 0,
//         .full_redraw = 0,
//     };
// }
// static inline draw_ctx dummy_draw_ctx(i32 width, i32 height){
//     return buffer_to_draw_ctx(zalloc(width * height * sizeof(color)), width, height);
// }
// static inline gpu_rect draw_ctx_rect(draw_ctx *ctx){
//     return (gpu_rect){
//         .point = {},
//         .size = {ctx->width,ctx->height}
//     };
// }
       
// typedef struct {
//     uint8_t modifier;
//     uint8_t rsvd;
//     char keys[6];
// } keypress;
// typedef enum in_event_type { KEY_RELEASE, KEY_PRESS, KEY_CONTINUE, MOD_RELEASE, MOD_PRESS } in_event_type;
// typedef struct {
//     in_event_type type;
//     unsigned char key;
//     unsigned char modifier;
// } kbd_event;
       
// typedef struct {
//     uint8_t buttons;
//     int8_t x;
//     int8_t y;
//     int8_t scroll;
// } mouse_input;
// enum { LMB, RMB, MMB };
// typedef struct {
//     mouse_input raw;
//     gpu_point position;
// } mouse_data;
// static inline bool mouse_button_down(mouse_data* m, uint8_t button){
//     return (m->raw.buttons >> button) & 1;
// }
       
       
// typedef __builtin_va_list va_list;
// typedef struct {
//     char *data;
//     uint32_t length;
//     uint32_t mem_length;
// } string;
// typedef struct string_list {
//     uint32_t count;
//     char array[];
// } string_list;
// static inline bool is_whitespace(char c){
//     return
//     c == '\t' ||
//     c == '\n' ||
//     c == ' ' ||
//     c == '\r' ||
//     0;
// }
// static inline bool is_printable(char c){
//     return (c >= ' ' && c <= '~') || (c >= '\t' && c <= '\r');
// }
// static inline bool is_alpha(char c) {
//     return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
// }
// static inline bool is_digit(char c) {
//     return (c >= '0' && c <= '9');
// }
// static inline bool is_alnum(char c) {
//     return is_alpha(c) || is_digit(c);
// }
// static inline int32_t str_has_char(const char* s, uint32_t max, char c){
//     for (uint32_t i = 0; (!max || i < max) && s[i]; i++) if (s[i] == c) return i;
//     return -1;
// }
// static inline int hex_val(char c) {
//     if (is_digit(c)) return c - '0';
//     if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
//     if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
//     return -1;
// }
// uint32_t u64_to_base(char *tmp, uint64_t v, unsigned base, int upper);
// size_t strlen_max(const char *s, uint32_t max_length);
// string string_from_literal(const char *literal);
// string string_from_literal_length(const char *array, uint32_t max_length);
// string string_from_char(const char c);
// string string_from_hex(uint64_t value);
// bool string_equals(string a, string b);
// string string_replace(const char *str, char orig, char repl);
// string string_format(const char *fmt, ...);
// size_t string_format_buf(char *out, size_t cap, const char *fmt, ...);
// string string_format_va(const char *fmt, va_list args);
// size_t string_format_va_buf(const char *fmt, char *out, size_t cap, va_list args);
// string string_tail(const char *array, uint32_t max_length);
// string string_repeat(char symbol, uint32_t amount);
// static inline void string_free(string str){
//     if (str.data && str.mem_length) release(str.data);
// }
// int tolower(int c);
// int toupper(int c);
// int strcmp_case(const char *a, const char *b, bool case_insensitive);
// int strncmp_case(const char *a, const char *b, bool case_insensitive, size_t length);
// bool strcont(const char *a, const char *b);
// int strstart_case(const char *a, const char *b, bool case_insensitive);
// int strend_case(const char *a, const char *b, bool case_insensitive);
// int strindex(const char *a, const char *b);
// int count_occurrences(const char* str, char c);
// uint64_t parse_hex_u64(const char* str, size_t size);
// uint64_t parse_int_u64(const char* str, size_t size);
// int64_t parse_int64(const char* str, size_t size);
// float parse_float(char *input,size_t length);
// double parse_double(char *input,size_t length);
// bool utf16tochar(uint16_t* str_in, char* out_str, size_t max_len);
// void strcat_buf(const char *a, const char *b, char *dest);
// char* strcat_new(const char *a, const char *b);
// string string_from_const(const char *literal);
// string string_concat(string a, string b);
// void string_concat_inplace(string *dest, string src);
// void string_append_bytes(string *dest, const void *buf, uint32_t len);
// const char* seek_to(const char *string, char character);
// char* strncpy(char* dst, const char* src, size_t cap);
// bool parse_uint32_dec(const char *s, uint32_t *out);
// char* strcasestr(const char* haystack, const char* needle);
// string string_replace_character(char* original, char symbol, char *value);
// static inline int strstart(const char *a, const char *b) { return strstart_case(a,b,0); }
// size_t strlen(const char *s);
// int strcmp(const char *a, const char *b);
// int strncmp(const char *a, const char *b, size_t length);
// int strend(const char *a, const char *b);
       
// typedef enum {
//     IP_VER4 = 4,
//     IP_VER6 = 6
// } ip_version_t;
// typedef struct __attribute__((aligned(8))) net_l4_endpoint {
//     ip_version_t ver;
//     uint8_t ip[16];
//     uint16_t port;
// } net_l4_endpoint;
// typedef enum {
//     PROTO_UDP = 0,
//     PROTO_TCP = 1
// } protocol_t;
// typedef enum Socket_Role {
//     SOCKET_CLIENT,
//     SOCKET_SERVER,
// } Socket_Role;
// typedef struct SocketHandle {
//     uint16_t id;
//     net_l4_endpoint connection;
//     protocol_t protocol;
// } SocketHandle;
// typedef enum {
//     IP_TX_AUTO = 0,
//     IP_TX_BOUND_L2 = 1,
//     IP_TX_BOUND_L3 = 2
// } ip_tx_scope_t;
// typedef struct {
//     uint8_t index;
//     ip_tx_scope_t scope;
// } ip_tx_opts_t;
       
       
// typedef uint64_t (*hash_map_hash_fn)(const void* key, uint64_t len);
// typedef int (*hash_map_keyeq_fn)(const void* a, uint64_t alen, const void* b, uint64_t blen);
// typedef struct hash_map_entry_t {
//     uint64_t hash;
//     void* key;
//     uint64_t key_len;
//     void* value;
//     struct hash_map_entry_t* next;
// } hash_map_entry_t;
// typedef struct hash_map {
//     hash_map_entry_t** buckets;
//     uint64_t capacity;
//     uint64_t size;
//     void* (*alloc)(size_t size);
//     void (*free)(void* ptr);
//     hash_map_hash_fn hash_fn;
//     hash_map_keyeq_fn keyeq_fn;
//     void (*value_dispose)(void* value);
//     uint64_t resize_threshold;
// } hash_map_t;
// hash_map_t* hash_map_create(uint64_t initial_capacity);
// hash_map_t* hash_map_create_alloc(uint64_t initial_capacity, void* (*alloc)(size_t size),void (*mfree)(void* ptr));
// void hash_map_destroy(hash_map_t* map);
// void hash_map_set_allocator(hash_map_t* map, void* (*alloc)(size_t), void (*dealloc)(void*));
// void hash_map_set_hash(hash_map_t* map, hash_map_hash_fn hash_fn, hash_map_keyeq_fn keyeq_fn);
// void hash_map_set_value_dispose(hash_map_t* map, void (*dispose_fn)(void*));
// int hash_map_put(hash_map_t* map, const void* key, uint64_t key_len, void* value);
// int hash_map_put_dictionary(hash_map_t* map, const char* key, void* value);
// void* hash_map_get(const hash_map_t* map, const void* key, uint64_t key_len);
// void* hash_map_get_dictionary(const hash_map_t *map, const char *str);
// bool hash_map_remove(hash_map_t* map, const void* key, uint64_t key_len, void** out_value);
// void hash_map_empty(hash_map_t* map);
// uint64_t hash_map_size(const hash_map_t* map);
// uint64_t hash_map_capacity(const hash_map_t* map);
// void hash_map_for_each(const hash_map_t* map, void (*func)(void* key, uint64_t key_len, void* value));
// uint64_t hash_map_fnv1a64(const void* data, uint64_t len);
       
// typedef u64 data_signature;
// typedef enum {
//     cursor_sync_none,
//     cursor_sync_follow,
//     cursor_sync_head,
//     cursor_sync_tail
// } cursor_sync;
// typedef struct file {
//     u64 id;
//     size_t size;
//     u64 cursor;
//     data_signature data_type;
// } file;
// typedef uint64_t file_offset;
// typedef enum SEEK_TYPE {
//     SEEK_ABSOLUTE,
//     SEEK_RELATIVE
// } SEEK_TYPE;
// typedef enum FS_RESULT {
//     FS_RESULT_DRIVER_ERROR,
//     FS_RESULT_SUCCESS,
//     FS_RESULT_NOTFOUND,
// } FS_RESULT;
// static inline uint64_t reserve_fd_gid(const char *path){
//     return hash_map_fnv1a64(path, strlen(path));
// }
       
// typedef enum {
//     BIND_L3 = 0,
//     BIND_L2 = 1,
//     BIND_IP = 2,
//     BIND_ANY = 3
// } SockBindKind;
// typedef enum {
//     DST_ENDPOINT = 0,
//     DST_DOMAIN = 1
// } SockDstKind;
// typedef enum {
//     SOCK_OPT_DEBUG = 1u << 0,
//     SOCK_OPT_KEEPALIVE = 1u << 1,
//     SOCK_OPT_BUF_SIZE = 1u << 2,
//     SOCK_OPT_DONTFRAG = 1u << 3,
//     SOCK_OPT_TTL = 1u << 4,
//     SOCK_OPT_MCAST_JOIN = 1u << 5
// } SockOptFlags;
// typedef enum {
//     SOCK_DBG_LOW = 0,
//     SOCK_DBG_MEDIUM = 1,
//     SOCK_DBG_ALL = 2
// } SockDebugLevel;
// typedef struct SocketExtraOptions {
//     uint32_t flags;
//     SockDebugLevel debug_level;
//     uint32_t buf_size;
//     uint32_t keepalive_ms;
//     uint8_t ttl;
//     ip_version_t mcast_ver;
//     uint8_t mcast_group[16];
// } SocketExtraOptions;
// typedef struct SockBindSpec{
//     SockBindKind kind;
//     ip_version_t ver;
//     uint8_t l3_id;
//     uint8_t ifindex;
//     uint8_t ip[16];
// } SockBindSpec;
       
       
// typedef enum {
//     buffer_opt_none = 0,
//     buffer_can_grow = 1 << 0,
//     buffer_circular = 1 << 1,
//     buffer_static = 1 << 2,
//     buffer_read_only = 1 << 3,
// } buffer_options;
// typedef struct {
//     void* buffer;
//     size_t buffer_size;
//     size_t limit;
//     buffer_options options;
//     uintptr_t cursor;
//     data_signature data_type;
// } buffer;
// buffer buffer_create(size_t size, buffer_options options);
// static inline void buffer_map_value(buffer *buf, void* value, size_t size, data_signature signature){
//     if (!buf || !value || !size) return;
//     buf->buffer = value;
//     buf->buffer_size = size;
//     buf->limit = size;
//     buf->options = buffer_static;
//     buf->data_type = signature;
// }
// size_t buffer_write(buffer *buf, char* fmt, ...);
// size_t buffer_write_va(buffer *buf, char* fmt, va_list args);
// size_t buffer_write_const(buffer *buf, const char *lit);
// size_t buffer_write_lim(buffer *buf, const char *lit, size_t size);
// size_t buffer_write_to(buffer *buf, const char *lit, size_t size, uintptr_t cursor);
// size_t buffer_write_space(buffer *buf);
// size_t buffer_read(buffer *buf, void *into, size_t size, uintptr_t cursor);
// size_t buffer_delete(buffer *buf, uptr cursor, size_t amount);
// void buffer_wipe(buffer *buf);
// void buffer_destroy(buffer *buf);
// uptr buffer_seek(buffer *buf, i64 amount, bool absolute);
       
// typedef struct {
//     char *data;
//     size_t length;
// } string_slice;
// string_slice make_string_slice(const char* buf, size_t start, size_t length);
// static inline string string_from_slice(string_slice slice){
//     return string_from_literal_length(slice.data, slice.length);
// }
// static inline string_slice slice_from_string(string str){
//     return (string_slice){.data = str.data,.length = str.length};
// }
// static inline string_slice slice_from_literal(const char* lit){
//     return (string_slice){.data = (char*)lit, .length = strlen(lit)};
// }
// static inline string_slice ptr_to_slice(sizedptr parent){
//     return (string_slice){.data = (char*)parent.ptr, .length = parent.size};
// }
// static inline bool slices_equal(string_slice sl1, string_slice sl2, bool case_insensitive){
//     if (sl1.length != sl2.length) return 0;
//     for (size_t i = 0; i < sl1.length; i++)
//         if (sl1.data[i] != sl2.data[i] && (!case_insensitive || tolower(sl1.data[i]) != tolower(sl2.data[i]))) return 0;
//     return 1;
// }
// static inline bool slice_lit_match(string_slice sl, const char *lit, bool case_insensitive){
//     return slices_equal(sl, make_string_slice(lit,0,strlen(lit)), case_insensitive);
// }
// static inline sizedptr slice_to_sizedptr(string_slice slice){
//     return (sizedptr){.ptr = (uptr)slice.data,.size = slice.length};
// }
// static inline int slice_index(string_slice a, const char *b){
//     for (u32 i = 0; i < a.length; i++){
//         int j = 0;
//         while (b[j] && i+j < a.length && a.data[i + j] == b[j]) j++;
//         if (!b[j]) return i;
//     }
//     return -1;
// }
// string_slice slice_trim_ws(string_slice slice, bool include_newline);
// bool slice_ends_with(string_slice slice, string_slice seek);
// bool slice_starts_with(string_slice slice, string_slice seek);
// typedef struct {
//     char seek;
//     char *str;
//     size_t length;
//     string_slice current;
//     uptr pointer;
//     bool allow_empty;
// } string_splitter;
// void string_split(const char *str, char seek, void (*perform)(string_slice slice));
// static inline string_splitter make_string_splitter(const char *str, char seek, bool allow_empty){
//     return (string_splitter){
//         .seek = seek,
//         .str = (char*)str,
//         .length = strlen(str),
//         .current = (string_slice){},
//         .pointer = 0,
//         .allow_empty = allow_empty
//     };
// }
// static inline string_splitter make_string_splitter_slice(string_slice slice, char seek, bool allow_empty){
//     return (string_splitter){
//         .seek = seek,
//         .str = slice.data,
//         .length = slice.length,
//         .current = (string_slice){},
//         .pointer = 0,
//         .allow_empty = allow_empty
//     };
// }
// bool string_splitter_advance(string_splitter *splitter);
// bool string_quick_split(char *str, char seek, string_slice *lhs, string_slice *rhs);
// static inline string_slice string_splitter_get_current(string_splitter *splitter){
//     string_slice ret = splitter->current;
//     if (ret.length && ret.data[ret.length-1] == splitter->seek) ret.length--;
//     return ret;
// }
// static inline string_slice string_splitter_remaining(string_splitter *splitter){
//     if (splitter->pointer >= splitter->length) return (string_slice){};
//     return (string_slice){.data = splitter->str + splitter->pointer, .length = splitter->length - splitter->pointer};
// }
// string_slice slice_seek_to(string_slice slice, char character);
// static inline string_slice slice_from_buffer(buffer *buf){
//     return (string_slice){ .data = (char*)buf->buffer, .length = buf->buffer_size };
// }
// static inline sizedptr sizedptr_from_buffer(buffer *buf){
//     return (sizedptr){ .ptr = (uptr)buf->buffer, .size = buf->buffer_size };
// }
// bool buffer_test();
// typedef enum { backing_physical, backing_virtual, backing_command, backing_transform } fs_backing_type;
// typedef enum { entry_invalid, entry_file, entry_directory } fs_entry_type;
// typedef struct {
//     size_t size;
//     fs_entry_type type;
//     data_signature data_type;
// } fs_stat;
// typedef FS_RESULT (*file_open_fn)(const char*, file*);
// typedef size_t (*file_read_fn)(file*, char*, size_t, file_offset);
// typedef size_t (*file_write_fn)(file*, const char *, size_t, file_offset);
// typedef void (*file_close_fn)(file *descriptor);
// typedef bool (*file_getstat_fn)(const char*, fs_stat*);
// typedef size_t (*file_readdir_fn)(const char*, void*, size_t, file_offset*);
// typedef bool (*file_truncate_fn)(file*);
// typedef size_t (*file_transform_fn)(const char*, void*, size_t);
// typedef u64 (*cmd_fn)(void* ctx, size_t len);
// typedef struct {
//     file_open_fn open;
//     file_read_fn read;
//     file_write_fn write;
//     file_close_fn close;
//     file_truncate_fn truncate;
//     file_getstat_fn getstat;
//     file_readdir_fn readdir;
//     file_transform_fn transform;
// } file_actions;
// typedef struct {
//     string alias_path;
//     file alias_fd;
// } alias_info_t;
// typedef enum {
//     fs_permission_none,
//     fs_permission_shared,
//     fs_permission_instanced,
//     fs_permission_request,
// } fs_permission_level;
// typedef struct module_file {
//     string name;
//     alias_info_t alias_info;
//     fs_backing_type backing_type;
//     fs_entry_type entry_type;
//     uint64_t fid;
//     uint64_t serial;
//     void *private_data;
//     data_signature data_type;
//     buffer file_buffer;
//     uint64_t references;
//     file_actions actions;
// } module_file;
// typedef struct system_module {
//     const char* name;
//     const char* mount;
//     uint64_t version;
//     u16 owner;
//     bool (*init)(struct system_module *);
//     bool (*fini)();
//     file_open_fn open;
//     file_read_fn read;
//     file_write_fn write;
//     file_close_fn close;
//     file_truncate_fn truncate;
//     file_getstat_fn getstat;
//     file_readdir_fn readdir;
//     file_transform_fn transform;
//     alias_info_t alias_info;
// } system_module;
       
// typedef enum {
//     SIG_NONE,
//     SIG_KILL,
//     SIG_QUIT,
//     SIG_STOP,
//     SIG_CONT,
//     SIG_KBD,
//     SIG_MOUSE,
//     NUMBER_SIGNALS
// } signal_types;
// typedef struct {
//     signal_types type;
//     u16 sender;
//     i64 value;
// } signal_info_t;
// typedef bool (*signal_handler)(signal_info_t *signal);
// static inline bool can_signal_be_handled(signal_types type){
//     return
//         type != SIG_KILL &&
//         type != SIG_STOP &&
//         type != SIG_CONT &&
//         1;
// }
// static inline bool signal_is_immediate(signal_types type){
//     return !can_signal_be_handled(type);
// }
       
       
       
// typedef struct vector2 {
//     float x,y;
// } vector2;
// typedef vector2 vec2;
// typedef struct {
//     vec2 pos;
//     u8 on_curve;
//     bool rsvd[3];
// } point_entry;
// typedef enum { point_graph_curve_none, point_graph_curve_quad_only, point_graph_curve_quad_cube } point_graph_curve_type;
// typedef struct {
//     int start;
//     int end;
//     bool close;
//     point_graph_curve_type curve_type;
//     i8 max_curve_points;
// } point_graph_slice;
// typedef struct {
//     int num_points;
//     point_entry *graph;
//     int num_slices;
//     point_graph_slice *slices;
//     vec2 size;
//     vec2 offset;
// } point_graph;
// typedef enum {
//     path_render_fill = 1 << 0,
//     path_render_outline = 1 << 1,
//     path_render_background = 1 << 2,
// } path_render_type;
// typedef struct {
//     u32 scale;
//     color background_color;
//     color outline_color;
//     color color;
//     path_render_type type;
// } path_render_options;
       
       
       
// typedef struct chunk_array_t {
//     size_t chunk_capacity;
//     size_t item_size;
//     size_t count;
//     struct chunk_array_t *next;
//     void* (*allocator)(size_t size);
//     void (*free)(void*);
// } chunk_array_t;
// chunk_array_t* chunk_array_create_alloc(size_t item_size, size_t chunk_capacity, void* (*allocator)(size_t size), void (*free)(void*));
// chunk_array_t* chunk_array_create(size_t item_size, size_t chunk_capacity);
// size_t chunk_array_push(chunk_array_t* array, void *data);
// void chunk_array_destroy(chunk_array_t *array);
// void chunk_array_reset(chunk_array_t *array);
// int chunk_array_remove(chunk_array_t *array, int count);
// void* chunk_array_get(chunk_array_t *array, uint64_t index);
// size_t chunk_array_count(chunk_array_t *array);
// void* chunk_array_find(chunk_array_t *array, void *query, bool (*match)(void* value, void *query));
// bool chunk_array_test();
// typedef chunk_array_t arr_stack_t;
       
// typedef enum { font_type_embedded , font_type_ttf } font_type;
// typedef struct {
//     font_type type;
//     int num_glyphs;
//     string name;
// } font_hdr;
// font_hdr* font_load(const char *location, font_type type);
// void font_destroy(font_hdr *hdr);
// gpu_size font_render_glyph_index(font_hdr *hdr, path_render_options options, draw_ctx *ctx, gpu_point location, u32 index);
// gpu_size font_render_glyph_character(font_hdr *hdr, path_render_options options, draw_ctx *ctx, gpu_point location, u32 character);
       
// void mark_dirty(draw_ctx *ctx, u32 x, uint32_t y, u32 w, u32 h);
// typedef enum { wrap_none, wrap_word, wrap_word_preserve_indent } wrap_policy;
// typedef struct {
//     path_render_options glyph;
//     font_hdr *font;
//     bool crop;
//     float line_spacing;
//     float character_spacing;
// } simple_text_format;
// typedef struct {
//     u32 scale;
//     color foreground;
//     color background;
//     range_t bounds;
//     wrap_policy wrap;
// } text_format;
// typedef enum { fmt_array_none, fmt_array_static, fmt_array_stack } fmt_array_type;
// typedef struct {
//     fmt_array_type array_type;
//     void *fmt;
//     size_t count;
// } text_format_arr;
// static inline text_format_arr stack_to_text_format(arr_stack_t *s){
//     return (text_format_arr){ .array_type = fmt_array_stack, .fmt = s, .count = chunk_array_count(s) };
// }
// static inline text_format_arr array_to_text_format(text_format *ptr, size_t count){
//     return (text_format_arr){ .array_type = fmt_array_static, .fmt = ptr, .count = count };
// }
// u32 fb_get_line_spacing(int scale);
// u32 fb_char_width(u32 scale);
// u32 fb_line_height(u32 scale);
// typedef enum { draw_text_render, draw_text_delete, draw_text_rerender } draw_text_op;
// typedef struct {
//     gpu_size size;
//     gpu_point cursor;
// } text_draw_result;
void fb_continuous_draw_text(draw_ctx *ctx, draw_text_op operation, gpu_point *cursor, string_slice slice, range_t *render_range, gpu_rect bounds, gpu_size *out_size, gpu_point scroll, text_format default_format, text_format_arr array);
// text_draw_result fb_draw_single_text(draw_ctx *ctx, string_slice slice, gpu_rect bounds, gpu_point scroll, text_format format);
// text_draw_result fb_draw_text(draw_ctx *ctx, string_slice slice, gpu_rect bounds, gpu_point scroll, text_format default_format, text_format_arr array);
// u32 lin_col_to_pos(i32 line, i32 col, string_slice content);
// void pos_to_lin_col(u32 pos, string_slice content, i32 *lin, i32 *col);
// gpu_size fb_write_slice(draw_ctx *ctx, string_slice slice, gpu_point loc, simple_text_format options);
// void fb_draw_raw_char(draw_ctx *ctx, uint32_t x, uint32_t y, char c, uint32_t scale, uint32_t color);
// static inline void fb_draw_char(draw_ctx *ctx, uint32_t x, uint32_t y, char c, uint32_t scale, uint32_t color){
//     fb_draw_raw_char(ctx, x, y, c, scale, color);
//     mark_dirty(ctx, x,y,8*scale,8*scale);
// }
// static inline gpu_size fb_draw_slice(draw_ctx *ctx, string_slice slice, uint32_t x0, uint32_t y0, uint32_t scale, uint32_t color){
//     return fb_write_slice(ctx, slice, (gpu_point){(i32)x0, (i32)y0}, (simple_text_format){
//         .glyph = {
//             .scale = scale,
//             .background_color = 0,
//             .outline_color = 0,
//             .color = color,
//             .type = path_render_fill,
//         },
//         .font = 0,
//         .crop = 0,
//         .line_spacing = 0,
//         .character_spacing = 0
//     });
// }
// static inline gpu_size fb_draw_string(draw_ctx *ctx, const char* s, uint32_t x0, uint32_t y0, uint32_t scale, uint32_t color){
//     return fb_draw_slice(ctx, slice_from_literal(s), x0, y0, scale, color);
// }
// typedef struct {
//     uint32_t img_width;
//     uint32_t img_height;
//     uint32_t start_x;
//     uint32_t start_y;
//     bool flip_x;
//     bool flip_y;
// } image_transform;
// u32 pixel_blend(u32 p1, u32 p2);
// void fb_clear(draw_ctx *ctx, uint32_t color);
// void fb_draw_pixel(draw_ctx *ctx, uint32_t x, uint32_t y, color color);
// void fb_fill_rect(draw_ctx *ctx, int32_t x, int32_t y, uint32_t width, uint32_t height, color color);
// void fb_outline_rect(draw_ctx *ctx, i32 x, i32 y, u32 width, u32 height, u32 size, color color);
// void fb_fill_partial_rect(draw_ctx *ctx, uint32_t x, uint32_t y, uint32_t width, uint32_t height, color color, uint32_t start_x, uint32_t start_y, uint32_t full_width);
// void fb_draw_img(draw_ctx *ctx, uint32_t x, uint32_t y, uint32_t *img, uint32_t img_width, uint32_t img_height);
// void fb_draw_partial_img(draw_ctx *ctx, uint32_t *img, uint32_t x, uint32_t y, uint32_t full_width, uint32_t full_height, image_transform transform);
// gpu_rect fb_draw_line(draw_ctx *ctx, i32 x0, i32 y0, i32 x1, i32 y1, color color);
// uint32_t fb_get_char_size(uint32_t scale);
// void fb_draw_cursor(draw_ctx *ctx, uint32_t color);
       
// typedef enum {
//     TINSPECT_NONE = 0,
//     TINSPECT_CONTROL = 1 << 1,
//     TINSPECT_TRACE = 1 << 2,
//     TINSPECT_INFO = 1 << 3,
//     TINSPECT_STATE = 1 << 4,
//     TINSPECT_INPUT = 1 << 5,
//     TINSPECT_OUTPUT = 1 << 6
// } debug_inspect_types;
       
// typedef enum {
//     pipe_default = 0,
//     pipe_from_beginning = 1,
// } pipe_options;
// enum {
//     EXEC_MODE_DEFAULT = 0,
//     EXEC_MODE_KEEP_FOCUS = 1,
// };
// extern void printl(const char *str);
// int print(const char *fmt, ...);
// void put(const char *fmt, ...);
// extern void serial_transmit(u8);
// extern void* page_alloc(size_t size);
// extern void page_free(void* ptr);
// extern bool read_key(keypress *kp);
// extern bool read_event(kbd_event *event);
// extern void get_mouse_status(mouse_data *in);
// extern void msleep(uint64_t time);
// extern __attribute__((noreturn)) void halt(int32_t exit_code);
// extern __attribute__((noreturn)) void halt_thread(int32_t exit_code);
// extern int32_t exec(const char* prog_name, int argc, const char* argv[], u32 mode);
// extern int32_t kill_process(uint16_t pid);
// extern void request_draw_ctx(draw_ctx*);
// static inline void request_app_ctx(draw_ctx* ctx){ request_draw_ctx(ctx); }
// extern void commit_draw_ctx(draw_ctx*);
// extern void resize_draw_ctx(draw_ctx*, u32 width, u32 height);
// extern void destroy_draw_ctx(draw_ctx *ctx);
// extern bool should_close_ctx();
// extern uint32_t gpu_char_size(uint32_t scale);
// extern uint64_t get_time();
// extern bool socket_create(Socket_Role role, protocol_t protocol, const SocketExtraOptions* extra, SocketHandle *out_handle);
// extern int32_t socket_bind(SocketHandle *handle, ip_version_t ip_version, uint16_t port);
// extern int32_t socket_connect(SocketHandle *handle, SockDstKind dst_kind, void* dst, uint16_t port);
// extern int32_t socket_listen(SocketHandle *handle);
// extern bool socket_accept(SocketHandle *spec);
// extern size_t socket_send(SocketHandle *handle, SockDstKind dst_kind, const void* dst, uint16_t port, void *packet, size_t size);
// extern bool socket_receive(SocketHandle *handle, void *packet, size_t size, net_l4_endpoint* out_src);
// extern int32_t socket_close(SocketHandle *handle);
// extern FS_RESULT openf(const char* path, file* descriptor);
// extern FS_RESULT pipef(const char *src, const char *dest, pipe_options options, file* descriptor);
// extern size_t readf(file *descriptor, char* buf, size_t size);
// extern size_t writef(file *descriptor, const char* buf, size_t size);
// extern size_t sreadf(const char* path, void* buf, size_t size);
// extern size_t swritef(const char* path, const void* buf, size_t size, bool append);
// extern size_t transformf(const char* path, void* buf, size_t size);
// extern void closef(file *descriptor);
// extern bool statf(const char *path, fs_stat *out_stat);
// extern bool truncatef(file*, size_t);
// extern bool load_fsmodule(system_module *mod, bool global);
// extern bool unload_fsmodule();
// extern bool send_signal(signal_types type, u16 proc_id);
// extern bool handle_signal(signal_types type, signal_handler handler);
// void seek(file *descriptor, int64_t offset, SEEK_TYPE type);
// void* realloc_sized(void* old_ptr, size_t old_size, size_t new_size);
// void *calloc(size_t nitems, size_t size);
// size_t dir_list(const char *path, void *buf, size_t size, u64 *offset);
// int system(const char *command);
// int system_focus(const char *command, u32 focus_mode);
// void in_case_of_js_break_glass();
// void register_behavior();
// bool thread_inspect(debug_inspect_types type, u16 pid, u16 tid);
// static inline void yield(){
//     msleep(0);
// }
// static inline void* malloc(size_t size){
//     return zalloc(size);
// }
// static inline void free_sized(void* ptr, size_t size){
//     release(ptr);
// }
       
       
       
       
       
// typedef struct shell_handle shell_handle;
// typedef enum {
//     console_ctrl_none,
//     console_ctrl_close
// } console_ctrls;
// typedef struct {
//     void (*console_output)(shell_handle*, char output);
//     void (*console_flush)(shell_handle*);
//     void (*console_clean)(shell_handle*);
//     void (*console_bell)(shell_handle*);
//     void (*console_ascii_cmd)(shell_handle*, char cmd, u16 proc_id);
//     void (*console_control)(shell_handle*, console_ctrls);
// } shell_bindings;
// typedef struct {
//     string current_directory;
// } shell_ctx;
// typedef struct {
//     void *ctx;
//     bool (*eval)(string_slice exp, void*);
//     bool (*is_script)(string_slice cmd);
// } shell_script_ctx;
// struct shell_handle {
//     buffer out_buffer;
//     shell_bindings bindings;
//     shell_ctx *common_ctx;
//     void *local_ctx;
//     shell_script_ctx scripting;
//     void *owner;
//     bool (*cmd_input)(shell_handle*, string_slice input);
// };
// extern shell_handle* current_shell;
// void new_shell(shell_handle *, shell_bindings bindings, void (*init_shell)(shell_handle *handle));
// void register_func();
// bool shell_interpret(shell_handle *, string_slice);
// bool run_cmd(shell_handle *, string_slice);
// static inline void shell_print_specify_newline(shell_handle *handle, bool newline, char *fmt, ...){
//     if (!handle || !handle->out_buffer.buffer) return;
//     __attribute__((aligned(16))) va_list args;
//     __builtin_va_start(args, fmt);
//     uptr cursor = handle->out_buffer.cursor;
//     size_t size = buffer_write_va(&handle->out_buffer, fmt, args);
//     if (newline) size += buffer_write_const(&handle->out_buffer, "\n");
//     __builtin_va_end(args);
//     if (!handle->bindings.console_output) return;
//     for (u32 i = 0; i < size; i++)
//         handle->bindings.console_output(handle, ((char*)handle->out_buffer.buffer)[cursor + i]);
//     if (handle->bindings.console_flush) handle->bindings.console_flush(handle);
// }
// static inline void current_shell_print(char *lit){
//     if (current_shell) shell_print_specify_newline(current_shell, 1, lit);
// }
// static inline void current_shell_put(char *lit){
//     if (current_shell) shell_print_specify_newline(current_shell, 0, lit);
// }
// typedef enum {
//     exit_implementation_error = -1,
//     exit_return_success = 0,
//     exit_generic_error = 1,
// } cmd_returns;
// typedef cmd_returns (*builtin_entry_point)(shell_handle* ctx, hash_map_t *arguments);
// typedef struct {
//     string_slice name;
//     struct {
//         bool use_option;
//         int position;
//         bool ignore_spaces;
//         string_slice option;
//         bool use_value;
//     } indicator;
//     bool optional;
// } cmd_arg;
// typedef struct {
//     builtin_entry_point entry_point;
//     string_slice name;
//     int argc;
//     cmd_arg arguments[];
// } cmd_def;
// typedef struct {
//     hash_map_t *builtins;
//     bool script_only;
// } sheldon_ctx;
// shell_handle* create_sheldon(shell_bindings bindings, void* owner, void (*register_builtins)(shell_handle *handle));
// typedef void (*dir_traverse)(const char *directory, const char* name);
// void traverse_directory(const char *directory, bool recursive, dir_traverse func);
// char* get_current_dir();
// void change_current_dir(const char *path);
// char* gethomedir();
// char* read_full_file(const char *path, size_t *out_size);
// ;
// static inline string_slice read_file_slice(const char *path){
//     size_t s = 0;
//     return (string_slice){.data = read_full_file(path, &s), .length = s};
// }
// bool write_full_file(const char *path, void* buf, size_t size);
// void read_lines(char *file, void *ctx, void (*handle_line)(void *ctx, string_slice line));
// static inline uint16_t find_find_extension_index(char *path){
//     uint16_t count = 0;
//     while (*path && *path != '.'){ path++; count++; }
//     return path ? count : 0;
// }
       
// static const char hid_keycode_to_char[256] = {
//     [0x04] = 'a', [0x05] = 'b', [0x06] = 'c', [0x07] = 'd',
//     [0x08] = 'e', [0x09] = 'f', [0x0A] = 'g', [0x0B] = 'h',
//     [0x0C] = 'i', [0x0D] = 'j', [0x0E] = 'k', [0x0F] = 'l',
//     [0x10] = 'm', [0x11] = 'n', [0x12] = 'o', [0x13] = 'p',
//     [0x14] = 'q', [0x15] = 'r', [0x16] = 's', [0x17] = 't',
//     [0x18] = 'u', [0x19] = 'v', [0x1A] = 'w', [0x1B] = 'x',
//     [0x1C] = 'y', [0x1D] = 'z',
//     [0x1E] = '1', [0x1F] = '2', [0x20] = '3', [0x21] = '4',
//     [0x22] = '5', [0x23] = '6', [0x24] = '7', [0x25] = '8',
//     [0x26] = '9', [0x27] = '0',
//     [0x28] = '\n', [0x2C] = ' ', [0x2D] = '-', [0x2E] = '=',
//     [0x2F] = '[', [0x30] = ']', [0x31] = '\\', [0x33] = ';',
//     [0x34] = '\'', [0x35] = '`', [0x36] = ',', [0x37] = '.',
//     [0x38] = '/', [0x58] = '\n',
// };
// static const char char_to_hid[256] = {
//     ['a'] = 0x04, ['b'] = 0x05, ['c'] = 0x06, ['d'] = 0x07,
//     ['e'] = 0x08, ['f'] = 0x09, ['g'] = 0x0A, ['h'] = 0x0B,
//     ['i'] = 0x0C, ['j'] = 0x0D, ['k'] = 0x0E, ['l'] = 0x0F,
//     ['m'] = 0x10, ['n'] = 0x11, ['o'] = 0x12, ['p'] = 0x13,
//     ['q'] = 0x14, ['r'] = 0x15, ['s'] = 0x16, ['t'] = 0x17,
//     ['u'] = 0x18, ['v'] = 0x19, ['w'] = 0x1A, ['x'] = 0x1B,
//     ['y'] = 0x1C, ['z'] = 0x1D, ['1'] = 0x1E, ['2'] = 0x1F,
//     ['3'] = 0x20, ['4'] = 0x21, ['5'] = 0x22, ['6'] = 0x23,
//     ['7'] = 0x24, ['8'] = 0x25, ['9'] = 0x26, ['0'] = 0x27,
//     ['\n'] = 0x28,[' '] = 0x2C, ['-'] = 0x2D, ['='] = 0x2E,
//     ['['] = 0x2F, [']'] = 0x30, ['\\'] = 0x31, [';'] = 0x33,
//     ['\''] = 0x34, ['`'] = 0x35, [','] = 0x36, ['.'] = 0x37,
//     ['/'] = 0x38, ['\r'] = 0x28,
// };
// static const char hid_keycode_to_shift_char[256] = {
//     [0x04] = 'A', [0x05] = 'B', [0x06] = 'C', [0x07] = 'D',
//     [0x08] = 'E', [0x09] = 'F', [0x0A] = 'G', [0x0B] = 'H',
//     [0x0C] = 'I', [0x0D] = 'J', [0x0E] = 'K', [0x0F] = 'L',
//     [0x10] = 'M', [0x11] = 'N', [0x12] = 'O', [0x13] = 'P',
//     [0x14] = 'Q', [0x15] = 'R', [0x16] = 'S', [0x17] = 'T',
//     [0x18] = 'U', [0x19] = 'V', [0x1A] = 'W', [0x1B] = 'X',
//     [0x1C] = 'Y', [0x1D] = 'Z',
//     [0x1E] = '!', [0x1F] = '@', [0x20] = '#', [0x21] = '$',
//     [0x22] = '%', [0x23] = '^', [0x24] = '&', [0x25] = '*',
//     [0x26] = '(', [0x27] = ')',
//     [0x28] = '\n',[0x2C] = ' ', [0x2D] = '_', [0x2E] = '+',
//     [0x2F] = '{', [0x30] = '}', [0x31] = '|', [0x33] = ':',
//     [0x34] = '"', [0x35] = '~', [0x36] = '<', [0x37] = '>',
//     [0x38] = '?', [0x58] = '\n',
// };
// char hid_to_char(unsigned char c, u8 modifier, u8 special);
// static inline bool printable(unsigned char c){
//     return c >= 'A' && c <='z';
// }
       
// static arr_stack_t *directories;
// static arr_stack_t *files;
// static int selected = 0;
// static bool viewing_file = 0;
// static char *viewed_path;
// static char *viewed_data;
// static size_t viewed_size;
// static draw_ctx filebrowser_ctx;
// static bool disable_multiverse;
// typedef struct {
//     string name;
//     string full_path;
//     fs_entry_type type;
//     data_signature signature;
// } file_data;
// void on_entry(const char* path, const char* name){
//     if (!strcmp(name, "..") || !strcmp(name, ".")) return;
//     string full_path = {};
//     u64 dir_count = chunk_array_count(directories);
//     if (dir_count){
//         string parent = (*(string*)chunk_array_get(directories,dir_count-1));
//         if (parent.data[parent.length-1] == '/')
//             full_path = string_format("%s%s", parent.data, name);
//         else
//             full_path = string_format("%s/%s", parent.data, name);
//     } else
//         full_path = string_from_const(name);
//     string s = string_from_const(name);
//     file_data fdata = {
//         .full_path = full_path,
//         .name = s,
//     };
//     fs_stat st = {};
//     if (statf(full_path.data, &st)){
//         fdata.signature = st.data_type;
//         fdata.type = st.type;
//     }
//     chunk_array_push(files, &fdata);
// }
// color color_for_type(fs_entry_type type){
//     switch (type) {
//         case entry_directory: return 0xFF0000DE;
//         case entry_invalid: return 0xFF777777;
//         default: return 0xFFFFFFFF;
//     }
// }
// void refresh(bool full){
//     u64 dir_count = chunk_array_count(directories);
//     if (full){
//         for (u64 i = 0; i < chunk_array_count(files); i++){
//             file_data *fdata = chunk_array_get(files,i);
//             string_free(fdata->full_path);
//             string_free(fdata->name);
//         }
//         chunk_array_reset(files);
//         if (dir_count > 0)
//             traverse_directory((*(string*)chunk_array_get(directories,dir_count-1)).data, 0, on_entry);
//     }
//     i64 file_count = chunk_array_count(files);
//     if (selected >= file_count)
//         selected = file_count > 0 ? file_count - 1 : 0;
//     fb_clear(&filebrowser_ctx, 0);
//     if (viewing_file){
//         fb_draw_string(&filebrowser_ctx, viewed_path, 4, 4, 2, 0xFFFFFFFF);
//         if (!viewed_data){
//             fb_draw_string(&filebrowser_ctx, "Failed to open file", 4, 40, 2, 0xFFFFFFFF);
//         } else {
//             size_t probe = viewed_size < 512 ? viewed_size : 512;
//             bool text = 1;
//             for (size_t i = 0; i < probe; i++){
//                 unsigned char c = (unsigned char)viewed_data[i];
//                 if (!c || (c < 0x20 && c != '\n' && c != '\r' && c != '\t')){
//                     text = 0;
//                     break;
//                 }
//             }
//             if (!text){
//                 fb_draw_string(&filebrowser_ctx, "Binary file preview is not supported", 4, 40, 2, 0xFFFFFFFF);
//             } else {
//                 int scale = 2;
//                 int char_w = (int)gpu_char_size(scale);
//                 int char_h = char_w + 2;
//                 int cols = filebrowser_ctx.width > 8 ? (int)(filebrowser_ctx.width - 8) / char_w : 1;
//                 int rows = filebrowser_ctx.height > 48 ? (int)(filebrowser_ctx.height - 48) / char_h : 1;
//                 int x = 4;
//                 int y = 40;
//                 int drawn = 0;
//                 size_t i = 0;
//                 if (cols < 1) cols = 1;
//                 if (rows < 1) rows = 1;
//                 while (i < viewed_size && drawn < rows){
//                     char line[256];
//                     int n = 0;
//                     while (i < viewed_size && viewed_data[i] != '\n' && viewed_data[i] != '\r' && n < cols && n < (int)sizeof(line) - 1)
//                         line[n++] = viewed_data[i++];
//                     line[n] = 0;
//                     fb_draw_string(&filebrowser_ctx, line, x, y, scale, 0xFFFFFFFF);
//                     drawn++;
//                     y += char_h;
//                     while (i < viewed_size && (viewed_data[i] == '\n' || viewed_data[i] == '\r')) i++;
//                 }
//             }
//         }
//         return;
//     }
//     if (dir_count > 0)
//         fb_draw_string(&filebrowser_ctx, (*(string*)chunk_array_get(directories,dir_count - 1)).data, 4, 4, 2, 0xFFFFFFFF);
//     for (int i = 0; i < file_count; i++){
//         int y = 40 + (i * 30);
//         if (selected == i)
//             fb_draw_string(&filebrowser_ctx, ">", 0, y, 3, 0xFFFFFFFF);
//         file_data data = (*(file_data*)chunk_array_get(files,i));
//         int offset = 30;
//         fb_draw_string(&filebrowser_ctx, data.name.data, offset, y, 3, color_for_type(data.type));
//         offset += fb_get_char_size(3) * (data.name.length + 1);
//         if (data.signature){
//             bool valid = 1;
//             char *sig = (char*)&data.signature;
//             for (int c = 0; c < 8; c++){
//                 valid &= !sig[c] || is_printable(sig[c]);
//             }
//             if (valid){
//                 fb_draw_slice(&filebrowser_ctx, (string_slice){sig,8}, offset, y, 3, 0xFF999999);
//             }
//         }
//     }
// }
// bool (*filebrowser_handle_path)(const char *name, const char *full_path);
// void enter_path(const char *name, const char *full_path){
//     if (!name || !*name) return;
//     if (filebrowser_handle_path && filebrowser_handle_path(name, full_path)) return;
//     if (!strend(name, ".red")|| !strend(name,".RED")){
//         exec(full_path, 0, 0, EXEC_MODE_DEFAULT);
//         halt(0);
//     }
//     if (strend(name, ".c") == 0){
//         const char* argv[1] = { full_path };
//         exec("/home/applications/braincode.red", 1, argv, EXEC_MODE_DEFAULT);
//         halt(0);
//     }
//     fs_stat st = {};
//     if (!statf(full_path, &st)){
//         print("Failed to get info for %s",full_path);
//         return;
//     }
//     if (st.type == entry_directory){
//         string s = string_from_literal(full_path);
//         chunk_array_push(directories,&s);
//         selected = 0;
//         viewing_file = 0;
//         refresh(1);
//         return;
//     }
//     if (viewed_data){
//         release(viewed_data);
//         viewed_data = 0;
//     }
//     viewed_path = (char*)full_path;
//     viewed_data = read_full_file(viewed_path, &viewed_size);
//     viewing_file = 1;
//     refresh(0);
// }
// void enter(char *name){
//     enter_path(name, name);
// }
// void navigate(char *name){
//     chunk_array_reset(directories);
//     string s = string_from_literal(name);
//     chunk_array_push(directories,&s);
//     refresh(1);
// }
// void pop_dir(){
//     if (viewing_file){
//         if (viewed_data){
//             release(viewed_data);
//             viewed_data = 0;
//         }
//         if (viewed_path)
//             viewed_path = 0;
//         viewed_size = 0;
//         viewing_file = 0;
//         refresh(1);
//         return;
//     }
//     u64 dir_count = chunk_array_count(directories);
//     if (dir_count <= disable_multiverse) return;
//     string last = (*(string*)chunk_array_get(directories,dir_count-1));
//     string_free(last);
//     chunk_array_remove(directories,1);
//     selected = 0;
//     refresh(1);
// }
// void filebrowser_input(kbd_event ev){
//     if (ev.type != KEY_PRESS) return;
//     if (ev.key == 0x2a){
//         pop_dir();
//         return;
//     }
//     if (viewing_file)
//         return;
//     if (!chunk_array_count(files))
//         return;
//     u64 file_count = chunk_array_count(files);
//     if (ev.key == 0x52){
//         selected = (selected - 1 + file_count) % file_count;
//         refresh(0);
//         return;
//     }
//     if (ev.key == 0x51){
//         selected = (selected + 1) % file_count;
//         refresh(0);
//         return;
//     }
//     if (ev.key == 0x28 || ev.key == 0x58){
//         file_data data = (*(file_data*)chunk_array_get(files,selected));
//         enter_path(data.name.data,data.full_path.data);
//     }
// }
// void init_filebrowser(char *path){
//     files = chunk_array_create(sizeof(file_data),32);
//     directories = chunk_array_create(sizeof(string),16);
//     enter(path);
// }
       
       
// typedef struct {
//     u32 count;
// } fs_dir_list;
// typedef struct {
//     u32 offset;
//     size_t limit;
//     fs_dir_list *list;
// } fs_dir_list_helper;
// fs_dir_list_helper create_dir_list_helper(void* buf,size_t limit);
// bool dir_list_fill(fs_dir_list_helper *helper, const char *name);
// bool stat_dir(fs_stat *out_stat);
// static inline size_t dir_buf_size(fs_dir_list_helper *helper){
//     return helper->offset + sizeof(fs_dir_list);
// }
// int memcmp(const void *s1, const void *s2, size_t count);
// void* memset(void* dest, int byte, size_t count);
// void* memset32(void* dest, uint32_t val, size_t count);
// void* memcpy(void *dest, const void *src, size_t count);
// void memreverse(void *ptr, size_t n);
// void* memmove(void *dest, const void *src, size_t count);
// void* memmem(const void* haystack, size_t haystack_len, const void* needle, size_t needle_len);
       
// typedef struct {
//     module_file *file;
//     hash_map_t *params;
//     string_slice forwarded;
//     string_slice path;
// } path_resolution;
// static bool match_param(string_slice slice, string_slice param);
// typedef enum {
//     path_resolution_none,
//     path_resolution_forward,
//     path_resolution_partial
// } path_resolution_policy;
// static void print_args(void *key, uint64_t keylen, void *val){
//     print("Argument %v>%s",(string_slice){key,keylen},val);
// }
// static path_resolution parse_path(arr_stack_t *entries, const char *path, path_resolution_policy policy, void (*multiple)(path_resolution file)){
//     if (strlen(path) && *path == '/') path++;
//     path_resolution res = {
//         .file = 0,
//         .params = hash_map_create(16),
//         .path = slice_from_literal(path)
//     };
//     if (res.path.length && res.path.data[res.path.length-1] == '/') res.path.length--;
//     size_t arr_size = chunk_array_count(entries);
//     for (size_t i = 0; i < arr_size; i++){
//         module_file *file = chunk_array_get(entries,i);
//         char *pattern = file->name.data;
//         string_splitter template_split = make_string_splitter(pattern, '/', 0);
//         string_splitter path_splitter = make_string_splitter(path, '/', 0);
//         bool found = 1;
//         while (string_splitter_advance(&template_split)){
//             if (!string_splitter_advance(&path_splitter)){
//                 if (!strlen(path) || (policy & path_resolution_partial)){
//                     if (template_split.pointer < template_split.length){
//                         found &= 0;
//                     } else if (template_split.current.length && *template_split.current.data == ':'){
//                         hash_map_put(res.params, template_split.current.data + 1, template_split.current.length - 1, string_from_literal("").data);
//                         found &= 1;
//                     }
//                 } else found &= 0;
//             } else {
//                 if (slices_equal(template_split.current, path_splitter.current, 1)){
//                     found &= 1;
//                 } else if (template_split.current.length && *template_split.current.data == ':' && match_param(path_splitter.current, template_split.current)){
//                     found &= 1;
//                     hash_map_put(res.params, template_split.current.data + 1, template_split.current.length - 1, string_from_slice(path_splitter.current).data);
//                 } else {
//                     found &= 0;
//                     break;
//                 }
//             }
//         }
//         if (found && ((policy & path_resolution_forward) || !string_splitter_advance(&path_splitter))){
//             res.file = file;
//             if (policy & path_resolution_forward){
//                 if (!file->alias_info.alias_path.length) continue;
//                 res.forwarded = string_splitter_remaining(&path_splitter);
//             }
//             if (multiple){
//                 multiple(res);
//             } else
//                 return res;
//         }
//         if (!multiple)
//             hash_map_destroy(res.params);
//         res.params = hash_map_create(16);
//         res.forwarded = (string_slice){};
//     }
//     hash_map_destroy(res.params);
//     return res;
// }
// static fs_dir_list_helper *router_fs_dir_helper;
// static void emit_route_contents(path_resolution resolution);
// static size_t list_route_directory_contents(arr_stack_t *entries, const char *subdirectory, fs_dir_list_helper *helper){
//     if (!helper) return 0;
//     router_fs_dir_helper = helper;
//     parse_path(entries, subdirectory, path_resolution_partial, emit_route_contents);
//     return dir_buf_size(helper);
// }
       
       
// typedef struct {
//     uint64_t s0;
//     uint64_t s1;
// } rng_t;
// static inline uint64_t rotl(uint64_t x, int k){
//     return (x << k)|(x >> (64 - k));
// }
// void rng_seed(rng_t* rng, uint64_t seed);
// float rng_next01f(rng_t* rng);
// uint8_t rng_next8(rng_t* rng);
// uint16_t rng_next16(rng_t* rng);
// uint32_t rng_next32(rng_t* rng);
// uint64_t rng_next64(rng_t* rng);
// uint8_t rng_between8(rng_t* rng, uint8_t min, uint8_t max);
// uint16_t rng_between16(rng_t* rng, uint16_t min, uint16_t max);
// uint32_t rng_between32(rng_t* rng, uint32_t min, uint32_t max);
// uint64_t rng_between64(rng_t* rng, uint64_t min, uint64_t max);
// void rng_fill8(rng_t* rng, uint8_t* dst, size_t count);
// void rng_fill16(rng_t* rng, uint16_t* dst, size_t count);
// void rng_fill32(rng_t* rng, uint32_t* dst, size_t count);
// void rng_fill64(rng_t* rng, uint64_t* dst, size_t count);
// void rng_fill_buf(rng_t* rng, void* dst, size_t count);
// static inline i64 powi(i64 n, i64 e){
//     if (e == 1) return n;
//     if (e == 2) return n << e;
//     i64 acc = n;
//     for (i64 i = 1; i < e; i++)
//         acc *= n;
//     return acc;
// }
// static __attribute__((always_inline)) inline float sqrtf(float v){
//     return __builtin_sqrtf(v);
// }
// static inline double pow2(double v){
//     return v*v;
// }
// static inline double pow3(double v){
//     return v*v*v;
// }
// static inline int min(int a, int b){
//     return a < b ? a : b;
// }
// static inline float minf(float a, float b){
//     return a < b ? a : b;
// }
// static inline int max(int a, int b){
//     return a > b ? a : b;
// }
// static inline float maxf(float a, float b){
//     return a > b ? a : b;
// }
// static inline int abs(int n){
//     return n < 0 ? -n : n;
// }
// static inline int clamp(int v, int min, int max){
//     int t = v < min ? min : v;
//     return t > max ? max : t;
// }
// static inline float absf(float n){
//     return n < 0 ? -n : n;
// }
// static inline double absd(double n){
//     return n < 0 ? -n : n;
// }
// static inline float clampf(float v, float min, float max){
//     float t = v < min ? min : v;
//     return t > max ? max : t;
// }
// static inline float saturate(float v){
//     return clampf(v, 0, 1.f);
// }
// static inline int sign(int x) {
//     return x < 0 ? -1 : 1;
// }
// static inline int signf(float x) {
//     return x < 0 ? -1 : 1;
// }
// static inline int lerp(int i, int start, int end, int steps) {
//     return start + (end - start) * i / steps;
// }
// static inline bool float_zero(float a){
//     const float epsilon = 1e-6f;
//     return absf(a) < epsilon;
// }
// static inline float lerpf(float a, float b, float t) {
//   if((a<=0 && b>=0) || (a>=0 && b<=0)) return t*b + (1-t)*a;
//   if(t==1) return b;
//   const float x = a + t*(b-a);
//   return (t>1) == (b>a) ? maxf(b,x) : minf(b,x);
// }
// static inline i64 ceil_to_int(double val){
//     i64 whole = (i64)val;
//     double frac = val - (double)whole;
//     return frac > 0 ? whole + 1 : whole;
// }
// static inline double ceil(double val){
//     return (double)ceil_to_int(val);
// }
// static inline i64 round_to_int(double val){
//     i64 whole = (i64)val;
//     double frac = val - (double)whole;
//     if (frac <= -0.5) return whole - 1;
//     return frac >= 0.5 ? whole + 1 : whole;
// }
// static inline i64 floor_to_int(double val){
//     return (i64)val;
// }
// static inline double floor(double val){
//     return (double)floor_to_int(val);
// }
// static inline int64_t abs_i64(int64_t v){
//     return v < 0 ? -v : v;
// }
// static inline int64_t clamp_i64(int64_t v, int64_t lo, int64_t hi){
//     if (v < lo) return lo;
//     if (v > hi) return hi;
//     return v;
// }
// static uint64_t sqrt_u64(uint64_t x){
//     uint64_t op = x;
//     uint64_t res = 0;
//     uint64_t one = 1ULL << 62;
//     while (one > op) one >>= 2;
//     while (one != 0) {
//         if (op >= res + one) {
//             op -= res + one;
//             res = (res >> 1) + one;
//         } else {
//             res >>= 1;
//         }
//         one >>= 2;
//     }
//     return res;
// }
// static arr_stack_t *entries = 0;
// static u64 (*hashing_func)(const char *path);
// static inline u64 hash_filename(const char *name){
//     return hashing_func ? hashing_func(name): hash_map_fnv1a64(name, strlen(name));
// }
// static inline module_file* make_entry(const char *name, fs_backing_type back_type, fs_entry_type ent_type, data_signature data_type, buffer buf){
//     if (!entries) entries = chunk_array_create(sizeof(module_file),32);
//     chunk_array_push(entries,&(module_file){
//         .name = string_from_literal(name),
//         .backing_type = back_type,
//         .entry_type = ent_type,
//         .file_buffer = buf,
//         .references = 0,
//         .data_type = data_type,
//         .fid = hash_filename(name),
//         .serial = hash_filename(name),
//     });
//     return chunk_array_get(entries, chunk_array_count(entries)-1);
// }
// static inline module_file* make_complex_entry(const char *name, fs_backing_type back_type, fs_entry_type ent_type, data_signature data_type, file_actions actions, string alias){
//     if (!entries) entries = chunk_array_create(sizeof(module_file),32);
//     chunk_array_push(entries,&(module_file){
//         .name = string_from_literal(name),
//         .alias_info = {
//             .alias_path = alias,
//         },
//         .file_buffer = {},
//         .backing_type = back_type,
//         .entry_type = ent_type,
//         .actions = actions,
//         .references = 0,
//         .data_type = data_type,
//         .fid = hash_filename(name),
//     });
//     return chunk_array_get(entries, chunk_array_count(entries)-1);
// }
// static inline module_file* eval_entry(string_slice path){
//     for (u32 i = 0; i < chunk_array_count(entries); i++){
//         module_file *file = chunk_array_get(entries,i);
//         if (slice_lit_match(path, file->name.data, 1)) return file;
//     }
//     return 0;
// }
// static inline module_file* find_entry(u64 fid){
//     for (u32 i = 0; i < chunk_array_count(entries); i++){
//         module_file *file = chunk_array_get(entries,i);
//         if (file->fid == fid) return file;
//     }
//     return 0;
// }
// static inline size_t list_entries(u64 *offset, const char *prefix, fs_dir_list_helper *helper){
//     size_t pref_len = strlen(prefix);
//     if (pref_len && *prefix == '/'){
//         prefix++;
//         pref_len--;
//     }
//     for (u64 i = offset ? *offset : 0; i < chunk_array_count(entries); i++){
//         char *entry_name = (*(module_file*)chunk_array_get(entries,i)).name.data;
//         if ((size_t)strstart(prefix, entry_name) == pref_len){
//             entry_name += pref_len;
//             if (strlen(entry_name) && *entry_name == '/') entry_name++;
//             if (!strlen(entry_name)) continue;
//             if (str_has_char(entry_name, strlen(entry_name), '/') >= 0) continue;
//             if (!dir_list_fill(helper, entry_name)){
//                 if (offset) *offset = i;
//                 return dir_buf_size(helper);
//             }
//         }
//     }
//     return dir_buf_size(helper);
// }
// static inline string_slice first_path_component(const char *path){
//     if (!strlen(path)) return (string_slice){};
//     const char *next = seek_to(path, '/');
//     string_slice slice = (string_slice){ .data = (char*)path, .length = next - path - (*(next-1) == '/' ? 1 : 0)};
//     if (!slice.length) return (string_slice){};
//     return slice;
// }
// static inline FS_RESULT vfs_open_aliased(module_file *mfile, path_resolution resolution, file *fd){
//     string fullpath = mfile->alias_info.alias_path;
//     string additional = (string){};
//     if (mfile->entry_type == entry_directory){
//         string_slice next = resolution.forwarded;
//         additional = string_format("%S/%s",fullpath,next);
//         fullpath = additional;
//     }
//     FS_RESULT result = openf(fullpath.data, &mfile->alias_info.alias_fd);
//     string_free(additional);
//     if (result != FS_RESULT_SUCCESS){
//         return result;
//     }
//     memcpy(fd, &mfile->alias_info.alias_fd, sizeof(file));
//     fd->id = mfile->fid;
//     return FS_RESULT_SUCCESS;
// }
// static inline FS_RESULT vfs_open(const char *path, file *fd){
//     if (path && *path == '/') path++;
//     if (!path || !strlen(path)) path = "#";
//     path_resolution resolution = parse_path(entries, path, path_resolution_forward, 0);
//     module_file *mfile = resolution.file;
//     if (!mfile) return FS_RESULT_NOTFOUND;
//     if (mfile->actions.open) return mfile->actions.open(path,fd);
//     mfile->references++;
//     if (mfile->alias_info.alias_path.length)
//         return vfs_open_aliased(mfile, resolution, fd);
//     fd->id = mfile->fid;
//     return FS_RESULT_SUCCESS;
// }
// static string_slice vfs_skip_path(string_slice path, string name){
//     if (name.length < path.length) return slice_from_string(name);
//     size_t m = path.length;
//     string_slice ret = {};
//     for (size_t i = 0; i < m; i++){
//         if (path.data[i] != name.data[i]){
//             ret = (string_slice){ .data = name.data + i, .length = name.length-i};
//             break;
//         }
//     }
//     ret = (string_slice){ .data = name.data + path.length, .length = name.length-path.length};
//     if (ret.length && ret.data[0] == '/') { ret.data++; ret.length--; }
//     return ret;
// }
// static file_offset *out_offset = 0;
// static file_offset current_offset = 0;
// static void emit_route_contents(path_resolution resolution){
//     if (!out_offset || (*out_offset) <= current_offset){
//         if (slice_lit_match(resolution.path,resolution.file->name.data, 1) && resolution.file->alias_info.alias_path.length){
//             if (!resolution.forwarded.length) {
//                 dir_list(resolution.file->name.data, router_fs_dir_helper->list, router_fs_dir_helper->limit, out_offset);
//                 return;
//             }
//             string fullpath = string_format("%S/%v",resolution.file->alias_info.alias_path,resolution.forwarded);
//             dir_list(fullpath.data, router_fs_dir_helper->list, router_fs_dir_helper->limit, out_offset);
//             return;
//         }
//         if (resolution.file->actions.readdir){
//             resolution.file->actions.readdir(resolution.forwarded.data, router_fs_dir_helper->list, router_fs_dir_helper->limit, out_offset);
//             return;
//         }
//         string_slice lpc = vfs_skip_path(resolution.path,resolution.file->name);
//         if (lpc.length)
//             if (!dir_list_fill(router_fs_dir_helper, lpc.data)){
//                 if (out_offset) *out_offset = current_offset;
//             }
//     }
//     current_offset++;
// }
// static bool match_param(string_slice slice, string_slice param){
//     return 0;
// }
// static inline size_t vfs_read(file *fd, char* buf, size_t size, file_offset offset){
//     module_file *mfile = find_entry(fd->id);
//     if (!mfile) return 0;
//     if (mfile->actions.read) return mfile->actions.read(fd, buf, size, offset);
//     if (mfile->alias_info.alias_fd.id){
//         size_t res = readf(&mfile->alias_info.alias_fd, buf, size);
//         memcpy(fd, &mfile->alias_info.alias_fd, sizeof(file));
//         fd->id = mfile->fid;
//         return res;
//     }
//     return buffer_read(&mfile->file_buffer, buf, size, offset);
// }
// static inline size_t vfs_transform(const char *path, void* buf, size_t size){
//     if (path && *path == '/') path++;
//     if (!path || !strlen(path)) path = "#";
//     path_resolution resolution = parse_path(entries, path, path_resolution_forward, 0);
//     module_file *mfile = resolution.file;
//     if (!mfile) return FS_RESULT_NOTFOUND;
//     if (mfile->actions.transform) return mfile->actions.transform(path, buf, size);
//     if (mfile->alias_info.alias_fd.id){
//         return transformf(mfile->alias_info.alias_path.data, buf, size);
//     }
//     return 0;
// }
// static inline size_t vfs_write(file *fd, const char* buf, size_t size, file_offset offset){
//     module_file *mfile = find_entry(fd->id);
//     if (!mfile) return 0;
//     if (mfile->actions.write) return mfile->actions.write(fd, (void*)buf, size, offset);
//     if (mfile->alias_info.alias_fd.id){
//         size_t res = writef(&mfile->alias_info.alias_fd, buf, size);
//         memcpy(fd, &mfile->alias_info.alias_fd, sizeof(file));
//         fd->id = mfile->fid;
//         return res;
//     }
//     return buffer_write_to(&mfile->file_buffer, buf, size, fd->cursor);
// }
// static inline bool vfs_stat(const char *path, fs_stat *out_stat){
//     if (!out_stat) return 0;
//     if (path && *path == '/') path++;
//     if (!path || !strlen(path)) path = "#";
//     path_resolution resolution = parse_path(entries, path, path_resolution_forward, 0);
//     module_file *file = resolution.file;
//     if (!file){
//         if (resolution.forwarded.length == 0)
//             return stat_dir(out_stat);
//         return 0;
//     }
//     if (file->actions.getstat) file->actions.getstat(path, out_stat);
//     if (file->alias_info.alias_path.length){
//         string fullpath = file->alias_info.alias_path;
//         string additional = (string){};
//         if (file->entry_type == entry_directory && resolution.forwarded.length){
//             additional = string_format("%S/%s",fullpath,resolution.forwarded);
//             fullpath = additional;
//         }
//         size_t res = statf(fullpath.data, out_stat);
//         string_free(additional);
//         return res;
//     }
//     out_stat->size = file->file_buffer.buffer_size;
//     out_stat->type = file->entry_type;
//     out_stat->data_type = file->data_type;
//     return 1;
// }
// static inline void vfs_close(file *fd){
//     module_file *mfile = find_entry(fd->id);
//     if (!mfile) return;
//     if (mfile->actions.close){
//         mfile->actions.close(fd);
//         return;
//     }
//     if (mfile->alias_info.alias_fd.id){
//         return closef(&mfile->alias_info.alias_fd);
//     }
// }
// static inline size_t vfs_readdir(const char *path, void *buf, size_t size, file_offset *offset){
//     fs_dir_list_helper helper = create_dir_list_helper(buf, size);
//     out_offset = offset;
//     return list_route_directory_contents(entries, path, &helper);
// }
// static inline size_t vfs_list(const char *path, void *buf, size_t size, file_offset *offset){
//     return vfs_readdir(path, buf, size, offset);
// }
// static inline FS_RESULT vfs_trace_open(const char *path, file *fd){
//      FS_RESULT res = vfs_open(path, fd);
//      print("vfs_trace_open result %llx",res);
//      return res;
// }
// static inline size_t vfs_trace_read(file *fd, char *buf, size_t size, file_offset offset){
//      size_t res = vfs_read(fd, buf, size, offset);
//      print("vfs_trace_read result %llx",res);
//      return res;
// }
// static inline size_t vfs_trace_transform(const char *path, void *buf, size_t size){
//      size_t res = vfs_transform(path, buf, size);
//      print("vfs_trace_transform result %llx",res);
//      return res;
// }
// static inline size_t vfs_trace_write(file *fd, const char *buf, size_t size, file_offset offset){
//      size_t res = vfs_write(fd, buf, size, offset);
//      print("vfs_trace_write result %llx",res);
//      return res;
// }
// static inline bool vfs_trace_stat(const char *path, fs_stat *outstat){
//      bool res = vfs_stat(path, outstat);
//      print("vfs_trace_stat result %llx",res);
//      return res;
// }
// static inline size_t vfs_trace_readdir(const char *path, void* buf, size_t size, file_offset *offset){
//      size_t res = vfs_readdir(path, buf, size, offset);
//      print("vfs_trace_readdir result %llx",res);
//      return res;
// }
// system_module menu_mod = {
//     .name = "demo menu",
//     .mount = "menu",
//     .version = (uint64_t)((((uint64_t)0) << 48) | (((uint64_t)1) << 32) | (((uint64_t)0) << 16) | ((uint64_t)0)),
//     .owner = 0,
//     .init = 0,
//     .fini = 0,
//     .open = vfs_open,
//     .read = vfs_read,
//     .write = vfs_write,
//     .close = vfs_close,
//     .truncate = 0,
//     .getstat = vfs_stat,
//     .readdir = vfs_readdir,
//     .transform = vfs_transform,
//     .alias_info = {}
// };
// static inline string make_menu_entry(const char *parent, const char *name, fs_entry_type ent_type, file_transform_fn trigger){
//     if (parent && *parent == '/') parent++;
//     if (name && *name == '/') name++;
//     if (!entries) entries = chunk_array_create(sizeof(module_file),32);
//     module_file *file = (module_file*)chunk_array_get(entries,chunk_array_push(entries,&(module_file){}));
//     *file = (module_file){
//         .name = {},
//         .alias_info = {},
//         .backing_type = backing_virtual,
//         .entry_type = ent_type,
//         .fid = hash_filename(name),
//         .serial = hash_filename(name),
//         .private_data = 0,
//         .data_type = 0,
//         .file_buffer = (buffer){
//             .buffer = 0,
//             .buffer_size = 0,
//             .limit = 0,
//             .options = buffer_read_only,
//             .cursor = 0,
//             .data_type = 0,
//         },
//         .references = 0,
//         .actions = {
//             .open = 0,
//             .read = 0,
//             .write = 0,
//             .close = 0,
//             .truncate = 0,
//             .getstat = 0,
//             .readdir = 0,
//             .transform = trigger,
//         },
//     };
//     if (parent) file->name = string_format("%s/%s",parent, name);
//     else file->name = string_from_literal(name);
//     return file->name;
// }
extern void setup_menu();
void menu_init(){
    setup_menu();
    load_fsmodule(&menu_mod, 0);
    swritef("/environment/menu", 0, 0, 0);
}
size_t test_fn(const char *path, void *c, size_t s){
    print("Test button pressed");
    return 0;
}
// void setup_menu(){ char *parent_dir = 0; { { char *local_dir = make_menu_entry(parent_dir, "File", entry_directory, 0).data; char *old_dir = parent_dir; parent_dir = local_dir; { { char *local_dir = make_menu_entry(parent_dir, "Hello", entry_directory, 0).data; char *old_dir = parent_dir; parent_dir = local_dir; { make_menu_entry(parent_dir, "World", entry_file, 0); }; parent_dir = old_dir; } make_menu_entry(parent_dir, "Test", entry_file, test_fn); }; parent_dir = old_dir; } { char *local_dir = make_menu_entry(parent_dir, "Edit", entry_directory, 0).data; char *old_dir = parent_dir; parent_dir = local_dir; { make_menu_entry(parent_dir, "Delete", entry_file, 0); }; parent_dir = old_dir; } }; };
int main(){
    menu_init();
    request_draw_ctx(&filebrowser_ctx);
    files = chunk_array_create(sizeof(file_data),32);
    directories = chunk_array_create(sizeof(string),16);
    enter("/");
    while (1){
        commit_draw_ctx(&filebrowser_ctx);
        kbd_event ev = {};
        if (!read_event(&ev)) {
            msleep(25);
            continue;
        }
        if (ev.type != KEY_PRESS) continue;
        if (ev.type == 0x29) { return 0; }
        filebrowser_input(ev);
    }
    return 0;
}
