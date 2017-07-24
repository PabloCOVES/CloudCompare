#include <potasse/shape/cone.hpp>

namespace potasse
{
  namespace shape
  {
    Cone::Cone(Vertex const &vertex, Normal const &normal, SCALAR halfAngle)
      : Base{Base::Type::Cone, vertex, normal},
      halfAngle_{halfAngle}
    {
    }

    SCALAR
      Cone::distance(Vertex const &vertex) const
      {
        Vector vector{ConstMap{vertex.vertex()} - ConstMap{Base::vertex_.vertex()}};
        SCALAR angle{Base::angle(vector, ConstMap{Base::normal_.normal()})};
        return vector.norm() * std::sin(std::abs(angle - halfAngle_));
      }

    typename Cone::Normal
      Cone::normal(Vertex const &vertex) const
      {
        Vector vector{ConstMap{vertex.vertex()} - ConstMap{Base::vertex_.vertex()}};

        SCALAR angle{Shape<3>::angle(vector, ConstMap{Base::normal_.normal()})};
        angle -= halfAngle_;

        SCALAR d1{vector.norm() * std::cos(angle)};
        SCALAR d2{d1 / std::cos(halfAngle_)};

        Normal output;
        Map map{output.normal()};
        map = -vector - ConstMap{Base::normal_.normal()} * d2;
        map.normalize();

        return output;
      }
  }
}
