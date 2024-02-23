#include <Rcpp.h>
#include <signal.h>

#include "jointprof_types.h"


using namespace Rcpp;

// [[Rcpp::export]]
List init_profiler_impl() {
  ProfilerDaisyChain* dc = new ProfilerDaisyChain();

  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGPROF);
  sigprocmask(SIG_BLOCK, &sigs, NULL);

  return List::create(XPtr<ProfilerDaisyChain>(dc));
}

// [[Rcpp::export]]
void start_profiler_impl(List ldc, std::string path) {
  XPtr<ProfilerDaisyChain> pdc = ldc[0];
  ProfilerDaisyChain* dc = pdc.get();
  dc->start(std::string(path));

  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGPROF);
  sigprocmask(SIG_UNBLOCK, &sigs, NULL);
}

// [[Rcpp::export]]
void stop_profiler_impl(List ldc) {
  XPtr<ProfilerDaisyChain> pdc = ldc[0];
  ProfilerDaisyChain* dc = pdc.get();
  dc->stop();
}

RcppExport void R_unload_jointprof(DllInfo *dll) {
  ProfilerDaisyChain::remove_handler();
}
