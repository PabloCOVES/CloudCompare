#ifndef _SHAPE_LINE_HPP_
#define _SHAPE_LINE_HPP_

#include <potasse/shape/shape.hpp>

namespace potasse
{
  namespace shape
  {
    template <std::size_t S>
      class Line
      : public Shape<S>
      {
        public:
          typedef Shape<S> Base;
          typedef typename Base::Vertex Vertex;
          typedef typename Base::Normal Normal;

          typedef typename Base::Vector Vector;
          typedef typename Base::Map Map;
          typedef typename Base::ConstMap ConstMap;

          Line(Vertex const &vertex, Normal const &normal);
          Line(Vertex const &vertex0, Vertex const &vertex1);

          using Base::normal;

          SCALAR distance(Vertex const &vertex) const;
          Normal normal(Vertex const &vertex) const;

        protected:
        private:
      };

    typedef Line<2> Line2;
    typedef Line<3> Line3;

  }
}

#include "impl/line.hpp"

#endif  //_SHAPE_LINE_HPP_
