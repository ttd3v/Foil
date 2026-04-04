# Foil Arena

## Abstract

This document describes the Foil Arena, an Arena that endeavor fast memory handling. Making the implementation of such in assembly, straightforward.

## Introduction and terminology

The document follows **RFC 2119** terminology, having the thereby defined meaning when the words are in upper-case. 

For defining the size and at times type of an instance, a typo enclosed parenthesis will be used, refering the the word before it. Typos being: "dyn", which specifies that the size is dynamic, and an "u" followed by bit count that specifies the instance is an unsigned integer. 


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

### Create

For the creation of a new structure, the function SHALL have the first argument being the count (dyn) of pages to start with, second argument SHALL be the flags (u64).

The flags are: 0x00 for static capacity (only the entered on the first argument), 0x01 for dynamic capacity (allow the array to either grow or shrink dynamically), 0x02 for single thread (assumes the environment cannot have any shape or form of parallelism, thus locks are never used), 0x04 to not allocate "medium" and "small".


