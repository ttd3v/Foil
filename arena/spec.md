# Foil Arena

Author: ttd3v
Date: 2026-4-4
Version: 0x0000

## Abstract

This document describes the Foil Arena, an Arena that endeavor fast memory handling. Making the implementation of such in assembly, straightforward— Even while not being limited to assembly, or any architecture.

## Introduction and terminology

This document covers the implementation of the Foil Arena, a simple allocator, made to both have a simple interface, and enable parallelism. Meanwhile keeping performance as a priority.


This document is highly biased by x86-64, if an instruction is mentioned, then the document is referring to the behavior of such. Thus, you SHALL implement the behavior of the instruction, if using another architecture.

The document follows **RFC 2119** terminology, having the thereby defined meaning when the words are in upper-case. 

For defining the size and at times type of an instance, a typo enclosed parenthesis will be used, refering the the word before it. Typos being: "dyn", which specifies that the size is dynamic, and an "u" followed by bit count that specifies the instance is an unsigned integer. 

## Execution description

To describe an execution, m0-64 (u64) will be used as a way of describe a read-write storage for data. Or h0-64 (dyn) for heap data, s0-64 (dyn) for stack data. Whichever of the mentioned, when between brackets (`[` or `]`) are for describing a pointer de-reference, which the size shall be described as mentioned in the terminology or an explicit way.

## Arena pool

The Arena is a stream bytes with a length of 72, it holding the array pointers, lengths, locks, and capacities.

The bytes between the offset 0 and 24 are three u64, being pointers to in memory arrays. These arrays holding pointers to the allocated pages. Their lengths being a sequence of three u32 in between the offsets 24 and 40, the maximum length being the according capacity.

Locks, in between the offsets 40 and 52, are three u32 which are locks. These locks being used to ensure correctness in contexts with parallelism.

As for the bytes between 64 and 72, they are a u64 which keeps the Arena flags.

For a reference purpose, the names of the arrays in the first 24 bytes of the structure are "normal", "medium", and "small".

Here is an ASCII visual representation of the structure:
```
     0          8          16
[  normal  |  medium  |  small  ]

    24     28      32        40       44       48    
[  len  |  len  |  len  ][  lock  |  lock  |  lock  ]

    52      56      60       64
[  cap  |  cap  |  cap  ][  flags  ]
```

The array "normal" keeps pointers to segments of 4096 bytes (full page), the array medium keeps pointers to segments of 2048 bytes, and small keeps
pointers to segments of 1024 bytes.

### Array

Arrays are a pre-allocated range of data, having a length and capacity. In it, values can only be pushed, or popped.

When a value is pushed, the length SHALL be compared to the capacity, if smaller, then the push can proceed. Thus, the new value (u64) being stored at the index equal to the length in the array.

When a pop is requested, the length SHALL be compared, if higher than zero, the operation can proceed. The pop must read the value in the array, which is at the index equal to the length, set it to zero, and output the readen.

On both methods, in case of failure, an error SHALL be issued. By either outputting it, or no-op.

### Individual page lifetime

Upon allocation on the Create, the page MUST have all of its bytes 0x00. So the memory allocated from the arena can have the guarantee of all bytes being zero, therefore to ensure it, before returning a page into the arena, the bytes of the page HAVE to be set 0x00.

### Locks

The locking mechanism is what guarantee correctness in environments with parallelism. A lock can have two states, `0x00000000` for deactivated, `0xFFFFFFFF` for activated. When a lock is deactivated, it means no other core is interacting with whichever the lock is protecting, thus it can be activated.

For a lock to be acquired, the core must execute a `LOCK cmpxchg` (in x86-64), exchanging the lock's value to activated's, only if the value was deactivated's. If the exchange completes successfully, the core that made it owns the lock, and therefore SHALL eventually deactivate it.

If the exchange fails, the core doing so must either yield a value mentioning the fail, or spin-lock — This fail being refered as "blocked-lock" in this text.

## Create

For the creation of a new structure, the function SHALL have the first argument being the count (dyn) of pages to start with, second argument SHALL be the flags (u64).

