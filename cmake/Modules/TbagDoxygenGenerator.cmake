#/// @file   TbagDoxygenGenerator.cmake
#/// @brief  tbag_doxygen_generate function prototype.
#/// @author zer0
#/// @date   2016-05-27

#/// Use Doxygen to create the HTML based API documentation.
#///
#/// @param __doxyfile [in] Doxyfile path.
#/// @param __working  [in] Working directory.
function (tbag_doxygen_generate __doxyfile __working)
    if (NOT DOXYGEN_FOUND)
        find_package (Doxygen)
    endif ()

    if (DOXYGEN_FOUND)
        add_custom_target (doxygen ALL "${DOXYGEN_EXECUTABLE}" "${__doxyfile}"
                           WORKING_DIRECTORY "${__working}")
    endif ()
endfunction ()

#/// Write doxygen index html file.
#///
#/// @param __output_path [in] Output html file path.
function (tbag_doxygen_generate__index_html __output_path)
    if (EXISTS "${__output_path}")
        file (REMOVE "${__output_path}")
    endif ()

    file (WRITE "${__output_path}" "<meta http-equiv=\"refresh\" content=\"0; url=doxygen/index.html\" />")
endfunction()

#/// Write default Doxyfile.
#///
#/// @param __output_path [in] Output Doxyfile file path.
function (tbag_doxygen_generate__default_doxyfile __output_path)
    if (EXISTS "${__output_path}")
        file (REMOVE "${__output_path}")
    endif ()

    file (WRITE "${__output_path}"
            "DOXYFILE_ENCODING       = UTF-8                        \n"
            "PROJECT_NAME            = \"${MAIN_NAME}\"             \n"
            "PROJECT_NUMBER          = \"${VERSION}\"               \n"
            "PROJECT_BRIEF           = \"${MAIN_BRIEF}\"            \n"
            "OUTPUT_DIRECTORY        = \"${PROJECT_BINARY_DIR}\"    \n"
            "OUTPUT_LANGUAGE         = Korean-en                    \n"
            "ALIASES                 = translate{2}=\"<dl class='section remarks'><dt>Translate-\\1</dt><dd>\\2</dd></dl>\" \n"
            "MARKDOWN_SUPPORT        = NO                           \n"
            "WARN_IF_UNDOCUMENTED    = NO                           \n"
            "INPUT                   = \"${PROJECT_SOURCE_DIR}\"    \n"
            "FILE_PATTERNS           = *.c *.cc *.cxx *.cpp *.c++ *.d *.java *.ii *.ixx *.ipp *.i++ *.inl *.h *.hh *.hxx *.hpp *.h++ *.idl *.odl *.inc \n"
            "RECURSIVE               = YES                          \n"
            "EXCLUDE_PATTERNS        = *build/*                     \n"
            "HTML_TIMESTAMP          = YES                          \n"
            "HTML_OUTPUT             = doxygen                      \n"
            "GENERATE_LATEX          = NO                           \n"
            "LATEX_OUTPUT            = latex                        \n"
            "MACRO_EXPANSION         = YES                          \n"
            "INCLUDE_PATH            = \"${PROJECT_SOURCE_DIR}\"    \n"
            "INCLUDE_FILE_PATTERNS   = *.h *.hh *.hxx *.hpp *.h++   \n"
            "USE_MATHJAX             = YES                          \n"
            "MATHJAX_RELPATH         = https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML \n"
            "QUIET                   = YES                          \n")
endfunction()

macro (tbag_doxygen_generate__auto_default)
    tbag_doxygen_generate__default_doxyfile ("${PROJECT_BINARY_DIR}/Doxyfile")
    tbag_doxygen_generate__index_html ("${PROJECT_BINARY_DIR}/doxygen.html")
    tbag_doxygen_generate ("${PROJECT_BINARY_DIR}/Doxyfile" "${PROJECT_BINARY_DIR}")
endmacro()

## -----------------
## Doxyfile content.
## -----------------

#DOXYFILE_ENCODING       = UTF-8
#PROJECT_NAME            =
#PROJECT_NUMBER          =
#PROJECT_BRIEF           =
#PROJECT_LOGO            =
#OUTPUT_DIRECTORY        =
#CREATE_SUBDIRS          = NO
#ALLOW_UNICODE_NAMES     = NO # Doxygen v1.8.6 unsupported tag.
#OUTPUT_LANGUAGE         = English
#BRIEF_MEMBER_DESC       = YES

