#include "connection.hpp"
#include "handler.hpp"
#include "error.hpp"
#include "log_level.hpp"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace po=boost::program_options;

int parse_options(const po::variables_map& vm, proxy::log_db dbtype)
{
    unsigned short input_local_port = 0;
    unsigned short intput_forward_port = 0;
    std::string input_local_host, input_forward_host, output_log_file;

    if (vm.size() != 5)
        return -1;
    if (vm.count("local-host"   ))
        input_local_host = vm["local-host"].as<std::string>();
    if(vm.count("local-port"))
        input_local_port = vm["local-port"].as<unsigned short>();
    if (vm.count("forward-host"))
        input_forward_host = vm["forward-host"].as<std::string>();
    if(vm.count("forward-port"))
        intput_forward_port = vm["forward-port"].as<unsigned short>();

    std::cout<<proxy::db.at(dbtype)<<": "<<input_local_host<<":"<<input_local_port<<" "<<input_forward_host<<":"<<intput_forward_port<<std::endl;

    boost::asio::io_service ios;
    try
    {
        proxy::handler acceptor(ios,
                                 input_local_host, input_local_port,
                                 input_forward_host, intput_forward_port);

        acceptor.do_accept();

        ios.run();
    }
    catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

int main(int ac, char* av[])
{
    po::options_description desc("General options");
    std::string task_type;
    desc.add_options()
        ("help,h", "Show help")
            ("type,t", po::value<std::string>(&task_type), "Select db: postgres")
        ;
    po::options_description postgres_desc("Postgres options");
    postgres_desc.add_options()
        ("local-host", po::value<std::string>(), "Input local host")
        ("local-port", po::value<unsigned short>(), "Input local port")
        ("forward-host", po::value<std::string>(), "Input forward host")
        ("forward-port", po::value<unsigned short>(), "Input forward port")
        ;

    po::variables_map vm;
    try {
        po::parsed_options parsed = po::command_line_parser(ac, av).options(desc).allow_unregistered().run();
        po::store(parsed, vm);
        po::notify(vm);

        if(task_type == "postgres") {
            desc.add(postgres_desc);
            po::store(po::parse_command_line(ac,av,desc), vm);
            if (!parse_options(vm, proxy::log_db::POSTGRESQL))
                desc.add(postgres_desc);
                std::cout << desc << std::endl;
        }
        else {
            desc.add(postgres_desc);
            std::cout << desc << std::endl;
        }
    }
    catch(std::exception& ex) {
        std::cout << desc << std::endl;
    }
    return 0;
}
