#include <potasse/detector/sphereDetector.hpp>

#include <potasse/estimator/closestPointToNLines.hpp>

#include <potasse/shape/sphere.hpp>

#include <Eigen/Dense>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud, typename NormalCloud>
    SphereDetector<PointCloud, NormalCloud>::SphereDetector(
        Parameter const &parameter,
        TreeParameter const &treeParameter)
        : ShapeDetector<PointCloud, NormalCloud>(parameter, treeParameter)
    {
    }

    template <typename PointCloud, typename NormalCloud>
    std::size_t
    SphereDetector<PointCloud, NormalCloud>::minimum() const
    {
      return 2;
    }

    template <typename PointCloud, typename NormalCloud>
    typename SphereDetector<PointCloud, NormalCloud>::ShapePtr
    SphereDetector<PointCloud, NormalCloud>::create(PointCloud const &pointCloud,
                                                    NormalCloud const &normalCloud,
                                                    Indices const &inliers) const
    {
      typedef typename PointCloud::value_type Point;

      Point center(potasse::estimator::ClosestPointToNLines<PointCloud, NormalCloud>::compute(pointCloud, normalCloud, inliers));

      Scalar radius(0);
      for (std::size_t i : inliers)
        radius += (center.point() - pointCloud[i].point()).norm();
      radius /= inliers.size();

      return std::make_shared<shape::Sphere<Scalar>>(center.point(), radius);
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

template class potasse::detector::SphereDetector<PointCloud, NormalCloud>;
