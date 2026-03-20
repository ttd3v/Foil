#include "array.h"
#include "../../syscall/mem/mem.h"

i32 array_new(array *self, u64 pages){
        u64 p = pages|1;
        u64 b = p*4096;
        ref m = map(b);
        self->pages = p;
        self->length = 0;
        self->buffer = m;
        return (i64)m < 0?-1:0;
}

void array_destroy(array *self){
        unmap(self->buffer, self->pages*4096);
}
i32 array_push(array *self, ref *element, u64 element_size, u64 N){
        u64 k = self->length+N;
        u64 w = self->pages*4096;
        u64 z = self->length*element_size;
        u64 l = N*element_size;
        if(k*element_size >= w){
                u64 page_count = (l + 4096 - 1) / 4096;
                ref pg = map(w);
                if(pg == MAP_FAIL){return -1;};
                cpy(pg, self->buffer, z);
                cpy(pg+z, element, l);
                unmap(self->buffer, w);
                self->buffer = pg;
                self->pages = page_count;
                self->length=k;
                return 0;
        }
        cpy(self->buffer+z, element, l);
        self->length=k;
        return 0;
}

i32 array_remove(array *self, ref *element, u64 element_size, u64 position){
        u64 index = position*element_size;
        cpy(element, self->buffer+index, element_size);
        cpy(self->buffer+index, self->buffer+((self->length-1)*element_size),element_size);
        self->length--;
        return 0;
}

i32 array_pop(array *self, ref *element, u64 element_size){
        return array_remove(self, element, element_size, self->length-1);
}


i32 array_read(array *self, ref *element, u64 element_size, u64 position){
        u64 index = (position%self->length)*element_size;
        cpy(element, self->buffer+index, element_size);
        return 0;
}

i32 array_expand(array *self, u64 count){
        u64 x = self->pages*4096;
        u64 y = self->pages+count;
        u64 z = y*4096;
        ref pg = map(z);
        if(pg == MAP_FAIL){return -1;}
        cpy(pg, self->buffer, x);
        unmap(self->buffer, x);
        self->buffer = pg;
        self->pages = y;
        return 0;
}

i32 array_shrink(array *self, u64 count){
        if(count >= self->pages){return -1;}
        u64 old_size = self->pages*4096;
        u64 new_pages = self->pages-count;
        u64 new_size = new_pages*4096;
        ref pg = map(new_size);
        if(pg == MAP_FAIL){return -1;}
        cpy(pg, self->buffer, new_size);
        unmap(self->buffer, old_size);
        self->buffer = pg;
        self->pages = new_pages;
        return 0;
}
