if HAVE_PROTOBUF
if HAVE_PROTOC
if HAVE_BENCHMARK

AM_CPPFLAGS += \
	-DRECURSOR=1 \
	$(LIBCRYPTO_INCLUDES) \
	$(BENCHMARK_CFLAGS)

AM_CXXFLAGS = \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DPKGLIBDIR=\"$(pkglibdir)\" \
	-DLOCALSTATEDIR=\"$(socketdir)\"

AM_LDFLAGS = \
	$(PROGRAM_LDFLAGS) \
	$(THREADFLAGS)

ACLOCAL_AMFLAGS = -I m4

CLEANFILES = dnsmessage.pb.cc \
	dnsmessage.pb.h

noinst_PROGRAMS = benchmarkpb

dnslabeltext.cc: dnslabeltext.rl
	$(AM_V_GEN)$(RAGEL) $< -o dnslabeltext.cc

benchmarkpb_SOURCES = \
	protobuf.cc protobuf.hh \
	rec-protobuf.cc rec-protobuf.hh \
	gettime.cc gettime.hh \
	dnsparser.cc dnsparser.hh \
	misc.cc misc.hh \
	dns.cc dns.hh \
	qtype.cc qtype.hh \
	namespaces.hh \
	dnsname.cc dnsname.hh \
	ascii.hh \
	dnswriter.cc dnswriter.hh \
	iputils.cc iputils.hh \
	pdnsexception.hh \
	filterpo.cc filterpo.hh \
	dnsrecords.cc dnsrecords.hh \
	sillyrecords.cc nsecrecords.cc \
	rcpgenerator.cc rcpgenerator.hh \
	dnslabeltext.cc \
	unix_utility.cc utility.hh \
	base32.cc base32.hh \
	base64.cc base64.hh \
	logger.cc logger.hh \
	lock.hh circular_buffer.hh \
	arguments.hh \
	benchmarkpb.cc

benchmarkpb_LDFLAGS = \
	$(AM_LDFLAGS) \
	$(DYNLINKFLAGS) \
	$(LIBCRYPTO_LDFLAGS) \
	$(BENCHMARK_LDFLAGS)

benchmarkpb_LDADD = \
	$(LIBCRYPTO_LIBS) \
	$(BENCHMARK_LIBS)

dnsmessage.pb.cc: dnsmessage.proto
	$(AM_V_GEN)$(PROTOC) --cpp_out=./ $<

BUILT_SOURCES = dnsmessage.pb.cc
benchmarkpb_LDADD += $(PROTOBUF_LIBS)
nodist_benchmarkpb_SOURCES = dnsmessage.pb.cc dnsmessage.pb.h
benchmarkpb$(OBJEXT): dnsmessage.pb.cc

endif
endif
endif
