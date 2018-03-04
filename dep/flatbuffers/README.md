## VERSION

- FlatBuffers 1.7.1

## LICENSE

- Apache License Version 2.0

## CHANGE LOG

### Change include header

<code>src/idl_gen_cpp.cpp</code> Replace from:
```c++
code += "#include \"flatbuffers/flatbuffers.h\"\n\n
```
To:
```c++
code += "#include <libtbag/3rd/flatbuffers/flatbuffers.h>\n\n";
```

