#include <potasse/estimator/neighborsEstimator.hpp>

#include <flann/flann.hpp>

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud>
    RadiusNeighborsEstimator<PointCloud>::RadiusNeighborsEstimator(Parameter const &parameter)
    : parameter_(parameter)
    {}

    template <typename PointCloud>
    typename RadiusNeighborsEstimator<PointCloud>::Neighbors
    RadiusNeighborsEstimator<PointCloud>::compute(PointCloud const &pointCloud) const
    {
      Neighbors output;

      flann::Matrix<Scalar> matrix(const_cast<Scalar *>(&pointCloud.front().front()), pointCloud.size(), pointCloud.front().size(), sizeof(Point));

      flann::Index<flann::L2<Scalar>> index(matrix, flann::KDTreeSingleIndexParams(10));
      index.buildIndex();

      flann::SearchParams searchParams;
      searchParams.cores = 0;

      index.radiusSearch(matrix, output.indices_, output.distances_, parameter_, searchParams);

      return output;
    }

    template <typename PointCloud>
    KnnNeighborsEstimator<PointCloud>::KnnNeighborsEstimator(Parameter const &parameter)
    : parameter_(parameter)
    {}

    template <typename PointCloud>
    typename KnnNeighborsEstimator<PointCloud>::Neighbors
    KnnNeighborsEstimator<PointCloud>::compute(PointCloud const &pointCloud) const
    {
      Neighbors output;

      flann::Matrix<Scalar> matrix(const_cast<Scalar *>(&pointCloud.front().front()), pointCloud.size(), pointCloud.front().size(), sizeof(Point));

      flann::Index<flann::L2<Scalar>> index(matrix, flann::KDTreeSingleIndexParams(10));
      index.buildIndex();

      flann::SearchParams searchParams;
      searchParams.cores = 0;

      index.knnSearch(matrix, output.indices_, output.distances_, parameter_, searchParams);

      return output;
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/point.hpp>

typedef potasse::cloud::Point<double, 3> Point;
typedef potasse::cloud::Cloud<Point> PointCloud;

template struct potasse::estimator::RadiusNeighborsEstimator<PointCloud>;
template struct potasse::estimator::KnnNeighborsEstimator<PointCloud>;
