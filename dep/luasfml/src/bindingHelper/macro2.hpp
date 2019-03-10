#undef X0
#undef X1
#undef X2
#undef X3

#ifndef NAME_TO_BIND
#error you must define the macro NAME_TO_BIND which should containt the name exposed to lua
#endif

#ifndef TYPE_TO_BIND
#error you must define the macro TYPE_TO_BIND which should containt the type exposed to lua
#endif


#define BUILD_NAME(type,name) type##_##name

#define HAS_RETURN(value1,value2) return value1(l, value2);
#define NO_RETURN(value1,value2)  value2;return 0;

#define RET_CTOR       RET_TYPE(TYPE_TO_BIND)
#define RET_TYPE(type) lua_push<type>
#define RET_NONE

#define NO_POINTER
#define IS_POINTER *

#define GET_PARAM(type, index,isPointer)  isPointer  lua_get<type isPointer >(l,index)

#define FUNCTION_0(functionToBind)                             functionToBind()
#define   METHOD_0(functionToBind) GET_PARAM(TYPE_TO_BIND*,1,NO_POINTER)->functionToBind()
#define   GETTER_0(functionToBind) GET_PARAM(TYPE_TO_BIND*,1,NO_POINTER)->functionToBind

#define CALL_FUNCTION_0(type,functionToBind)    type##_0(functionToBind)

#define FUNCTION_1(functionToBind,firstParam,firstIsPointer)                                        functionToBind( GET_PARAM(firstParam,1,firstIsPointer))
#define   METHOD_1(functionToBind,firstParam,firstIsPointer) GET_PARAM(TYPE_TO_BIND*,1,NO_POINTER)->functionToBind(  GET_PARAM(firstParam,2,firstIsPointer))
#define   SETTER_1(functionToBind,firstParam,firstIsPointer) GET_PARAM(TYPE_TO_BIND*,1,NO_POINTER)->functionToBind = GET_PARAM(firstParam,2,firstIsPointer)

#define CALL_FUNCTION_1(type,functionToBind,firstParam,firstIsPointer)    type##_1(functionToBind,firstParam,firstIsPointer)


#define FUNCTION_2(functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer)                                        functionToBind( GET_PARAM(firstParam,1,firstIsPointer),GET_PARAM(secondParam,2,secondeIsPointer))
#define   METHOD_2(functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer) GET_PARAM(TYPE_TO_BIND*,1,NO_POINTER)->functionToBind(  GET_PARAM(firstParam,2,firstIsPointer),GET_PARAM(secondParam,3,secondeIsPointer))

#define CALL_FUNCTION_2(type,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer)    type##_2(functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer)

#define FUNCTION_3(functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer)                                        functionToBind( GET_PARAM(firstParam,1,firstIsPointer),GET_PARAM(secondParam,2,secondeIsPointer),GET_PARAM( thirdParam,3, thirdIsPointer))
#define   METHOD_3(functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer) GET_PARAM(TYPE_TO_BIND*,1,NO_POINTER)->functionToBind(  GET_PARAM(firstParam,2,firstIsPointer),GET_PARAM(secondParam,3,secondeIsPointer),GET_PARAM( thirdParam,4, thirdIsPointer))

#define CALL_FUNCTION_3(type,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer)    type##_3(functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer)


#define DEFAULT_CTOR_1(type,firstParam,firstIsPointer) \
template<> type* newDefault(lua_State * l) { return new  FUNCTION_1(type,firstParam,firstIsPointer) ; }

#define DEFAULT_CTOR_2(type,firstParam,firstIsPointer,secondParam,secondeIsPointer) \
template<> type* newDefault(lua_State * l) { return new  FUNCTION_2(type,firstParam,firstIsPointer,secondParam,secondeIsPointer) ; }

#define DEFAULT_CTOR_3(type,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer) \
template<> type* newDefault(lua_State * l) { return new  FUNCTION_3(type,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer) ; }

#ifdef __GO_FOR_IMPLEMENTATION__
#define X3(name, type,hasReturn,returnType,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer) \
    static int BUILD_NAME(NAME_TO_BIND,name) (lua_State * l) { hasReturn(returnType,  CALL_FUNCTION_3(type,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer) ) }
#define X2(name, type,hasReturn,returnType,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer) \
    static int BUILD_NAME(NAME_TO_BIND,name) (lua_State * l) { hasReturn(returnType,  CALL_FUNCTION_2(type,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer) ) }
#define X1(name, type,hasReturn,returnType,functionToBind,firstParam,firstIsPointer) \
    static int BUILD_NAME(NAME_TO_BIND,name) (lua_State * l) { hasReturn(returnType,  CALL_FUNCTION_1(type,functionToBind,firstParam,firstIsPointer) ) }
#define X0(name, type,hasReturn,returnType,functionToBind) \
    static int BUILD_NAME(NAME_TO_BIND,name) (lua_State * l) { hasReturn(returnType, CALL_FUNCTION_0(type,functionToBind) ) }

#else
#define X3(name, type,hasReturn,returnType,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer, thirdParam, thirdIsPointer)\
    { #name        , BUILD_NAME(NAME_TO_BIND,name) },
#define X2(name, type,hasReturn,returnType,functionToBind,firstParam,firstIsPointer,secondParam,secondeIsPointer)\
    { #name        , BUILD_NAME(NAME_TO_BIND,name) },
#define X1(name, type,hasReturn,returnType,functionToBind,firstParam, firstIsPointer)  \
    { #name        , BUILD_NAME(NAME_TO_BIND,name) },
#define X0(name, type,hasReturn,returnType,functionToBind)                             \
{ #name        , BUILD_NAME(NAME_TO_BIND,name) },

#endif
