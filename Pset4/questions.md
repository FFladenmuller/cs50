# Questions

## What's `stdint.h`?


A header file in the C standard library introduced in the C99 standard library section 7.18
to allow programmers to write more portable code by providing a set of typedefs that specify 
exact-width integer types, together with the defined minimum and maximum allowable values for 
each type, using macros.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?


These are several reasons to use these types:
* You want a safe and easy way to port code.
* You want to control the exact layout of a struct as to have no padding. 
* It can help you to avoid using excessively large types. 
> For example, while `long long` is at least 64 bits, it might be 128-bit on some machines, 
and using it when what you need is just a type that can store 64 bit numbers would be very 
wasteful on such machines. `int_least64_t` solves the problem. 
> - R..

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?


Type | Byte count
---- | ----------
`BYTE` | 1
`DWORD` | 4
`LONG` | 4
`WORD` | 2

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."


BM

## What's the difference between `bfSize` and `biSize`?


`bfSize` is the size, in bytes, of the bitmap file, while `biSize` is the number of bytes required by the structure.

## What does it mean if `biHeight` is negative?


The image is a top down DIB (device-independent bitmap), which cannot be compressed.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?


`biBitCount`

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?


If the file does not exist.

## Why is the third argument to `fread` always `1` in our code?


We want to always read the exact amount of information in whatever we are writing.
I.e. when we read the `BITMAPFILEHEADER` we want to only read whatever the size of 1
`BITMAPFILEHEADER`.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?


0

## What does `fseek` do?


`fseek` allows you to "fast forward" or "rewind" within a file to a certain memory location.

## What is `SEEK_CUR`?


The method fseek: 
```
int fseek(FILE* fp, long int offset, int from_where);
```
can take `SEEK_CUR` as an argument for its 3rd parameter.
> If you want to change the location of the pointer fp from its current location, set `from_where` to `SEEK_CUR`.
> - CS50 Reference
