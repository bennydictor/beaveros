# Beaver OS
It's an OS!

# Building
To build Beaver OS, you should install two cross-compilers: `i686-elf` and `x86_64-elf`.

## Installing cross-compilers
If you are a Gentoo user, first of all, respect to you. You can just install `sys-devel/crossdev`
and it will do the work for you:
```
crossdev --stage1 -t i686-elf
crossdev --stage1 -t x86_64-elf
```

Otherwise, you should do it manually. Google it youself.

## Building Beaver OS
Just run `make`. It will produce `beaveros.iso`.

# Using
To use Beaver OS, boot `beaveros.iso` as a Live CD/USB image.
