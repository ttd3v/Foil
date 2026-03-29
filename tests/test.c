#include "../debug/print/print.h"
#include "../debug/ping/ping.h"
#include "../data-structures/array/array.h"
#include "../data-structures/btree/btree.h"
#include "../atomic/queue/aqueue.h"
#include "../syscall/mem/mem.h"
#include "../syscall/random/random.h"
#include "../hashes/blake2d/blake2d.h"
#include "../algorithms/stringify/stringify.h"
#define TVALUE 4096
i32 main(){
/////////////////////////////////////////////////////////////////////////////////////////
{
        array list = {0};
        i32 o = array_new(&list, 2);
        if (o != 0){
                char out[] = "failed to initialize array\n";
                print(out, sizeof(out));
                return -1;
        }
        for(u64 i = 0; i < 1024; i++){
                array_push(&list, (ref)&i, 8, 1);
        }

        u64 val = 0;
        for(i32 i = 0; i < 1024; i++){
                array_pop(&list, (ref)&val, 8);
                if(val != 1023-i){
                        char out[] = "x!=y; array_pop\n";
                        print(out, sizeof(out));
                        return -1;
                }
        }
        
        val = 123;
        array_push(&list, (ref)&val, 8, 1);
        val = 0;
        array_read(&list, (ref)&val, 8, 0);
        if(val != 123){
                char out[] = "x!=y; array_read";
                print(out,sizeof(out));
                return -1;
        }

        array_remove(&list, (ref)&val, 8, 0);
        
        if(list.length != 0){
                char out[] = "array_delete\n";
                print(out,sizeof(out));
                return -1;
        }
        
        array_destroy(&list);
        s_print("[+] ARRAY\n");
}
/////////////////////////////////////////////////////////////////////////////////////////
{
        u64* keys = map(TVALUE*8);
        randall(keys, TVALUE*8);
        btreeU64 *tree = (btreeU64*)-1;
        i32 o = btreeU64_new(&tree);
        if (o != 0){
                char out[] = "failed to initialize btree\n";
                print(out, sizeof(out));
                return -1;
        }
        //ping(0);
        for (u64 key = 0; key < TVALUE; key++) {
                btreeU64_push(tree, keys[key], key);
        }
        //ping(1);
         for (u64 key = 0; key < 252; key++) {
                u64 val = -1;
                i32 a= btreeU64_get(tree, keys[key], &val);
                if(val != key){
                        char out[] = "\nx!=y; btree_get\n";
                        print(out,sizeof(out));
                        //ping(key);
                        char o[] = "\n";
                        print(o,sizeof(o));
                        //ping(val);
                        
                        char b[] = "\n";
                        print(b,sizeof(b));
                        //ping (a);
                        return -1;
                }
        }
        //ping(2);
        for (u64 key = 0; key < TVALUE; key++) {
                u64 val = -1;
                i32 a= btreeU64_get(tree, keys[key], &val);
                if(val != key){
                        char out[] = "\nx!=y; btree_get\n";
                        print(out,sizeof(out));
                        //ping(key);
                        char o[] = "\n";
                        print(o,sizeof(o));
                        //ping(val);
                        
                        char b[] = "\n";
                        print(b,sizeof(b));
                        //ping (a);
                        return -1;
                }
        }
        for (u64 key = 0; key < TVALUE; key++) {
                btreeU64_remove(tree, keys[key]);
        }
        u64 v = -1;
        i32 e = btreeU64_get(tree, keys[0], &v);
        if(v != -1){
                char out[] = "x!=y; btree_remove\n";
                print(out,sizeof(out));
                return -1;
        }
        s_print("[+] BTREE\n");
}
/////////////////////////////////////////////////////////////////////////////////////////
{
    aqueue q;
    aqueue_make(&q, 16);
    u64 n = q.capacity;
    u64 value;
    for (u64 i = 0; i < n; i++) {
        value = i;
        i32 e = aqueue_push(&q, &value);
        if (e != 0) {
            char out[] = "aqueue_push full\n";
            print(out, sizeof(out));
            //ping(i);
            aqueue_destroy(&q);
            return -1;
        }
    }

    for (u64 i = 0; i < n; i++) {
        value = (u64)-1;
        i32 e = aqueue_pop(&q, &value);
        if (e != 0 || value != i) {
            char out[] = "x!=y; aqueue_pop\n";
            print(out, sizeof(out));
            //ping(i);
            //ping(value);
            aqueue_destroy(&q);
            return -1;
        }
    }

    aqueue_destroy(&q);
        s_print("[+] AQUEUE\n");
}
/////////////////////////////////////////////////////////////////////////////////////////
{
        u8 out[64];
        u8 expected[64] = {
                0xBA, 0x80, 0xA5, 0x3F, 0x98, 0x1C, 0x4D, 0x0D,
                0x6A, 0x27, 0x97, 0xB6, 0x9F, 0x12, 0xF6, 0xE9,
                0x4C, 0x21, 0x2F, 0x14, 0x68, 0x5A, 0xC4, 0xB7,
                0x4B, 0x12, 0xBB, 0x6F, 0xDB, 0xFF, 0xA2, 0xD1,
                0x7D, 0x87, 0xC5, 0x39, 0x2A, 0xAB, 0x79, 0x2D,
                0xC2, 0x52, 0xD5, 0xDE, 0x45, 0x33, 0xCC, 0x95,
                0x18, 0xD3, 0x8A, 0xA8, 0xDB, 0xF1, 0x92, 0x5A,
                0xB9, 0x23, 0x86, 0xED, 0xD4, 0x00, 0x99, 0x23
        };
        u8 in[3] = "abc";
        blake2d(out, sizeof(out), in, 3);
        for (u64 i = 0; i < 64; i++){
                if(out[i] != expected[i]){
                        char num1[32];
                        char num2[32];
                        char num3[32];
                        i32 n1l = stringify_u64(out[i],num1);
                        i32 n2l = stringify_u64(expected[i], num2);
                        i32 n3l = stringify_u64(i, num3);
                        s_print("[-] BLAKE2D\n\n");
                        char m0[] = "-> Expected=";
                        char m1[] = " Out=";
                        char m2[] = " At=";
                        print(m0,sizeof(m0));
                        print(num2,n2l);
                        print(m1,sizeof(m1));
                        print(num1,n1l);
                        print(m2,sizeof(m2));
                        print(num3,n3l);
                        s_print("\n");
                        return -1;
                }
        } 
        s_print("[+] BLAKE2D\n");
}
        return 0;
}
