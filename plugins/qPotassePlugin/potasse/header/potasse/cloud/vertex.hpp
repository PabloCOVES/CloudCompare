#ifndef _CLOUD_VERTEX_HPP_
#define _CLOUD_VERTEX_HPP_

#include <array>

namespace potasse
{
  namespace cloud
  {
    template <std::size_t S>
      class Vertex
      : public std::array<SCALAR, S>
      {
        public:
          typedef std::array<SCALAR, S> Base;
          typedef typename Base::const_pointer const_pointer;
          typedef typename Base::pointer pointer;
          typedef typename Base::size_type size_type;
          typedef typename Base::value_type value_type;

          explicit Vertex() = default;
          explicit Vertex(Base const &base);

          pointer vertex();
          const_pointer vertex() const;

        protected:
        private:
      };

    typedef Vertex<2> Vertex2;
    typedef Vertex<3> Vertex3;
  }
}

#include "impl/vertex.hpp"

#endif  //_CLOUD_VERTEX_HPP_
