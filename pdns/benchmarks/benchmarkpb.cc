// #include "rec-protobuf.hh"

#include <google/protobuf/arena.h>
#include <dnsmessage.pb.h>
#include <stdlib.h>
#include <boost/uuid/string_generator.hpp>
#include "iputils.hh"
#include "rec-protobuf.hh"
#include "dnsrecords.hh"

#include <benchmark/benchmark.h>

using google::protobuf::Arena;

std::string lastPB;

__attribute__((noinline))
void fill(PBDNSMessage *m) {
  m->set_type(PBDNSMessage_Type_DNSResponseType);
  m->set_messageid("foo");
  m->set_serveridentity("some-server");
  m->set_socketfamily(PBDNSMessage_SocketFamily_INET);
  m->set_socketprotocol(PBDNSMessage_SocketProtocol_UDP);
  m->set_from("AAAA"); // TODO: IPv4
  m->set_to("BBBB"); // TODO: IPv4
  m->set_inbytes(rand() % 12345);
  m->set_timesec(1597761828);
  m->set_timeusec(10000);
  m->set_id(123);

  auto q = m->mutable_question();
  q->set_qname("example.com.");
  q->set_qtype(1);
  q->set_qclass(1);

  auto r = m->mutable_response();
  r->set_rcode(0);
  r->clear_rrs();
  auto rr = r->add_rrs();
  rr->set_name(".example.com.");
  rr->set_type(1);
  rr->set_class_(1);
  rr->set_ttl(60);
  rr->set_rdata("ABCD");
  r->set_appliedpolicy("some-policy");
  r->clear_tags();
  r->add_tags("tag1");
  r->add_tags("tag2");
  r->set_querytimesec(1597761828);
  r->set_querytimeusec(0);

  m->set_originalrequestorsubnet("QWER");
  m->set_requestorid("some-subscriber");
  m->set_fromport(1234);
  m->set_toport(53);

  lastPB = m->SerializeAsString();
}

boost::uuids::string_generator gen;
boost::uuids::uuid u1 = gen("{01234567-89ab-cdef-0123-456789abcdef}");
ComboAddress req("10.2.3.4");
ComboAddress resp("10.2.3.6");
DNSName dn("example.com.");
DNSName dnr("example.com.");
std::string serverID("myhost");
std::string appliedPolicy("some-policy");
std::unordered_set<std::string> policyTags = {
  "tag1",
  "tag2"
};
Netmask subnet(ComboAddress("1.2.3.4"), 16);
std::string req_id("some-subscriber");

__attribute__((noinline))
void PDNSCreateMessage(const std::vector<DNSRecord>& rrs) {
  RecProtoBufMessage m(DNSProtoBufMessage::Response, u1, &req, &resp, dn, 1, 1, 1223, true, rand() % 12345);
  m.setServerIdentity(serverID);
  m.setTime(1597761828, 10000);

  m.setQuestion(dn, 1, 1);
  
  m.addRRs(rrs, {QType::A});
  m.setAppliedPolicy(appliedPolicy);

  m.setPolicyTags(policyTags);
  m.setQueryTime(1597761828, 0);

  m.setEDNSSubnet(subnet, 14);
  m.setRequestorId(req_id);

  m.setRequestorPort(1234);
  m.setResponderPort(53);

  //m.serialize(lastPB);
}


static void BM_ArenaPerMessage(benchmark::State& state) {
  for (auto _ : state) {
    Arena arena;
    auto m = Arena::CreateMessage<PBDNSMessage>(&arena);
    fill(m);
  }
}
BENCHMARK(BM_ArenaPerMessage);

static void BM_ArenaPerBatch(benchmark::State& state) {
  Arena *arena;
  arena = new Arena;
  int i = 0;
  for (auto _ : state) {
    i++;
    if (i>1000) {
      delete arena;
      arena = new Arena;
      i = 1;
    }
    auto m = Arena::CreateMessage<PBDNSMessage>(arena);
    fill(m);
  }
}
BENCHMARK(BM_ArenaPerBatch);

static void BM_ArenaPerBatchReuseMessage(benchmark::State& state) {
  Arena *arena;
  arena = new Arena;
  int i = 0;
  PBDNSMessage *m = Arena::CreateMessage<PBDNSMessage>(arena);
  for (auto _ : state) {
    i++;
    if (i>1000) {
      delete arena;
      arena = new Arena;
      m = Arena::CreateMessage<PBDNSMessage>(arena);
      i = 1;
    }
    fill(m);
  }
}
BENCHMARK(BM_ArenaPerBatchReuseMessage);

static void BM_NoArena(benchmark::State& state) {
  for (auto _ : state) {
    PBDNSMessage m;
    fill(&m);
  }
}
BENCHMARK(BM_NoArena);

static void BM_NoArenaReuseMessage(benchmark::State& state) {
  PBDNSMessage m;
  for (auto _ : state) {
    fill(&m);
  }
}
BENCHMARK(BM_NoArenaReuseMessage);

static void BM_NoArenaPDNSMessage(benchmark::State& state) {
  reportBasicTypes();
  // Precreate the DNSRecord array to simulate what happens in recursor
  DNSRecord dr;
  dr.d_type = QType::A;
  dr.d_content = DNSRecordContent::mastermake(QType::A, QClass::IN, "192.0.2.142");
  dr.d_name = dnr;
  std::vector<DNSRecord> rrs = { dr };
  
  for (auto _ : state) {
    PDNSCreateMessage(rrs);
  }
}
BENCHMARK(BM_NoArenaPDNSMessage);


BENCHMARK_MAIN();
