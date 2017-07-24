#include <potasse/estimator/planeEstimator.hpp>

namespace potasse
{
  namespace estimator
  {
    typename PlaneEstimator::Plane
      PlaneEstimator::estimate(VertexCloud const &vertexCloud,
          Indices const &inliers)
      {
        typedef typename Plane::ConstMap ConstMap;
        typedef typename Plane::Map Map;
        typedef typename Plane::Vector Vector;

        Vertex centroid{{0, 0, 0}};
        Map centroidMap{centroid.vertex()};

        for (auto const &index : inliers)
          centroidMap += ConstMap{vertexCloud[index].vertex()};
        centroidMap /= inliers.size();

        typedef Eigen::Matrix<SCALAR, 3, 3> Matrix;
        Matrix matrix{Matrix::Zero()};
        for (auto const &index : inliers)
        {
          Vector vertex{ConstMap{vertexCloud[index].vertex()} - centroidMap};
          matrix += vertex * vertex.transpose();
        }

        Normal normal;
        Map normalMap{normal.normal()};
        normalMap = Eigen::SelfAdjointEigenSolver<Matrix>(matrix).eigenvectors().col(0);

        return Plane{centroid, normal};
      }
  }
}
