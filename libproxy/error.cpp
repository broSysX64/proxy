#include "error.hpp"
#include "log_level.hpp"

namespace proxy {

class error_private {
public:
    error_private(error_type type, const std::string &text)
        : type_(type), error_(text) {}

    error_type type_;
    std::string error_;
};

error::error(error_type type, const std::string &text)
    : p_(new error_private(type, text)) {}

error::error(const error &other) = default;

error::~error() {}

error_type error::type() const { return p_->type_; }

error &error::operator=(const error &other) = default;

bool error::has_error() const { return p_->type_ != error_type::NO_ERROR; }

std::string error::text() const { return p_->error_; }

} // namespace proxy