The flags are (can be OR'ed): 0x00 for static capacity (only the entered on the first argument), 0x01 for dynamic capacity (allow the array to either grow or shrink dynamically), 0x02 for single thread (assumes the environment cannot have any shape or form of parallelism, thus locks are never used), 0x04 to not allocate "medium" and "small". 0x08 for enabling spin-locks in case of a blocked-lock. 

Once created, one memory allocation SHALL be made, it being of `72 + (N*24)` bytes, where N is the first argument. 
After the allocation, which if failed SHALL have the error outputed, the first capacity numbers SHALL be N. The first array pointer SHALL be the returned from the allocation plus 72, the second SHALL be the pointer plus `72+(N*8)`, the third SHALL be the pointer plus `72+(N*16)`.

If the flag "0x04" is set, then the first length SHALL be N, and the rest 0. Otherwise, the length of all SHALL be N.

The contents of the arrays must be populated accordingly to their lengths, thus the arrays MUST have pointers within that reference a region of bytes which size is the array's (specific size mentioned before).

The pages which pointers will be in the arrays MAY be allocated individually, as a way of leveraging ASLR at the cost of more context switching.

## Allocate

The function `allocate` is for retrieve a page from the arena, the first parameter being a pointer to the arena, second parameter a number (u8, u16, u32, or u64). The function SHALL return `-1` when it fails to pull a page, `-2` when the arena is out of pages, `-3` if the locks are are activated, and any value higher than zero being the pointer derived from the arena.

The second parameter SHALL be either zero, one, or two. This being used to tell which array, and therefore sort of page the caller wants.

### Execution

Assuming the first parameter is in m0, and the second in m1, m2 being `[m0+64]` (u8) which are the flags. If the environment is defined, in the flags as single-threaded, all the lock-related instances will never happen.

For handling the locks, the lock in `[m0+40+(m1*4)]` (u32) is activated. In case of failure, a spin-lock SHOULD happen if allowed, `-3` returned otherwise. Then, `[m0+24+(m1*4)]` (u32) into m3.

If m3 equals zero, and the dynamic capacity isn't allowed, `-2` is returned. Otherwise, X pages (The number of X is implementation dependent) MUST be allocated (`-1` returned if any of the allocations fail, the ones that succeeded pushed into the selected array), the capacity increased by X, and X-1 pointers pushed into the selected array, while one of the pointers returned after the lock being deactivated as a result.

If m3 is not zero, a pointer SHALL be pop'ed from the selected array, and returned after the lock is deactivated.

## Deallocate

> If the single thread feature is in the flags, all the steps regarding the lock MAY be ignored or skipped (depends on implementation).

The function `deallocate` is for yielding a page back to the arena, the first parameter being a pointer (u64) to the arena, second parameter a number (u8, u16, u32, or u64), and third parameter a u64. The function SHALL return `-1` for when it fails to yield a page back, or `0` for success.

The first parameter is the arena pointer, second is the pointer being free'd, third is the array number (either u8, u16, u32, or u64) which SHALL be either 0, 1, or 2.

### Execution

First parameter is in m0, second in m1, and third in m2, and m3 being `[m0+64]` (the flags).

The execution attempts to activate the lock. In case of failure and the feature being present, a spin-lock happens; `-2` being returned if the feature isn't present. 

The length is fetched into m3 from `[m0 + 24 + (m2*4)]` (u32), and the capacity into m4 from `[m0 + 52 + (m2*4)]`. If the length is equal or higher than the capacity, `-1` SHALL be returned after the lock get deactivated. Otherwise, the pointer SHALL be pushed into the array, lock deactivated, and `0` returned.

## Grow

> If the single thread feature is in the flags, all the steps regarding the lock MAY be ignored or skipped (depends on implementation).

The function `grow` is for increasing the capacity of an array in the arena, having three parameters. First being the arena pointer (u64), second the count of elements to be inserted into the arena (u64), and third the array number (either u8, u16, u32, or u64).

Return values being zero for success, `-1` for a failure during memory allocation, `-2` the lock couldn't be activated, and `0` for success.

### Execution

m0 keeps the arena pointer, m1 the count of elements to be inserted into the array (hereby mentioned as N), m2 the array number.

The execution attempts to activate the lock of the selected array — In `[m0+40+(m2*4)]`. In case of failure and the feature being present, a spin-lock happens (if the feature is present); `-2` being returned if the feature isn't present. 

the capacity at `[m0+52+(m3*4)]` is fetched into m3 increased by N, length from `[m0+24+(m3*4)]` is fetched into m4, a memory buffer of `(N+m3)*8`
being allocated it being h0 — lock SHALL be deactivated, and `-1` SHALL be returned if this allocation fails. Then, all the items in the array is copied into the h0, the old array unmapped, and the new array pointer set into the arena. Then, N segments of `4096>>m2` bytes are allocated and pushed into the array; lock is deactivated and `0` returned.

If any memory operation of the mentioned fails, `-1` SHALL be returned.

## Shrink

> If the single thread feature is in the flags, all the steps regarding the lock MAY be ignored or skipped (depends on implementation).

The function `shrink` removes all the pages available in all arrays, decreasing their capacity by the count of elements removed of each.

The first parameter is the arena buffer.

### Execution

m0 is the first parameter. The following execution covers the shrink process of each array. Thus it MAY be an iteration to
shrink all.

The execution attempt to activate the array lock, in case of failure and the spin-lock feature isn't enabled `-1` is returned. If the spin lock feature is enabled and the failure occurs, the spin-lock happens.

The length of the array is fetched into m1 and capacity into m2, if qual to zero, the iteration/execution continues, otherwise lock is deactivated and `0` (success) is returned. 

m1 elements are popped, and unmap applied on all. Then, a new array SHALL be allocated, it being `(m2-m1)*8`; m2 also sub-ed by m1. Then, the old array SHALL have unmap applied, and the m2 written into the capacity of the current array, 0 written into the length of the current array, and the new array pointer SHALL be written into the current array pointer.

If the allocation fails, the lock SHALL be deactivated and `-2` returned.

After all shrinks, the lock SHALL be deactivated, and `0` (success) returned.
