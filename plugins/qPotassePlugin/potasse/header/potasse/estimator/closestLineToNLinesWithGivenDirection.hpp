#ifndef _POTASSE_ESTIMATOR_CLOSESTLINETONLINESWITHGIVENDIRECTION_HPP_
#define _POTASSE_ESTIMATOR_CLOSESTLINETONLINESWITHGIVENDIRECTION_HPP_

#include <list>

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud, typename NormalCloud>
    class ClosestLineToNLinesWithGivenDirection
    {
     public:
      typedef typename PointCloud::value_type Point;
      typedef typename NormalCloud::value_type Normal;
      typedef std::list<std::size_t> Indices;

      static Point compute(PointCloud const &pointCloud,
                           NormalCloud const &normalCloud,
                           Indices const &indices,
                           Normal const &normal);

     protected:
     private:
    };
  }
}

#endif  //_POTASSE_ESTIMATOR_CLOSESTLINETONLINESWITHGIVENDIRECTION_HPP_
