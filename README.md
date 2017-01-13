# Beaver OS
It's an OS!
The only official way to refer to this project is 'Beaver OS'.

# Building
To build Beaver OS, you should install two cross-compilers: `i686-elf` and `x86_64-elf`.

## Installing cross-compilers
If you are a Gentoo user, you can just install `sys-devel/crossdev`
and it will do the work for you:
```
crossdev --stage1 -t i686-elf
crossdev --stage1 -t x86_64-elf
```

Otherwise, you should do it manually:
* Download [binutils] (ftp://ftp.gnu.org/gnu/binutils/) and [gcc] (ftp://ftp.gnu.org/gnu/gcc/)
* Build binutils for `i686`:
```
mkdir build-i686
cd build-i686
../confgure \
    --prefix=/opt/cross \
    --target=i686 \
    --disable-nls \
    --disable-werror \
make
make install
```
* Build binutils for `x86_64`:
```
mkdir build-x86_64
cd build-x86_64
../confgure \
    --prefix=/opt/cross \
    --target=x86_64
    --disable-nls \
    --disable-werror \
make
make install
```
* Download gcc's prerequesties:
```
./contrib/download_prerequisites
```
* Build gcc for `i686`:
```
mkdir build-i686
cd build-i686
../confgure \
    --prefix=/opt/cross \
    --target=i686 \
    --disable-nls \
    --enable-languages=c,c++ \
    --without-headers
make
make install
```
* Build gcc for `x86_64`:
```
mkdir build-x86_64
cd build-x86_64
../confgure \
    --prefix=/opt/cross \
    --target=i686 \
    --disable-nls \
    --enable-languages=c,c++ \
    --without-headers
make
make install
```
* Set PATH:
```
export PATH=/opt/cross/bin:$PATH
```
## Building
Just run `make`. It will produce `beaveros.iso`.

# Using
To use Beaver OS, boot `beaveros.iso` as a Live CD/USB image.
If you have QEMU, you can just run `make run`.

# Debugging
If you have QEMU, you can just run `make run-debug`.
