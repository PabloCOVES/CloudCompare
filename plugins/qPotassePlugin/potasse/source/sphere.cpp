#include <potasse/shape/sphere.hpp>

namespace potasse
{
  namespace shape
  {
    Sphere::Sphere(Vertex const &vertex, SCALAR radius)
      : Base(Base::Type::Sphere, vertex, Normal{{0, 0, 0}}),
      radius_{radius}
    {
    }

    SCALAR
      Sphere::distance(Vertex const &vertex) const
      {
        return (ConstMap{Base::vertex_.vertex()} - ConstMap{vertex.vertex()}).norm() - radius_;
      }

    typename Sphere::Normal
      Sphere::normal(Vertex const &vertex) const
      {
        Normal normal{};

        Map map{normal.normal()};
        map = ConstMap{vertex.vertex()} - ConstMap{Base::vertex_.vertex()};
        map.normalize();

        return normal;
      }

    SCALAR const &
      Sphere::radius() const
      {
        return radius_;
      }
  }
}
