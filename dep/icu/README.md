## VERSION

- icu4c 57.1

## LICENSE

- ICU License

## STUBDATA SETUP

0. Download ICU DATA Library:
 - http://apps.icu-project.org/datacustom/
1. Download path:
 - <code>./data/in/icudt57l.dat</code>
3. Run configuration:
 - <code>./configure && make -j8</code>
4. Check <code>icupkg.inc</code> file:
 - <code>./data/icupkg.inc</code>
 - IMPORTANT:
  - Don't use the assembly argument in <code>icupkg.inc</code> file.
  - Remove <code>GENCCODE_ASSEMBLY_TYPE</code> value in <code>icupkg.inc</code> file.
 - [IF] MacOSX settings:
  - <code>export DYLD_LIBRARY_PATH=$PWD/lib:$DYLD_LIBRARY_PATH</code>
5. Extract items from the package:
 - <code>./bin/icupkg -d ./data/out/build/icudt57l --list -x \* ./data/in/icudt57l.dat -o ./data/out/tmp/icudata.lst</code>
6. Produce packaged ICU data from the given list(s) of files:
```bash
mkdir -p out/tmp
## if you want to change the temp directory: -T ./data/out/tmp -> -T ./output_temp
./bin/pkgdata -O ./data/icupkg.inc -q -c -s ./data/out/build/icudt57l -d ./lib -e icudt57 -T ./data/out/tmp \
              -p icudt57l -m static -r 57.1 -L icudata ./data/out/tmp/icudata.lst
```
7. Check directory:
 - <code>./data/out/tmp</code>
8. Copy <code>*.c</code> files:
 - <code>./data/out/tmp/*.c</code>

