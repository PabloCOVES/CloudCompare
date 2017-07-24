#ifndef _POTASSE_ESTIMATOR_NEIGHBORSESTIMATOR_HPP_
#define _POTASSE_ESTIMATOR_NEIGHBORSESTIMATOR_HPP_

#include <vector>

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud>
    struct NeighborsEstimator
    {
      struct Neighbors;
      typedef typename PointCloud::value_type Point;
      typedef typename Point::value_type Scalar;

      virtual Neighbors compute(PointCloud const &pointCloud) const = 0;

      struct Neighbors
      {
        typedef std::vector<std::vector<std::size_t>> Indices;
        typedef std::vector<std::vector<Scalar>> Distances;

        Indices indices_;
        Distances distances_;
      };
    };

    template <typename PointCloud>
    struct RadiusNeighborsEstimator: public NeighborsEstimator<PointCloud>
    {
      typedef typename NeighborsEstimator<PointCloud>::Neighbors Neighbors;
      typedef typename NeighborsEstimator<PointCloud>::Point Point;
      typedef typename NeighborsEstimator<PointCloud>::Scalar Scalar;

      typedef Scalar Parameter;

      RadiusNeighborsEstimator(Parameter const &parameter = Scalar());
      Neighbors compute(PointCloud const &pointCloud) const;

      Parameter parameter_;
    };

    template <typename PointCloud>
    struct KnnNeighborsEstimator: public NeighborsEstimator<PointCloud>
    {
      typedef typename NeighborsEstimator<PointCloud>::Neighbors Neighbors;
      typedef typename NeighborsEstimator<PointCloud>::Point Point;
      typedef typename NeighborsEstimator<PointCloud>::Scalar Scalar;

      typedef std::size_t Parameter;

      KnnNeighborsEstimator(Parameter const &parameter = Parameter());

      Neighbors compute(PointCloud const &pointCloud) const;

      Parameter parameter_;
    };
  }
}

#endif  //_POTASSE_ESTIMATOR_NEIGHBORSESTIMATOR_HPP_
