set(LIB_NAME gramatico_lexicon)

add_library(${LIB_NAME})
target_sources(${LIB_NAME} PRIVATE lexicon.cpp delas/delas_parser.cpp)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/lexicon)
target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)

add_library(gramatico::lexicon ALIAS ${LIB_NAME})