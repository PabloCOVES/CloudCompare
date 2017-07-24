#ifndef _SHAPE_PLANE_HPP_
#define _SHAPE_PLANE_HPP_

#include <potasse/shape/shape.hpp>

namespace potasse
{
  namespace shape
  {
    struct Plane
      : Shape<3>
    {
      typedef Shape<3> Base;
      typedef typename Base::Vertex Vertex;
      typedef typename Base::Normal Normal;

      typedef typename Base::Vector Vector;
      typedef typename Base::Map Map;
      typedef typename Base::ConstMap ConstMap;

      Plane(Vertex const &vertex, Normal const &normal);
      Plane(Vertex const &vertex0,
          Vertex const &vertex1,
          Vertex const &vertex2);

      using Base::normal;

      SCALAR distance(Vertex const &vertex) const;
      Normal normal(Vertex const &vertex) const;

      static Normal normal(Vertex const &vertex0, Vertex const &vertex1, Vertex const &vertex2);
    };
  }
}

#endif  //_SHAPE_PLANE_HPP_
