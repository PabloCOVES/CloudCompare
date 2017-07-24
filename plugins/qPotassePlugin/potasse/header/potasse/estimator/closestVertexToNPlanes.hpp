#ifndef _ESTIMATOR_CLOSESTPOINTTONPLANES_HPP_
#define _ESTIMATOR_CLOSESTPOINTTONPLANES_HPP_

#include <potasse/shape/plane.hpp>

#include <vector>

namespace potasse
{
  namespace estimator
  {
    struct ClosestPointToNPlanes
    {
      typedef shape::Plane Plane;
      typedef typename Plane::Vertex Vertex;
      typedef typename Plane::Normal Normal;

      typedef typename Plane::Vector Vector;
      typedef typename Plane::Map Map;
      typedef typename Plane::ConstMap ConstMap;

      typedef std::vector<Plane> Planes;
      static Vertex estimate(Planes const &planes);
    };
  }
}

#endif  //_ESTIMATOR_CLOSESTPOINTTONPLANES_HPP_
