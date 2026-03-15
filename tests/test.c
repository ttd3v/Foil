#include "../debug/print/print.h"
#include "../debug///ping///ping.h"
#include "../data-structures/array/array.h"
#include "../data-structures/btree/btree.h"
#include "../atomic/queue/aqueue.h"
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

        {
                char out[] = "[+] ARRAY\n";
                print(out,sizeof(out));
        }
}
/////////////////////////////////////////////////////////////////////////////////////////
{
        btreeU64 *tree = (struct btreeU64*)-1;
        i32 o = btreeU64_new(&tree);
        if (o != 0){
                char out[] = "failed to initialize btree\n";
                print(out, sizeof(out));
                return -1;
        }
        //ping(0);
        for (u64 key = 0; key < TVALUE; key++) {
                btreeU64_push(tree, key, key);
        }
        //ping(1);
         for (u64 key = 0; key < 252; key++) {
                u64 val = -1;
                i32 a= btreeU64_get(tree, key, &val);
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
                i32 a= btreeU64_get(tree, key, &val);
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
        //ping(3);
        btreeU64_push(tree, 1, 2);
        //ping(4);
        btreeU64_remove(tree, 1);
        //ping(5);
        u64 v = 0;
        btreeU64_get(tree, 1, &v);
        if(v != 2){
                char out[] = "x!=y; btree_remove\n";
                print(out,sizeof(out));
                return -1;
        }
    char out[] = "[+] BTREE\n";
    print(out, sizeof(out));
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
    char out[] = "[+] AQUEUE\n";
    print(out, sizeof(out));
}
/////////////////////////////////////////////////////////////////////////////////////////
        return 0;
}