#REPEAT_BRIEF             = YES
#ABBREVIATE_BRIEF         =
#ALWAYS_DETAILED_SEC      = NO
#INLINE_INHERITED_MEMB    = NO
#FULL_PATH_NAMES          = YES
#STRIP_FROM_PATH          =
#STRIP_FROM_INC_PATH      =
#SHORT_NAMES              = NO
#JAVADOC_AUTOBRIEF        = NO
#QT_AUTOBRIEF             = NO

#MULTILINE_CPP_IS_BRIEF   = NO
#INHERIT_DOCS             = YES
#SEPARATE_MEMBER_PAGES    = NO
#TAB_SIZE                 = 4
#ALIASES                  =
#TCL_SUBST                =
#OPTIMIZE_OUTPUT_FOR_C    = NO
#OPTIMIZE_OUTPUT_JAVA     = NO
#OPTIMIZE_FOR_FORTRAN     = NO
#OPTIMIZE_OUTPUT_VHDL     = NO

#EXTENSION_MAPPING        =
#MARKDOWN_SUPPORT         = YES
#AUTOLINK_SUPPORT         = YES
#BUILTIN_STL_SUPPORT      = NO
#CPP_CLI_SUPPORT          = NO
#SIP_SUPPORT              = NO
#IDL_PROPERTY_SUPPORT     = YES
#DISTRIBUTE_GROUP_DOC     = NO
#GROUP_NESTED_COMPOUNDS   = NO # Doxygen v1.8.6 unsupported tag.
#SUBGROUPING              = YES

#INLINE_GROUPED_CLASSES   = NO
#INLINE_SIMPLE_STRUCTS    = NO
#TYPEDEF_HIDES_STRUCT     = NO
#LOOKUP_CACHE_SIZE        = 0
#EXTRACT_ALL              = NO
#EXTRACT_PRIVATE          = NO
#EXTRACT_PACKAGE          = NO
#EXTRACT_STATIC           = NO
#EXTRACT_LOCAL_CLASSES    = YES
#EXTRACT_LOCAL_METHODS    = NO

#EXTRACT_ANON_NSPACES     = NO
#HIDE_UNDOC_MEMBERS       = NO
#HIDE_UNDOC_CLASSES       = NO
#HIDE_FRIEND_COMPOUNDS    = NO
#HIDE_IN_BODY_DOCS        = NO
#INTERNAL_DOCS            = NO
#CASE_SENSE_NAMES         = NO
#HIDE_SCOPE_NAMES         = NO
#HIDE_COMPOUND_REFERENCE  = NO # Doxygen v1.8.6 unsupported tag.
#SHOW_INCLUDE_FILES       = YES

#SHOW_GROUPED_MEMB_INC    = NO
#FORCE_LOCAL_INCLUDES     = NO
#INLINE_INFO              = YES
#SORT_MEMBER_DOCS         = YES
#SORT_BRIEF_DOCS          = NO
#SORT_MEMBERS_CTORS_1ST   = NO
#SORT_GROUP_NAMES         = NO
#SORT_BY_SCOPE_NAME       = NO
#STRICT_PROTO_MATCHING    = NO
#GENERATE_TODOLIST        = YES

#GENERATE_TESTLIST        = YES
#GENERATE_BUGLIST         = YES
#GENERATE_DEPRECATEDLIST  = YES
#ENABLED_SECTIONS         =
#MAX_INITIALIZER_LINES    = 30
#SHOW_USED_FILES          = YES
#SHOW_FILES               = YES
#SHOW_NAMESPACES          = YES
#FILE_VERSION_FILTER      =
#LAYOUT_FILE              =

#CITE_BIB_FILES           =
#QUIET                    = NO
#WARNINGS                 = YES
#WARN_IF_UNDOCUMENTED     = YES
#WARN_IF_DOC_ERROR        = YES
#WARN_NO_PARAMDOC         = NO
#WARN_AS_ERROR            = NO # Doxygen v1.8.6 unsupported tag.
#WARN_FORMAT              = "$file:$line: $text"
#WARN_LOGFILE             =
#INPUT                    =

