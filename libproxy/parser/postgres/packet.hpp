#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include <boost/archive/tmpdir.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/cstdint.hpp>
#include <boost/endian/conversion.hpp>

#include "logger/logger.hpp"
#include "logger/logger_factory.hpp"

#include "log_level.hpp"

namespace proxy {
namespace parser {
namespace postgres {

class header_psql
{
    unsigned char command_;
    int length_;
    std::string query_;
public:
    typedef std::shared_ptr<header_psql> header_psql_ptr;
    header_psql() {
    }

    header_psql(unsigned char command, int length, std::string query) : command_(command)
                                                                             , length_(length)
                                                                              , query_(query) {
    }

    log_protocol_psql command() {
        if (command_ == 'Q')
            return  log_protocol_psql::QUERY;

        return  log_protocol_psql::NONE;
    }
    int length() {return length_; }
    std::string query() {
        return query_;
    }

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /* file_version */){
        ar & command_;
        ar & length_;
        ar & query_;
    }
};

class parser_header
{
public:
    typedef boost::shared_ptr<parser_header> parse_header_ptr;

    parser_header() : query_()
             , buffer_()
             , state_(parser_state::startup_message) {
    }

    header_psql::header_psql_ptr parse_buffer(char* buf, const size_t size)
    {
        if (!size)
        {
            return std::make_shared<header_psql>();
        }

        buffer_.append(std::string(reinterpret_cast<const char *>(buf), size));

        while (buffer_.size() > 5)
        {
            if (state_ == parser_state::startup_message && buffer_.size() > 15)
            {
                auto package_size = boost::endian::endian_reverse(*reinterpret_cast<const boost::uint32_t *>(buffer_.c_str() + 8));

                if (buffer_.size() >= package_size)
                {
                    buffer_ = buffer_.substr(package_size + 8);
                    state_ = parser_state::commands;
                    continue;
                }
            }

            if (state_ == parser_state::commands)
            {
                auto package_size = boost::endian::endian_reverse(*reinterpret_cast<const boost::uint32_t *>(buffer_.c_str() + 1));
                if (buffer_.size() >= package_size)
                {
                    unsigned char comamnd = buffer_[0];
                    int length = std::atoi(buffer_.substr(1, 4).c_str());
                    query_ = buffer_.substr(5, package_size - 5);

                    buffer_ = buffer_.substr(std::min(static_cast<size_t>(package_size) + 1, buffer_.size()));
                    return std::make_shared<header_psql>(comamnd, length, query_);
                }
            }
            break;
        }

        return std::make_shared<header_psql>();
    }
    const std::string &get_last_query() {
        return query_;
    }

private:
    std::string query_;
    std::string buffer_;
    parser_state state_;
};

} // namespace postgres
} // namespace parser
} // namespace proxy

#endif // PACKET_HPP
