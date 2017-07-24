#ifndef _ESTIMATOR_CONEESTIMATOR_HPP_
#define _ESTIMATOR_CONEESTIMATOR_HPP_

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/vertex.hpp>

#include <potasse/shape/cone.hpp>

namespace potasse
{
  namespace estimator
  {
    struct ConeEstimator
    {
      typedef cloud::Vertex<3> Vertex;
      typedef cloud::Normal<3> Normal;

      typedef cloud::Cloud<Vertex> VertexCloud;
      typedef cloud::Cloud<Normal> NormalCloud;

      typedef typename Vertex::size_type Size;

      typedef typename VertexCloud::Indices Indices;

      typedef shape::Cone Cone;
      static Cone estimate(VertexCloud const &vertexCloud,
          NormalCloud const &normalCloud,
          Indices const &indices);
    };
  }
}

#endif  //_ESTIMATOR_CONEESTIMATOR_HPP_
