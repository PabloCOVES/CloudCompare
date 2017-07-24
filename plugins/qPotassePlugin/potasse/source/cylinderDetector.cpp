#include <potasse/detector/cylinderDetector.hpp>

#include <potasse/estimator/mostOrthogonalVectorToNVectors.hpp>
//#include <potasse/estimator/closestLineToNLinesWithGivenDirection.hpp>
#include<potasse/estimator/closestVertexToNLines.hpp>

#include <potasse/shape/cylinder.hpp>

#include <Eigen/Dense>

namespace potasse
{
  namespace detector
  {
    template <typename VertexCloud, typename NormalCloud>
    CylinderDetector<VertexCloud, NormalCloud>::CylinderDetector(
        Parameter const &parameter,
        TreeParameter const &treeParameter)
        : ShapeDetector<VertexCloud, NormalCloud>(parameter, treeParameter)
    {
    }

    template <typename VertexCloud, typename NormalCloud>
    std::size_t
    CylinderDetector<VertexCloud, NormalCloud>::minimum() const
    {
      return 2;
    }

    template <typename VertexCloud, typename NormalCloud>
    typename CylinderDetector<VertexCloud, NormalCloud>::ShapePtr
    CylinderDetector<VertexCloud, NormalCloud>::create(VertexCloud const &vertexCloud,
                                                    NormalCloud const &normalCloud,
                                                    Indices const &inliers) const
    {
      typedef typename VertexCloud::value_type Vertex;
      typedef typename NormalCloud::value_type Normal;

      Normal axis(potasse::estimator::MostOrthogonalVectorToNVectors<NormalCloud>::compute(normalCloud, inliers));
      //Vertex vertex(potasse::estimator::ClosestLineToNLinesWithGivenDirection<VertexCloud, NormalCloud>::compute(vertexCloud, normalCloud, inliers, axis));
      Vertex vertex(potasse::estimator::ClosestVertexToNLines<VertexCloud, NormalCloud>::compute(vertexCloud, normalCloud, inliers));

      Scalar radius(0), length(0);
      for (std::size_t const &i : inliers)
      {
        radius += ((vertex.vertex() - vertexCloud[i].vertex()).cross(axis.normal())).norm();

        Vertex tmpVertex; tmpVertex.vertex() = (vertex.vertex() - vertexCloud[i].vertex());
        Scalar angle(std::acos(normalCloud[i].normal().dot(tmpVertex.vertex() / tmpVertex.vertex().norm())));
        tmpVertex.vertex() += tmpVertex.vertex() * std::sin(angle);

        Scalar tmpLength((vertex.vertex() - tmpVertex.vertex()).norm());
        if (tmpLength > length) length = tmpLength;
      }
      radius /= inliers.size();
      length *= 2.;

      return std::make_shared<shape::Cylinder<Scalar>>(vertex.vertex(), axis.normal(), radius, length);
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/vertex.hpp>

typedef potasse::cloud::Vertex<3> Vertex;
typedef potasse::cloud::Normal<3> Normal;

typedef potasse::cloud::Cloud<Vertex> VertexCloud;
typedef potasse::cloud::Cloud<Normal> NormalCloud;

template class potasse::detector::CylinderDetector<VertexCloud, NormalCloud>;
