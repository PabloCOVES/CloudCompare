#include <potasse/shape/cylinder.hpp>
#include <potasse/shape/line.hpp>

namespace potasse
{
  namespace shape
  {
    Cylinder::Cylinder(Vertex const &vertex, Normal const &normal, SCALAR radius)
      : Base{Base::Type::Cylinder, vertex, normal},
      radius_{radius}
    {
    }

    SCALAR
      Cylinder::distance(Vertex const &vertex) const
      {
        Line<3> line{Base::vertex_, Base::normal_};
        return line.distance(vertex) - radius_;
      }

    typename Cylinder::Normal
      Cylinder::normal(Vertex const &vertex) const
      {
        Line<3> line{Base::vertex_, Base::normal_};
        return line.Shape<3>::normal(vertex);
      }

    SCALAR const &
      Cylinder::radius() const
      {
        return radius_;
      }
  }
}
