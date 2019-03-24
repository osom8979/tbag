#pragma once
#include <sstream>
#include <string.h>

#include <iostream>

namespace swoosh {
  namespace glsl{
    static std::string formatGLSL(const char* glsl) {
      std::stringstream ss;

      std::size_t size = strlen(glsl) + 1;
      char* input = new char[size];
      char delim[] = ";";
      memcpy(input, glsl, size);
      input[size] = '\0';

      rsize_t strmax = sizeof input;
      char* next_token;

      char* line = strtok_s(input, delim, &next_token);

      while (line != 0)
      {
        ss << line << ";\n";
        line = strtok_s(0, delim, &next_token);
      }

      // delete[] input;

      std::string output = ss.str(); //Get the string stream as a std::string
      std::size_t found = output.find('\n'); // Find the first line break, this is the #version decl
      output.erase(found + 1, 1); // Erase this quote char
      output.erase(output.length() - 3, 2); // Remove the quote and last delim char from macro expansion
      return output;
    }
  }
}

#define SWOOSH_EMBED_TO_STR(...) #__VA_ARGS__
#define GLSL(version, ...)  swoosh::glsl::formatGLSL("#version " #version "\n" SWOOSH_EMBED_TO_STR(#__VA_ARGS__))