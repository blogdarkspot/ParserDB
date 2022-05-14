############### LIB ########################################################

#set(LIB_NAME sample)

#add_library(${LIB_NAME})
#target_sources(${LIB_NAME} PRIVATE delaf_dictionary.cpp)
#target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
#target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)

#add_library(template::sample ALIAS ${LIB_NAME})

add_subdirectory(lexicon)
add_subdirectory(grammar)