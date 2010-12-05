//................................:: ZEST ::....................................
//
// main.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include "version.hpp"
#include "server.hpp"
#include "signals.hpp"
#include "router.hpp"
#include "route.hpp"

#if !defined(_WIN32)

#include <pthread.h>
#include <signal.h>

int main(int argc, char* argv[])
{
  try
  {
    // Declare the supported options.
    
    std::string config_file;
    std::string address;
    std::string port;
    unsigned int threads;
    std::string redis_servers;
    
    namespace po = boost::program_options;
    
    po::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ("config-file,c", po::value<std::string>(&config_file)
            ->default_value("/usr/local/etc/zest.conf"),
              "set configuration file")
    ;
    
    po::options_description config("Configuration");
    config.add_options()
        ("address,a", po::value<std::string>(&address)
            ->default_value("0.0.0.0"), "set server address")
        ("port,p", po::value<std::string>(&port)
            ->default_value("80"), "set server port")
        ("threads,t", po::value<unsigned int>(&threads)
            ->default_value(4), "set number of threads")
        ("redis-servers,r", po::value<std::string>(&redis_servers)
            ->default_value("127.0.0.1:6379:1"), "set redis servers")
    ;
    
    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config);

    po::options_description config_file_options;
    config_file_options.add(config);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);
    
    std::ifstream ifs(config_file.c_str());
    po::store(po::parse_config_file(ifs, config_file_options), vm);
    po::notify(vm);
    
    ifs.close();

    if (vm.count("help")) {
        std::cout << cmdline_options << std::endl;
        return 0;
    }
    
    if (vm.count("version")) {
        std::cout << zest::version::version_string << std::endl;
        return 0;
    }
    
    // Create router and bind it to on_request signal.
    zest::server::router_ptr r(new zest::server::router());
    zest::server::request_sig.connect(
      boost::bind(&zest::server::router::process, r, _1, _2));
    zest::server::route_ptr home = zest::server::route::create("/index.:format")
      ->add_param("format",
        zest::server::param_option<std::string>("[a-z0-9]{3,5}"));
    r->map(home);
    home->match("/index.json");

    // Block all signals for background thread.
    sigset_t new_mask;
    sigfillset(&new_mask);
    sigset_t old_mask;
    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

    // Run server in background thread.
    zest::server::server s(address, port, threads);
    boost::thread t(boost::bind(&zest::server::server::run, &s));

    // Restore previous signals.
    pthread_sigmask(SIG_SETMASK, &old_mask, 0);

    // Wait for signal indicating time to shut down.
    sigset_t wait_mask;
    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, SIGINT);
    sigaddset(&wait_mask, SIGQUIT);
    sigaddset(&wait_mask, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
    int sig = 0;
    sigwait(&wait_mask, &sig);

    // Stop the server.
    s.stop();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}

#endif // !defined(_WIN32)
