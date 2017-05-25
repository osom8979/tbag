@0xf5eb5a72eb4961ef;

## project.cmake:
##  tbag_modules__build_ext_capnp_cpp (tbag-tpot-capnp
##      "${CMAKE_SOURCE_DIR}/libtbag/tpot/struct/Protocol.capnp")

struct Date {
    year  @0 :Int16; # The year. Must include the century.
    month @1 :UInt8; # Month number, 1-12.
    day   @2 :UInt8; # Day number, 1-30.
}

struct Time {
    hours   @0 : UInt8; # Hours number, 0-23.
    minutes @1 : UInt8; # Minutes number, 0-23.
    seconds @2 : UInt8; # Minutes number, 0-23.
}

struct TimeDetail {
    millisec @0 : UInt16; # Milliseconds number, 0-999
    microsec @1 : UInt16; # Microseconds number, 0-999
    nanosec  @2 : UInt16; # Nanoseconds number, 0-999
}

## ------------
## Packet body.
## ------------

struct Header {
    request @0 : UInt64 = 0;
}

struct Packet {
    header @0 : Header;
    body : union {
        unemployed @1 : Void;
        version    @2 : Version;
        command    @3 : Command;
        script     @4 : Script;
        message    @5 : Message;
    }
}

## ------------
## Packet body.
## ------------

struct Version {
    major @0 : UInt16;
    minor @1 : UInt16;
    patch @2 : UInt16;
}

struct Command {
    exec @0 : Text;
    cwd  @1 : Text;
    args @2 : List(Text);
    envs @3 : List(Env);
}

struct Env {
    key   @0 : Text;
    value @1 : Text;
}

struct Script {
    mime @0 : Text;
    body @1 : Text;
}

struct Message {
    msg @0 : Text;
}

