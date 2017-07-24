#include <potasse/estimator/exception.hpp>

namespace potasse
{
  namespace estimator
  {
    Exception::Exception(std::string const &what,
                         std::string const &file,
                         std::size_t const &line) throw()
        : what_(what), file_(file), line_(line)
    {
    }

    char const *
    Exception::what() const throw()
    {
      return what_.c_str();
    }

    char const *
    Exception::file() const throw()
    {
      return file_.c_str();
    }

    size_t
    Exception::line() const throw()
    {
      return line_;
    }
  }
}
