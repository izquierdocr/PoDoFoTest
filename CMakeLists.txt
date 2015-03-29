cmake_minimum_required(VERSION 2.8)

project(podofotest)

SET (PoDoFo_INC_DIRS "/usr/local/inlude")
SET (PoDoFo_LIB "podofo")
SET(podofo_static_LIB_DEPENDS "general;/usr/lib/x86_64-linux-gnu/libfontconfig.so;general;/usr/lib/x86_64-linux-gnu/libz.so;general;/usr/lib/x86_64-linux-gnu/libidn.so;general;/usr/lib/x86_64-linux-gnu/libcrypto.so;general;/usr/lib/x86_64-linux-gnu/libjpeg.so;general;-lpthread;general;/usr/lib/x86_64-linux-gnu/libfreetype.so;general;/usr/lib/x86_64-linux-gnu/libpng.so;general;/usr/lib/x86_64-linux-gnu/libz.so;general;/usr/lib/x86_64-linux-gnu/libtiff.so;")

include_directories( ${PoDoFo_INC_DIRS} )

add_executable(podofotest main.cpp)

target_link_libraries( podofotest ${PoDoFo_LIB} )
target_link_libraries( podofotest ${podofo_static_LIB_DEPENDS} )

