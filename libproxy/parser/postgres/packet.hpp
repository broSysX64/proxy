#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/archive/tmpdir.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

namespace parser {
namespace postgres {

class header
{
    friend std::ostream & operator<<(std::ostream &os, const header &h);
    friend class boost::serialization::access;
    unsigned char command;
    int length;
    std::string query;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /* file_version */){
        ar & command & length & &query;
    }
public:
    header(){}
    header(unsigned char _command, int _length, std::string _query)
        : command(_command)
        , length(_length)
        , query(_query) {
    }
};

std::ostream & operator<<(std::ostream &os, const header &h)
{
    return os << h.command << h.length << h.query;
}

} // postgres
} // parser

#endif // PACKET_HPP
