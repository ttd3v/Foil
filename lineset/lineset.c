#include "lineset.h"
#include "../mem/mem.h"
#include "../constants/errno.h"
#include "../parsers/parsers.h"

int lineset_parse(str string, u64 length, LineSet* output){
        u64 ptr[512] = {0};
        u64 cursor = 0;
        

        u64 crlf = 0;
        u64 line_start = 0;
        u64 eq_pos = 0;
        for (u64 i = 0; i < length && cursor < 509; i++){
                char byte = string[i];
                if(byte == '='){
                        ptr[cursor++] = line_start; 
                        ptr[cursor++] = i;          
                        eq_pos = i + 1;             
                        continue;
                }
                if((byte == '\r' || byte == '\n')){
                        if(!crlf){
                                ptr[cursor++] = eq_pos;
                                ptr[cursor++] = i;
                                crlf = 1;
                        }
                        line_start = i + 1;
                }
                if(byte != '\r' && byte != '\n'){
                        crlf = 0;
                }
        }
        if(cursor % 4 == 2){
                ptr[cursor++] = eq_pos;
                ptr[cursor++] = length;
        }
        
        u64 item_count = cursor/4;
        u64 total_value_size = 0;
        u64 total_string_size = 0;
        str key_repository;
        u64 keys_repo_size = 0;
        enum LineSetValueType type_range[item_count];
        for (u64 i = 0; i < item_count; i++){
                u64 cell0 = ptr[i*4];
                u64 cell1 = ptr[1+(i*4)];
                u64 value0 = ptr[2+(i*4)];
                u64 value1 = ptr[3+(i*4)];
                keys_repo_size += cell1-cell0;
                if(cell1-cell0 > 4096){
                        return -1;
                }
                u64 value_length = value1-value0;
                enum LineSetValueType type_of_value = String;
                i64 sequence[3] = {0,0,0}; 
                for (u64 j = 0; j < value_length; j++){
                        char c = string[value0 + j];
                        sequence[0] += (!(c >= '0' && c <= '9')) && c != '.'; // not a number
                        sequence[1] += (c >= '0' && c <= '9'); // number
                        sequence[2] += c=='.';
                }
                if (sequence[0] > 0){
                        type_of_value = String;
                }
                if(sequence[1] > 0 && sequence[0] == 0 && sequence[2] == 0){
                        type_of_value = Number;
                }
                if(sequence[0] == 0 && sequence[1] > 0 && sequence[2] == 1){
                        type_of_value = Float;
                }
                if(sequence[0] >= 4 && sequence[0] < 6 && sequence[1] == 0 && sequence[2] == 0){
                        if(
                                (string[value0] == 't' || string[value0] == 'T') &&
                                (string[value0+1] == 'r' || string[value0+1] == 'R') &&
                                (string[value0+2] == 'u' || string[value0+2] == 'U') &&
                                (string[value0+3] == 'e' || string[value0+3] == 'E')
                        ){
                                type_of_value = Bool;
                        }else if(
                                (string[value0] == 'f' || string[value0] == 'F') &&
                                (string[value0+1] == 'a' || string[value0+1] == 'A') &&
                                (string[value0+2] == 'l' || string[value0+2] == 'L') &&
                                (string[value0+3] == 's' || string[value0+3] == 'S') &&
                                (string[value0+4] == 'e' || string[value0+4] == 'E')
                        ){
                                type_of_value = Bool;
                        }else{
                                type_of_value = String;
                        }
                }
                total_string_size += cell1-cell0;
                switch (type_of_value){
                        case String:
                                total_value_size += value1-value0;
                                type_range[i] = type_of_value;
                                break;
                        case Bool:
                                total_value_size += 1;
                                type_range[i] = type_of_value;
                                break;
                        case Number:
                                total_value_size += 8;
                                type_range[i] = type_of_value;
                                break;
                        case Float:
                                total_value_size += 8;
                                type_range[i] = type_of_value;
                                break;
                        default:
                                break;
                }
        }
     

        key_repository = map(keys_repo_size);
        u64 line_set_keys_size = item_count*sizeof(u64)*2; 
        LineSetKey* keys = map(line_set_keys_size);
        LineSetValue* values = map(sizeof(LineSetValue) * item_count);
        u8* buffer = map(total_value_size);
        output->length = item_count;
        output->name = keys;
        output->name_buffer_size = line_set_keys_size;
        output->value = values;
        output->value_buffer_size = (item_count) * 16;
        output->data_buffer_size = total_value_size;
        output->data_buffer = buffer;
        output->key = key_repository;
        output->key_buffer_size = keys_repo_size;

        if((i64)key_repository == ENOMEM || (i64)keys == ENOMEM || (i64)values == ENOMEM || (i64)buffer == ENOMEM ){
                lineset_destroy(*output);
                return -4;
        }

        for(u64 i = 0; i < item_count; i++){
                u64 cell0  = ptr[i*4];
                u64 cell1  = ptr[i*4 + 1];
                u64 c_delta = cell1-cell0;
                keys[i].length = c_delta;
                keys[i].string = string+cell0;
        }

        u64 buffer_cursor = 0;
        u64 kr_cursor = 0;
        for (u64 i = 0; i < item_count; i++){
                //u64 ic = (i*item_count);
                u64 cell0  = ptr[i*4];
                u64 cell1  = ptr[i*4 + 1];
                u64 value0 = ptr[i*4 + 2];
                u64 value1 = ptr[i*4 + 3];

                u64 c_delta = cell1-cell0;
                u64 v_delta = value1-value0;
                cpy(key_repository+kr_cursor,string+cell0,c_delta); 
                
                output->name[i].string = key_repository + kr_cursor;
                output->name[i].length = c_delta;
                kr_cursor += c_delta;
                output->value[i].value_type = type_range[i];
                output->value[i].value = buffer+buffer_cursor;
                switch (type_range[i]){
                        case String:
                                cpy(buffer+buffer_cursor,string+value0,v_delta);
                                output->value[i].size = v_delta;
                                buffer_cursor += v_delta;
                                break;
                        case Number:{
                                u64 range = value1-value0;
                                i32 result = parse_number(string+value0, range, (i64*)(buffer+buffer_cursor));
                                if(result < 0){return -2;};
                                buffer_cursor += 8;
                        }break;
                        case Float:{
                                u64 range = value1-value0;
                                f64 me = 0;
                                i32 result = parse_float(string+value0, range, &me);
                                cpy(buffer+buffer_cursor,&me,8);
                                if(result < 0){return -2;};
                                buffer_cursor += 8;
                        }break;
                        case Bool:
                                buffer[buffer_cursor] = (string[value0] == 'T' || string[value0] =='t')?0xFF:0x00;
                                buffer_cursor++;
                                break;
                        default:
                                break;
                }
        }

        unmap(string,length);
        return 0;
}

void lineset_destroy(LineSet self){
        if((i64)self.data_buffer != ENOMEM){ unmap(self.data_buffer,self.data_buffer_size); };
        if((i64)self.name != ENOMEM){ unmap(self.name,self.name_buffer_size); };
        if((i64)self.value != ENOMEM){ unmap(self.value,self.value_buffer_size); };
        if((i64)self.key != ENOMEM){ unmap(self.key,self.key_buffer_size); };
}

void lineset_get(LineSet *self, str key, LineSetValue *result){
        for(u64 i = 0; i < self->length; i++){
                u64 o = 0;
                u64 i_len = self->name[i].length;
                while (o != i_len){
                        if ((*(u8*)(self->name[i].string+o)) != *(u8*)(key+o)) break;
                        o++;
                }
                if(o == i_len){
                        *result = self->value[i];
                        return;
                }
                
        }
}
