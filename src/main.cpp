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
#include "router.hpp"
#include "route.hpp"
#include "local.hpp"

// include log4cxx header files.
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#if !defined(_WIN32)

#include <pthread.h>
#include <signal.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

// Root logger.
LoggerPtr logger(Logger::getLogger("application"));

int main(int argc, char* argv[])
{
  try
  {
    
    // Create application.
    zest::server::application_ptr a(new zest::server::local());
    
    // Declare the supported options.
    std::string config_file;
    std::string log_config_file;
    std::string address;
    std::string port;
    unsigned int threads;
    
    namespace po = boost::program_options;
    
    po::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ("config-file,c", po::value<std::string>(&config_file)
            ->default_value("/usr/local/etc/zest/zest.conf"),
              "set configuration file")
    ;
    
    po::options_description config("Configuration");
    config.add_options()
        ("log-config-file,l", po::value<std::string>(&log_config_file)
            ->default_value("/usr/local/etc/zest/log.conf"), 
              "set log config file")
        ("address,a", po::value<std::string>(&address)
            ->default_value("0.0.0.0"), "set server address")
        ("port,p", po::value<std::string>(&port)
            ->default_value("80"), "set server port")
        ("threads,t", po::value<unsigned int>(&threads)
            ->default_value(4), "set number of threads")
    ;
    
    po::options_description application("Application");
    
    // Let application register its options.
    a->add_options(application);
    
    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(application);

    po::options_description config_file_options;
    config_file_options.add(config).add(application);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << cmdline_options << std::endl;
        return 0;
    }
    
    if (vm.count("version")) {
        std::cout << zest::server::version_string << std::endl;
        return 0;
    }
    
    // Load config file.
    std::ifstream ifs(config_file.c_str());
    po::store(po::parse_config_file(ifs, config_file_options), vm);
    po::notify(vm);
    ifs.close();
    
    std::cout << log_config_file << std::endl;
    
    // Load log config file.
    PropertyConfigurator::configure(log_config_file);
    
    LOG4CXX_INFO(logger,
        "Starting server on " << address << ":" << port << ".");
    
    // Initialize models.
    a->define_models();
    
    // Initialize middlewares.
    a->add_middlewares();

    // Block all signals for background thread.
    sigset_t new_mask;
    sigfillset(&new_mask);
    sigset_t old_mask;
    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);
    
    
    // Create router.
    zest::server::router_ptr r(new zest::server::router());
    a->map_routes(r);
    
    // Run server in background thread.
    zest::server::server s(address, port, threads, r);
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
    
    LOG4CXX_INFO(logger,
        "Stoping server on " << address << ":" << port << ".");
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}

#endif // !defined(_WIN32)
