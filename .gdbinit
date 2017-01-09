# add this file to ~/.gdbinit:
# add-auto-load-safe-path <this file path>
target remote localhost:1234
symbol-file loader.bin
