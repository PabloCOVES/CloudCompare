#ifndef _POTASSE_DETECTION_EXCEPTION_HPP_
#define _POTASSE_DETECTION_EXCEPTION_HPP_

#include<exception>
#include<string>

namespace potasse
{
  namespace detector
  {
    class Exception
      : public std::exception
    {
      public:
        Exception
          ( std::string const &what,
            std::string const &file,
            std::size_t const &line ) throw();

        virtual char const *what() const throw();

        char const *file() const throw();
        std::size_t line() const throw();

      protected:
      private:
        std::string what_;
        std::string file_;
        std::size_t line_;

    };
  }
}

#endif //_POTASSE_DETECTION_EXCEPTION_HPP_
