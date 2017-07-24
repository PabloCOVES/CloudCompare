#ifndef _SHAPE_SPHERE_HPP_
#define _SHAPE_SPHERE_HPP_

#include <potasse/shape/shape.hpp>

namespace potasse
{
  namespace shape
  {
    class Sphere
      : public Shape<3>
    {
      public:
        typedef Shape<3> Base;
        typedef typename Base::Vertex Vertex;
        typedef typename Base::Normal Normal;

        typedef typename Base::Vector Vector;
        typedef typename Base::Map Map;
        typedef typename Base::ConstMap ConstMap;

        Sphere(Vertex const &vertex, SCALAR radius);

        SCALAR distance(Vertex const &vertex) const;
        Normal normal(Vertex const &vertex) const;

        SCALAR const &radius() const;

      protected:
      private:
        SCALAR radius_;
    };
  }
}

#endif  //_SHAPE_SPHERE_HPP_
