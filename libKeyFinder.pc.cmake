prefix=@CMAKE_INSTALL_PREFIX@
includedir=${prefix}/include
libdir=${prefix}/@CMAKE_INSTALL_LIBDIR@

Name: libKeyFinder
Description: libKeyFinder can be used to estimate the musical key of digital audio
Version: @LIBKEYFINDER_VERSION@
URL: https://github.com/ibsh/libKeyFinder
Libs: -L${libdir} -lkeyfinder
Cflags: -I${includedir}
