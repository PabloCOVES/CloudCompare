#ifndef _ESTIMATOR_CYLINDERESTIMATOR_HPP_
#define _ESTIMATOR_CYLINDERESTIMATOR_HPP_

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/vertex.hpp>

#include <potasse/shape/cylinder.hpp>

namespace potasse
{
  namespace estimator
  {
    struct CylinderEstimator
    {
      typedef cloud::Vertex<3> Vertex;
      typedef cloud::Normal<3> Normal;

      typedef cloud::Cloud<Vertex> VertexCloud;
      typedef cloud::Cloud<Normal> NormalCloud;

      typedef typename Vertex::size_type Size;

      typedef typename VertexCloud::Indices Indices;

      typedef shape::Cylinder Cylinder;
      static Cylinder estimate(VertexCloud const &vertexCloud,
          NormalCloud const &normalCloud,
          Indices const &indices);
    };
  }
}

#endif  //_ESTIMATOR_CYLINDERESTIMATOR_HPP_
