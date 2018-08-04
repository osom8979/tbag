## VERSION

- SQLite 3.24.0

## Update

Fixed compile error in MSVC:
```diff
diff --git a/libtbag/3rd/sqlite3/sqlite3.c b/libtbag/3rd/sqlite3/sqlite3.c
index ce4f343f..425dacf1 100644
--- a/libtbag/3rd/sqlite3/sqlite3.c
+++ b/libtbag/3rd/sqlite3/sqlite3.c
@@ -1060,11 +1060,12 @@ SQLITE_PRIVATE const char **sqlite3CompileOptions(int *pnOpt){
 #ifndef SQLITE3_H
 #define SQLITE3_H
 #include <stdarg.h>     /* Needed for the definition of va_list */
+#include <libtbag/3rd/sqlite3/sqlite3_export.h>
 
 /*
 ** Make sure we can call this stuff from C++.
 */
-#if 0
+#ifdef __cplusplus
 extern "C" {
 #endif
 
@@ -10169,7 +10170,7 @@ SQLITE_API int sqlite3_deserialize(
 # undef double
 #endif
 
-#if 0
+#ifdef __cplusplus
 }  /* End of the 'extern "C"' block */
 #endif
 #endif /* SQLITE3_H */
```

## LICENSE

- Public Domain

