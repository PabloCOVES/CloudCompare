#include <potasse/estimator/closestLineToNLinesGivenItsDirection.hpp>

namespace potasse
{
  namespace estimator
  {
    typename ClosestLineToNLinesGivenItsDirection::Line
      ClosestLineToNLinesGivenItsDirection::estimate(Lines const &lines,
          Normal const &direction)
      {
        typedef Eigen::Matrix<SCALAR, 3, 3> Matrix;

        Matrix matrix{Matrix::Zero()};
        Vector vector{Vector::Zero()};

        for (Line const &line : lines)
        {
          Vertex const &vertex(line.vertex());
          Normal const &normal(line.normal());

          Vector const cross{ConstMap{normal.normal()}.cross(ConstMap{direction.normal()})};
          SCALAR const dot{cross.dot(ConstMap{vertex.vertex()})};

          matrix(0, 0) += cross(0) * cross(0);
          matrix(0, 1) += cross(0) * cross(1);
          matrix(0, 2) += cross(0) * cross(2);
          matrix(1, 0) += cross(0) * cross(1);
          matrix(1, 1) += cross(1) * cross(1);
          matrix(1, 2) += cross(1) * cross(2);
          matrix(2, 0) += cross(0) * cross(2);
          matrix(2, 1) += cross(1) * cross(2);
          matrix(2, 2) += cross(2) * cross(2);

          vector(0) += cross(0) * dot;
          vector(1) += cross(1) * dot;
          vector(2) += cross(2) * dot;
        }

        Vertex vertex;

        Map map{vertex.vertex()};
        map = matrix.householderQr().solve(vector);

        return Line{vertex, direction};
      }
  }
}
