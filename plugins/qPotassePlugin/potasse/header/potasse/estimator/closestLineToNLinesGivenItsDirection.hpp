#ifndef _ESTIMATOR_CLOSESTLINETONLINESGIVENITSDIRECTION_HPP_
#define _ESTIMATOR_CLOSESTLINETONLINESGIVENITSDIRECTION_HPP_

#include <potasse/shape/line.hpp>

#include <vector>

namespace potasse
{
  namespace estimator
  {
    struct ClosestLineToNLinesGivenItsDirection
    {
      typedef shape::Line<3> Line;
      typedef typename Line::Vertex Vertex;
      typedef typename Line::Normal Normal;

      typedef typename Line::Vector Vector;
      typedef typename Line::Map Map;
      typedef typename Line::ConstMap ConstMap;

      typedef std::vector<Line> Lines;
      static Line estimate(Lines const &lines,
          Normal const &direction);
    };
  }
}

#endif  //_ESTIMATOR_CLOSESTLINETONLINESGIVENITSDIRECTION_HPP_
