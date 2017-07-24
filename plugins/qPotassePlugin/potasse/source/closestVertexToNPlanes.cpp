#include <potasse/estimator/closestPointToNPlanes.hpp>

namespace potasse
{
  namespace estimator
  {
    typename ClosestPointToNPlanes::Vertex
      ClosestPointToNPlanes::estimate(Planes const &planes)
      {
        typedef Eigen::Matrix<SCALAR, 3, 3> Matrix;

        Matrix matrix{Matrix::Zero()};
        Vector vector{Vector::Zero()};

        for (Plane const &plane : planes)
        {
          Vertex const &vertex(plane.vertex());
          Normal const &normal(plane.normal());

          matrix(0, 0) += normal[0] * normal[0];
          matrix(0, 1) += normal[0] * normal[1];
          matrix(0, 2) += normal[0] * normal[2];

          matrix(1, 0) += normal[1] * normal[0];
          matrix(1, 1) += normal[1] * normal[1];
          matrix(1, 2) += normal[1] * normal[2];

          matrix(2, 0) += normal[2] * normal[0];
          matrix(2, 1) += normal[2] * normal[1];
          matrix(2, 2) += normal[2] * normal[2];

          SCALAR scalar(ConstMap{vertex.vertex()}.dot(ConstMap{normal.normal()}));
          vector(0) += normal[0] * scalar;
          vector(1) += normal[1] * scalar;
          vector(2) += normal[2] * scalar;
        }

        Vertex output;

        Map map{output.vertex()};
        map = matrix.householderQr().solve(vector);

        return output;
      }
  }
}
