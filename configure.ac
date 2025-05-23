#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([PSP Software Development Kit], m4_esyscmd_s([cat VERSION]),
    [https://github.com/pspdev/pspsdk/issues], [pspsdk],
    [https://pspdev.github.io/pspsdk])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([tools/bin2c.c])
AC_CONFIG_HEADERS([config.h])
AC_PSPDEV_PATH

AM_INIT_AUTOMAKE([1.14 foreign])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PSPDEV_TOOLCHAIN
AC_PROG_CC
# These will actually be overridden by the psp-specific counterparts.  They are
# here mainly to shut automake up.
AC_PROG_CXX
AM_PROG_AS
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h malloc.h stdlib.h string.h unistd.h zlib.h])
AC_CHECK_LIB([z], [compress])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_BIGENDIAN

AC_ARG_WITH(gprofflags,  [ --with-gprofflags  build with gprof flag included], [], [--with_gprofflags=no])
AC_ARG_WITH(pthread,  [ --with-pthread  build with pthread support], [], [with_pthread=yes])

# Checks for library functions.
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([getcwd strchr])

# Doxygen.
DX_HTML_FEATURE(ON)
DX_DOT_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(PSPSDK, Doxyfile, doc/build)

# Override the default prefix to point to where the pspdev tools should be installed.
# The binaries will end up in $prefix/bin, but pspsdk itself will end up in $prefix/$target/sdk.
# TODO: We should see if the user will ever want to override this.
prefix="$pspdev"
psp_targetdir="psp"

# Create the pspsdk installation paths.
pspsdk_topdir="$psp_targetdir/sdk"
pspsdk="$prefix/$pspsdk_topdir"
pspsdk_includedir="$pspsdk/include"
pspsdk_libdir="$pspsdk/lib"
PSPSDK="$pspsdk"
PSPSDK_INCLUDEDIR="$pspsdk_includedir"
PSPSDK_LIBDIR="$pspsdk_libdir"
AC_DEFINE_UNQUOTED(PSPSDK_TOPDIR, ["$pspsdk_topdir"],
          [Toplevel directory where PSPSDK will be installed, relative to $prefix])
AC_SUBST(PSPSDK)
AC_SUBST(PSPSDK_INCLUDEDIR)
AC_SUBST(PSPSDK_LIBDIR)

# CFLAGS and CXXFLAGS used to build pspsdk libraries.
PSPSDK_CFLAGS="$CFLAGS -mno-gpopt -Wall -Werror -D_PSP_FW_VERSION=600"
if test "$with_gprofflags" = yes ; then
	PSPSDK_CFLAGS="$PSPSDK_CFLAGS -pg -g"
fi
PSPSDK_CXXFLAGS="$PSPSDK_CFLAGS -fno-exceptions -fno-rtti"
if test "$with_pthread" = no ; then
	PSPSDK_CFLAGS="$PSPSDK_CFLAGS -DPSP_WITHOUT_PTHREAD"
fi

AC_SUBST(PSPSDK_CFLAGS)
AC_SUBST(PSPSDK_CXXFLAGS)

# Turn on all warnings (for host programs).
if test x$ac_compiler_gnu = xyes; then
   CFLAGS="$CFLAGS $CFLAGS_FOR_HOST -Wall"
fi
AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/atrac3/Makefile
                 src/audio/Makefile
                 src/base/Makefile
                 src/base/linkfile.prx
                 src/ctrl/Makefile
                 src/debug/Makefile
                 src/display/Makefile
                 src/dmac/Makefile
                 src/fpu/Makefile
                 src/ge/Makefile
                 src/gu/Makefile
                 src/gum/Makefile
                 src/hprm/Makefile
                 src/kernel/Makefile
                 src/libcglue/Makefile
                 src/libpthreadglue/Makefile
                 src/modinfo/Makefile
                 src/mp3/Makefile
                 src/mpeg/Makefile
                 src/nand/Makefile
                 src/net/Makefile
                 src/openpsid/Makefile
                 src/power/Makefile
                 src/prof/Makefile
                 src/registry/Makefile
                 src/rtc/Makefile
                 src/sircs/Makefile
                 src/sdk/Makefile
                 src/startup/Makefile
                 src/umd/Makefile
                 src/usb/Makefile
                 src/usbstor/Makefile
                 src/user/Makefile
                 src/utility/Makefile
                 src/vaudio/Makefile
                 src/vfpu/Makefile
                 src/video/Makefile
                 src/vsh/Makefile
                 src/wlan/Makefile
                 src/samples/Makefile
                 tools/Makefile
                 tools/PrxEncrypter/Makefile])
AC_OUTPUT
