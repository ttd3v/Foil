#ifndef LINESET_H
#define LINESET_H
#include "../../typos.h"
enum LineSetValueType:u64 {
        String = 0,
        Bool = 1,
        Number = 2,
        Float = 3
};
// true
// false

typedef struct{
        enum LineSetValueType value_type;
        mem value;
        u64 size;
} LineSetValue;

typedef struct {
        u64 length;
        str string;
} LineSetKey;

typedef struct {
        LineSetKey* name;
        LineSetValue* value;
        u8* data_buffer;
        u64 data_buffer_size;
        u64 name_buffer_size;
        u64 value_buffer_size;
        u64 length;
        str key;
        u64 key_buffer_size;
} LineSet;

///     LineSetParse  
/// ====================
/// Parse a line-set
///
/// Unmaps:
///     output
/// Maps:
///     Only the internal values of output
/// Returns:
/// 0  ->  Success
/// -1 ->  Too big lines
/// -2 ->  Malformed lines
/// -3 -> Failed to map memory
i32 lineset_parse(str string, u64 length, LineSet* output);

///     lineset_destroy
/// ========================
/// Unmap all mapped memory
void lineset_destroy(LineSet self);

///     lineset_get
/// ===================
/// Yield a reference to a value (if any) derived from a key.
void lineset_get(LineSet *self, str key, LineSetValue* result);
#endif