#INPUT_ENCODING           = UTF-8
#FILE_PATTERNS            =
#RECURSIVE                = NO
#EXCLUDE                  =
#EXCLUDE_SYMLINKS         = NO
#EXCLUDE_PATTERNS         =
#EXCLUDE_SYMBOLS          =
#EXAMPLE_PATH             =
#EXAMPLE_PATTERNS         =
#EXAMPLE_RECURSIVE        = NO

#IMAGE_PATH               =
#INPUT_FILTER             =
#FILTER_PATTERNS          =
#FILTER_SOURCE_FILES      = NO
#FILTER_SOURCE_PATTERNS   =
#USE_MDFILE_AS_MAINPAGE   =
#SOURCE_BROWSER           = NO
#INLINE_SOURCES           = NO
#STRIP_CODE_COMMENTS      = YES
#REFERENCED_BY_RELATION   = NO

#REFERENCES_RELATION      = NO
#REFERENCES_LINK_SOURCE   = YES
#SOURCE_TOOLTIPS          = YES
#USE_HTAGS                = NO
#VERBATIM_HEADERS         = YES
#ALPHABETICAL_INDEX       = YES
#COLS_IN_ALPHA_INDEX      = 5
#IGNORE_PREFIX            =
#GENERATE_HTML            = YES
#HTML_OUTPUT              = html

#HTML_FILE_EXTENSION      = .html
#HTML_HEADER              =
#HTML_FOOTER              =
#HTML_STYLESHEET          =
#HTML_EXTRA_STYLESHEET    =
#HTML_EXTRA_FILES         =
#HTML_COLORSTYLE_HUE      = 220
#HTML_COLORSTYLE_SAT      = 100
#HTML_COLORSTYLE_GAMMA    = 80
#HTML_TIMESTAMP           = NO

#HTML_DYNAMIC_SECTIONS    = NO
#HTML_INDEX_NUM_ENTRIES   = 100
#GENERATE_DOCSET          = NO
#DOCSET_FEEDNAME          = "Doxygen generated docs"
#DOCSET_BUNDLE_ID         = org.doxygen.Project
#DOCSET_PUBLISHER_ID      = org.doxygen.Publisher
#DOCSET_PUBLISHER_NAME    = Publisher
#GENERATE_HTMLHELP        = NO
#CHM_FILE                 =
#HHC_LOCATION             =

#GENERATE_CHI             = NO
#CHM_INDEX_ENCODING       =
#BINARY_TOC               = NO
#TOC_EXPAND               = NO
#GENERATE_QHP             = NO
#QCH_FILE                 =
#QHP_NAMESPACE            = org.doxygen.Project
#QHP_VIRTUAL_FOLDER       = doc
#QHP_CUST_FILTER_NAME     =
#QHP_CUST_FILTER_ATTRS    =

#QHP_SECT_FILTER_ATTRS    =
#QHG_LOCATION             =
#GENERATE_ECLIPSEHELP     = NO
#ECLIPSE_DOC_ID           = org.doxygen.Project
#DISABLE_INDEX            = NO
#GENERATE_TREEVIEW        = NO
#ENUM_VALUES_PER_LINE     = 4
#TREEVIEW_WIDTH           = 250
#EXT_LINKS_IN_WINDOW      = NO
#FORMULA_FONTSIZE         = 10

#FORMULA_TRANSPARENT      = YES
#USE_MATHJAX              = NO
#MATHJAX_FORMAT           = HTML-CSS
#MATHJAX_RELPATH          = http://cdn.mathjax.org/mathjax/latest
#MATHJAX_EXTENSIONS       =
#MATHJAX_CODEFILE         =
#SEARCHENGINE             = YES
#SERVER_BASED_SEARCH      = NO
#EXTERNAL_SEARCH          = NO
#SEARCHENGINE_URL         =

#SEARCHDATA_FILE          = searchdata.xml
#EXTERNAL_SEARCH_ID       =
#EXTRA_SEARCH_MAPPINGS    =
#GENERATE_LATEX           = YES
#LATEX_OUTPUT             = latex
#LATEX_CMD_NAME           = latex
#MAKEINDEX_CMD_NAME       = makeindex
#COMPACT_LATEX            = NO
#PAPER_TYPE               = a4
#EXTRA_PACKAGES           =

