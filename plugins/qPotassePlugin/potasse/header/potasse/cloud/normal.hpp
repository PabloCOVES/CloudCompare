#ifndef _CLOUD_NORMAL_HPP_
#define _CLOUD_NORMAL_HPP_

#include <array>

namespace potasse
{
  namespace cloud
  {
    template <std::size_t S>
      class Normal
      : public std::array<SCALAR, S>
      {
        public:
          typedef std::array<SCALAR, S> Base;
          typedef typename Base::const_pointer const_pointer;
          typedef typename Base::pointer pointer;
          typedef typename Base::size_type size_type;
          typedef typename Base::value_type value_type;

          explicit Normal() = default;
          explicit Normal(Base const &base);

          pointer normal();
          const_pointer normal() const;

        protected:
        private:
      };

    typedef Normal<2> Normal2;
    typedef Normal<3> Normal3;
  }
}

#include "impl/normal.hpp"

#endif  //_CLOUD_NORMAL_HPP_
