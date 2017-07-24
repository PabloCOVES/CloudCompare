#ifndef _SHAPE_CYLINDER_HPP_
#define _SHAPE_CYLINDER_HPP_

#include <potasse/shape/shape.hpp>

namespace potasse
{
  namespace shape
  {
    class Cylinder
      : public Shape<3>
    {
      public:
        typedef Shape<3> Base;
        typedef typename Base::Vertex Vertex;
        typedef typename Base::Normal Normal;

        typedef typename Base::Vector Vector;
        typedef typename Base::Map Map;
        typedef typename Base::ConstMap ConstMap;

        using Base::normal;

        Cylinder(Vertex const &vertex, Normal const &normal, SCALAR radius);

        SCALAR distance(Vertex const &vertex) const;
        Normal normal(Vertex const &vertex) const;

        SCALAR const &radius() const;

      protected:
      private:
        SCALAR radius_;
    };
  }
}

#endif  //_SHAPE_CYLINDER_HPP_
