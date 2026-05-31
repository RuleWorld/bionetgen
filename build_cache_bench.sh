g++ -O3 -std=c++17 cache_bench.cpp \
  build/src/libbng_engine.a \
  build/src/libbng_ast.a \
  build/src/libbng_parser.a \
  build/src/libbng_core.a \
  build/src/libnauty.a \
  build/_deps/antlr4_runtime-build/runtime/libantlr4-runtime.a \
  -Isrc -Isrc/parser -Isrc/parser/generated -Ibuild/_deps/antlr4_runtime-src/runtime/Cpp/runtime/src \
  -o cache_bench
