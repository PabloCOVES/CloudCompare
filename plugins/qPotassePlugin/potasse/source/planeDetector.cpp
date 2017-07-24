#include <potasse/detector/planeDetector.hpp>

#include <potasse/shape/plane.hpp>

#include<Eigen/Dense>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud, typename NormalCloud>
    PlaneDetector<PointCloud, NormalCloud>::PlaneDetector(
        Parameter const &parameter,
        TreeParameter const &treeParameter)
        : ShapeDetector<PointCloud, NormalCloud>(parameter, treeParameter)
    {
    }

    template <typename PointCloud, typename NormalCloud>
    std::size_t
    PlaneDetector<PointCloud, NormalCloud>::minimum() const
    {
      return 3;
    }

    template <typename PointCloud, typename NormalCloud>
    typename PlaneDetector<PointCloud, NormalCloud>::ShapePtr
    PlaneDetector<PointCloud, NormalCloud>::create(PointCloud const &pointCloud,
                                                   NormalCloud const &,
                                                   Indices const &inliers) const
    {
      return create(pointCloud, inliers);
    }

    template <typename PointCloud, typename NormalCloud>
    typename PlaneDetector<PointCloud, NormalCloud>::ShapePtr
    PlaneDetector<PointCloud, NormalCloud>::create(PointCloud const &pointCloud,
                                                   Indices const &inliers) const
    {
      typedef typename potasse::shape::Plane<Scalar>::Vector Vector;

      Vector centroid(Vector::Zero());
      for (size_t const &inlier : inliers)
        centroid += pointCloud[inlier].point();
      centroid /= inliers.size();

      typedef Eigen::Matrix<Scalar, 3, 3> Matrix;
      Matrix matrix(Matrix::Zero());
      for (size_t const &inlier : inliers)
      {
        Vector const point(pointCloud[inlier].point() - centroid);
        matrix += point * point.transpose();
      }
      return std::make_shared<potasse::shape::Plane<Scalar>>(centroid, Eigen::SelfAdjointEigenSolver<Matrix>(matrix).eigenvectors().col(0));
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/point.hpp>

typedef potasse::cloud::Point<double, 3> Point;
typedef potasse::cloud::Normal<double, 3> Normal;

typedef potasse::cloud::Cloud<Point> PointCloud;
typedef potasse::cloud::Cloud<Normal> NormalCloud;

template class potasse::detector::PlaneDetector<PointCloud, NormalCloud>;
