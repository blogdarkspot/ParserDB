set(LIB_NAME gramatico_grammar)

#add_library(${LIB_NAME} SHARED)
#target_sources(${LIB_NAME} PRIVATE grammar.cpp)
#target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/grammar ${CMAKE_SOURCE_DIR}/include)
#target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)

#add_library(gramatico::grammar ALIAS ${LIB_NAME})


find_package(PythonLibs 3 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

set(PYTHON_MODULE_NAME gramatico)
# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")
add_library(${PYTHON_MODULE_NAME} MODULE grammar_ext.cpp)
target_include_directories(${PYTHON_MODULE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${PYTHON_INCLUDE_DIRS})
target_compile_features(${PYTHON_MODULE_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PYTHON_MODULE_NAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} gramatico::lexicon)

add_executable(main grammar.cpp)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include/grammar ${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIR} )
target_compile_features(main PUBLIC cxx_std_20)
target_link_libraries(main gramatico::lexicon)

add_library(${LIB_NAME})
target_sources(${LIB_NAME} PRIVATE dummy.cpp)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/grammar ${CMAKE_SOURCE_DIR}/include )
target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)
target_link_libraries(${LIB_NAME} gramatico::lexicon)
add_library(gramatico::grammar ALIAS ${LIB_NAME})
