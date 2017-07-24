#ifndef _SHAPE_SHAPE_HPP_
#define _SHAPE_SHAPE_HPP_

#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/vertex.hpp>

#include <Eigen/Dense>

namespace potasse
{
  namespace shape
  {
    template <std::size_t S>
      class Shape
      {
        public:
          typedef cloud::Vertex<S> Vertex;
          typedef cloud::Normal<S> Normal;

          typedef Eigen::Matrix<SCALAR, 3, 1> Vector;
          typedef Eigen::Map<Vector> Map;
          typedef Eigen::Map<Vector const> ConstMap;

          enum class Type
          {
            Cone,
            Cylinder,
            Plane,
            Line,
            Sphere
          } type_;

          Shape(Type type,
              Vertex const &vertex,
              Normal const &normal);

          virtual SCALAR distance(Vertex const &vertex) const = 0;
          virtual Normal normal(Vertex const &vertex) const = 0;

          SCALAR angle(Vertex const &vertex, Normal const &normal) const;
          Vertex project(Vertex const &vertex) const;

          Vertex &vertex();
          Vertex const &vertex() const;

          Normal &normal();
          Normal const &normal() const;

          static SCALAR angle(Vector const &lhs, Vector const &rhs);
          static Normal normal(Vertex const &vertex0, Vertex const &vertex1);

        protected:
          Vertex vertex_;
          Normal normal_;

        private:
      };

    typedef Shape<2> Shape2;
    typedef Shape<3> Shape3;
  }
}

#include "impl/shape.hpp"

#endif  //_SHAPE_SHAPE_HPP_
