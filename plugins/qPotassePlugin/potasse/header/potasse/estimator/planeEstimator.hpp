#ifndef _ESTIMATOR_PLANEESTIMATOR_HPP_
#define _ESTIMATOR_PLANEESTIMATOR_HPP_

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/vertex.hpp>

#include <potasse/shape/plane.hpp>

namespace potasse
{
  namespace estimator
  {
    struct PlaneEstimator
    {
      typedef cloud::Vertex<3> Vertex;
      typedef cloud::Normal<3> Normal;

      typedef cloud::Cloud<Vertex> VertexCloud;

      typedef typename Vertex::size_type Size;

      typedef typename VertexCloud::Indices Indices;

      typedef shape::Plane Plane;
      static Plane estimate(VertexCloud const &vertexCloud,
          Indices const &indices);
    };
  }
}

#endif  //_ESTIMATOR_PLANEESTIMATOR_HPP_
