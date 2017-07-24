#ifndef _POTASSE_ESTIMATOR_NORMALS_HPP_
#define _POTASSE_ESTIMATOR_NORMALS_HPP_

#include<potasse/estimator/neighborsEstimator.hpp>

#include<memory>

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud, typename NormalCloud>
    struct NormalsEstimator
    {
      typedef std::shared_ptr<NeighborsEstimator<PointCloud>> Strategy;

      explicit NormalsEstimator(Strategy const &strategy = nullptr);

      NormalCloud compute(PointCloud const &pointCloud);

      static NormalCloud compute(Strategy const &strategy,
                                 PointCloud const &pointCloud);

      Strategy strategy_;
    };
  }
}

#endif  //_POTASSE_ESTIMATOR_NORMALS_HPP_
