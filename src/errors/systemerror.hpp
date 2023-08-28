#include <stdexcept>

struct SystemError : public std::logic_error
{
    SystemError(std::string const &msg)
        : std::logic_error(msg){};
};
