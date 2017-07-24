#ifndef _CLOUD_CLOUD_HPP_
#define _CLOUD_CLOUD_HPP_

#include <list>
#include <vector>

namespace potasse
{
  namespace cloud
  {
    template <typename T>
      class Cloud
      : public std::vector<T>
      {
        public:
          typedef std::vector<T> Base;
          typedef typename Base::size_type size_type;
          typedef typename Base::value_type value_type;

          typedef std::list<size_type> Indices;

        protected:
        private:
      };

    template<std::size_t S> class Vertex;
    template<std::size_t S> class Normal;
    typedef Cloud<Vertex<3>> VertexCloud3;
    typedef Cloud<Vertex<2>> VertexCloud2;
    typedef Cloud<Normal<3>> NormalCloud3;
    typedef Cloud<Normal<2>> NormalCloud2;
  }
}

#endif  //_CLOUD_CLOUD_HPP_
