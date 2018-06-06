if DEBUG
DBGFLAGS = -g -DDEBUG
endif

DEF_SAGA           = -D_SAGA_LINUX -D_TYPEDEF_BYTE -D_TYPEDEF_WORD -D_SAGA_UNICODE
CXX_INCS           = -I$(top_srcdir)/src/saga_core
AM_CXXFLAGS        = -fPIC $(CXX_INCS) $(DEF_SAGA) $(DBGFLAGS) $(GOMPFLAGS)
AM_LDFLAGS         = -fPIC -shared -avoid-version
pkglib_LTLIBRARIES = libnetwork_topologize.la
libnetwork_topologize_la_SOURCES =\
topologize.cpp\
stp.cpp\
TLB_Interface.cpp\
topologize.h\
stp.h\
TLB_Interface.h

libnetwork_topologize_la_LIBADD = $(top_srcdir)/src/saga_core/saga_api/libsaga_api.la

