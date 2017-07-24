#ifndef _SHAPE_CONE_HPP_
#define _SHAPE_CONE_HPP_

#include <potasse/shape/shape.hpp>

namespace potasse
{
  namespace shape
  {
    class Cone
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

        Cone(Vertex const &vertex, Normal const &normal, SCALAR halfAngle);

        SCALAR distance(Vertex const &vertex) const;
        Normal normal(Vertex const &vertex) const;

      protected:
      private:
        SCALAR halfAngle_;
    };
  }
}

#endif  //_SHAPE_CONE
