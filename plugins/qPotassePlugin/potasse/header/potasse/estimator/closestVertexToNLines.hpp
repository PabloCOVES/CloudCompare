#ifndef _POTASSE_ESTIMATOR_CLOSESTPOINTTONLINES_HPP_
#define _POTASSE_ESTIMATOR_CLOSESTPOINTTONLINES_HPP_

#include <list>

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud, typename NormalCloud>
    class ClosestPointToNLines
    {
     public:
      typedef typename PointCloud::value_type Point;
      typedef std::list<std::size_t> Indices;

      static Point compute(PointCloud const &pointCloud,
                           NormalCloud const &normalCloud,
                           Indices const &indices);

     protected:
     private:
    };
  }
}

#endif  //_POTASSE_ESTIMATOR_CLOSESTPOINTTONLINES_HPP_