#LATEX_HEADER             =
#LATEX_FOOTER             =
#LATEX_EXTRA_STYLESHEET   = # Doxygen v1.8.6 unsupported tag.
#LATEX_EXTRA_FILES        =
#PDF_HYPERLINKS           = YES
#USE_PDFLATEX             = YES
#LATEX_BATCHMODE          = NO
#LATEX_HIDE_INDICES       = NO
#LATEX_SOURCE_CODE        = NO
#LATEX_BIB_STYLE          = plain

#LATEX_TIMESTAMP          = NO # Doxygen v1.8.6 unsupported tag.
#GENERATE_RTF             = NO
#RTF_OUTPUT               = rtf
#COMPACT_RTF              = NO
#RTF_HYPERLINKS           = NO
#RTF_STYLESHEET_FILE      =
#RTF_EXTENSIONS_FILE      =
#RTF_SOURCE_CODE          = NO
#GENERATE_MAN             = NO
#MAN_OUTPUT               = man

#MAN_EXTENSION            = .3
#MAN_SUBDIR               = # Doxygen v1.8.6 unsupported tag.
#MAN_LINKS                = NO
#GENERATE_XML             = NO
#XML_OUTPUT               = xml
#XML_PROGRAMLISTING       = YES
#GENERATE_DOCBOOK         = NO
#DOCBOOK_OUTPUT           = docbook
#DOCBOOK_PROGRAMLISTING   = NO # Doxygen v1.8.6 unsupported tag.
#GENERATE_AUTOGEN_DEF     = NO

#GENERATE_PERLMOD         = NO
#PERLMOD_LATEX            = NO
#PERLMOD_PRETTY           = YES
#PERLMOD_MAKEVAR_PREFIX   =
#ENABLE_PREPROCESSING     = YES
#MACRO_EXPANSION          = NO
#EXPAND_ONLY_PREDEF       = NO
#SEARCH_INCLUDES          = YES
#INCLUDE_PATH             =
#INCLUDE_FILE_PATTERNS    =

#PREDEFINED               =
#EXPAND_AS_DEFINED        =
#SKIP_FUNCTION_MACROS     = YES
#TAGFILES                 =
#GENERATE_TAGFILE         =
#ALLEXTERNALS             = NO
#EXTERNAL_GROUPS          = YES
#EXTERNAL_PAGES           = YES
#PERL_PATH                = /usr/bin/perl
#CLASS_DIAGRAMS           = YES

#MSCGEN_PATH              =
#DIA_PATH                 =
#HIDE_UNDOC_RELATIONS     = YES
#HAVE_DOT                 = NO
#DOT_NUM_THREADS          = 0
#DOT_FONTNAME             = Helvetica
#DOT_FONTSIZE             = 10
#DOT_FONTPATH             =
#CLASS_GRAPH              = YES
#COLLABORATION_GRAPH      = YES

#GROUP_GRAPHS             = YES
#UML_LOOK                 = NO
#UML_LIMIT_NUM_FIELDS     = 10
#TEMPLATE_RELATIONS       = NO
#INCLUDE_GRAPH            = YES
#INCLUDED_BY_GRAPH        = YES
#CALL_GRAPH               = NO
#CALLER_GRAPH             = NO
#GRAPHICAL_HIERARCHY      = YES
#DIRECTORY_GRAPH          = YES

#DOT_IMAGE_FORMAT         = png
#INTERACTIVE_SVG          = NO
#DOT_PATH                 =
#DOTFILE_DIRS             =
#MSCFILE_DIRS             =
#DIAFILE_DIRS             =
#PLANTUML_JAR_PATH        = # Doxygen v1.8.6 unsupported tag.
#PLANTUML_INCLUDE_PATH    = # Doxygen v1.8.6 unsupported tag.
#DOT_GRAPH_MAX_NODES      = 50
#MAX_DOT_GRAPH_DEPTH      = 0

#DOT_TRANSPARENT          = NO
#DOT_MULTI_TARGETS        = NO
#GENERATE_LEGEND          = YES
#DOT_CLEANUP              = YES

