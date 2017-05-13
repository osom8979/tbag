#!/usr/bin/env bash

find $PWD/capnp -name "*.h" | xargs sed -i ".tmp" -e 's;\(^#include \)"\(.*\)"$;\1<libtbag/3rd/capnp/\2>;g'
find $PWD/capnp -name "*.h" | xargs sed -i ".tmp" -e 's;\(^#include \<\)capnp/;\1libtbag/3rd/capnp/;g'
find $PWD/capnp -name "*.h" | xargs sed -i ".tmp" -e 's;\(^#include \<\)kj/;\1libtbag/3rd/kj/;g'
find $PWD/capnp -name "*.tmp" | xargs rm

find $PWD/kj -name "*.h" | xargs sed -i ".tmp" -e 's;\(^#include \)"\.\./\(.*\)"$;\1<libtbag/3rd/kj/\2>;g'
find $PWD/kj -name "*.h" | xargs sed -i ".tmp" -e 's;\(^#include \)"\(.*\)"$;\1<libtbag/3rd/kj/\2>;g'
find $PWD/kj -name "*.tmp" | xargs rm

