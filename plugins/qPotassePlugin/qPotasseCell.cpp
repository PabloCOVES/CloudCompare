#include "qPotasseCell.h"
#include <iostream>

#include <cassert>

#include <ReferenceCloud.h>
#include <ccCylinder.h>
#include <ccGLMatrix.h>
#include <ccPlane.h>

#include <Eigen/Dense>
#include <Eigen/Geometry>

Cell::PlanePtr detectPlane(CCLib::ReferenceCloud const &referenceCloud) {
    typedef Eigen::Vector3f Vector;

    Vector centroid(Vector::Zero());
    CCVector3 point;
    for (unsigned int i{0}, i_{referenceCloud.size()}; i < i_; ++i) {
        referenceCloud.getPoint(i, point);
        centroid[0] += static_cast<float>(point.x);
        centroid[1] += static_cast<float>(point.y);
        centroid[2] += static_cast<float>(point.z);
    }
    centroid /= referenceCloud.size();

    typedef Eigen::Matrix<float, 3, 3> Matrix;
    Matrix matrix(Matrix::Zero());

    for (unsigned int i{0}, i_{referenceCloud.size()}; i < i_; ++i) {
        referenceCloud.getPoint(i, point);

        Vector tmp;
        tmp[0] = static_cast<float>(point.x);
        tmp[1] = static_cast<float>(point.y);
        tmp[2] = static_cast<float>(point.z);

        tmp -= centroid;
        matrix += tmp * tmp.transpose();
    }

    Eigen::Vector3f normal{
        Eigen::SelfAdjointEigenSolver<Matrix>(matrix).eigenvectors().col(0)};

    Eigen::Quaternionf rotation{
        Eigen::Quaternionf::FromTwoVectors(Vector::UnitZ(), normal)};

    Eigen::Transform<float, 3, Eigen::Affine> transform{
        rotation.toRotationMatrix()};
    transform.translate(centroid);

    ccGLMatrix glMatrix;
    for (unsigned int i{0}; i < 16; ++i)
        glMatrix.data()[i] = transform.data()[i];

    return std::make_shared<ccPlane>(1, 1, &glMatrix);
}

typename Eigen::Vector3f mostOrthogonalVectorToNVectors(
    std::vector<Eigen::Vector3f> const &normals) {
    using Vector = Eigen::Vector3f;
    using Matrix = Eigen::Matrix3f;

    Vector output{Vector::Zero()};
    Matrix matrix{Matrix::Zero()};
    for (Eigen::Vector3f const &normal : normals) {
        matrix(0, 0) += normal(0) * normal(0);
        matrix(0, 1) += normal(0) * normal(1);
        matrix(0, 2) += normal(0) * normal(1);

        matrix(1, 0) += normal(0) * normal(1);
        matrix(1, 1) += normal(1) * normal(1);
        matrix(1, 2) += normal(1) * normal(2);

        matrix(2, 0) += normal(0) * normal(2);
        matrix(2, 1) += normal(1) * normal(2);
        matrix(2, 2) += normal(2) * normal(2);
    }

    return Eigen::SelfAdjointEigenSolver<Matrix>(matrix).eigenvectors().col(0);
}

typename Eigen::Vector3f closestPointToNLines(
    std::vector<Eigen::Vector3f> const &vertices,
    std::vector<Eigen::Vector3f> const &normals) {
    using Vector = Eigen::Vector3f;
    using Matrix = Eigen::Matrix3f;

    Vector vector{Vector::Zero()};
    Matrix matrix{Matrix::Zero()};

    for (unsigned long i{0}, i_{vertices.size()}; i < i_; ++i) {
        Vector p(vertices[i]);
        Vector n(normals[i]);

        matrix(0, 0) += n(1) * n(1) + n(2) * n(2);
        matrix(0, 1) += -n(0) * n(1);
        matrix(0, 2) += -n(0) * n(2);
        matrix(1, 0) += -n(0) * n(1);
        matrix(1, 1) += n(0) * n(0) + n(2) * n(2);
        matrix(1, 2) += -n(1) * n(2);
        matrix(2, 0) += -n(0) * n(2);
        matrix(2, 1) += -n(1) * n(2);
        matrix(2, 2) += n(0) * n(0) + n(1) * n(1);

        vector(0) += p(0) * (n(1) * n(1) + n(2) * n(2)) -
                     n(0) * (p(1) * n(1) + p(2) * n(2));
        vector(1) += p(1) * (n(0) * n(0) + n(2) * n(2)) -
                     n(1) * (p(0) * n(0) + p(2) * n(2));
        vector(2) += p(2) * (n(0) * n(0) + n(1) * n(1)) -
                     n(2) * (p(0) * n(0) + p(1) * n(1));
    }

    return matrix.householderQr().solve(vector);
}

Cell::CylinderPtr detectCylinder(CCLib::ReferenceCloud const &referenceCloud,
                                 std::vector<Eigen::Vector3f> const &vertices,
                                 std::vector<Eigen::Vector3f> const &normals) {
    typedef Eigen::Vector3f Vector;
    typedef Eigen::Matrix3f Matrix;

    Vector axis{mostOrthogonalVectorToNVectors(normals)};
    Vector point{closestPointToNLines(vertices, normals)};

    float radius(0), length(0);

    for (unsigned long i{0}, i_{vertices.size()}; i < i_; ++i) {
        radius += ((point - vertices[i]).cross(axis)).norm();

        Vector tmpVertex{Vector::Zero()};
        tmpVertex = (point - vertices[i]);
        float angle(std::acos(normals[i].dot(tmpVertex / tmpVertex.norm())));
        tmpVertex += tmpVertex * std::sin(angle);

        float tmpLength((point - tmpVertex).norm());
        if (tmpLength > length) length = tmpLength;
    }

    radius /= vertices.size();
    length *= 2.;

    Eigen::Quaternionf rotation{
        Eigen::Quaternionf::FromTwoVectors(Vector::UnitZ(), axis)};

    Eigen::Transform<float, 3, Eigen::Affine> transform{
        rotation.toRotationMatrix()};
    transform.translate(point);

    ccGLMatrix glMatrix;
    for (unsigned int i{0}; i < 16; ++i)
        glMatrix.data()[i] = transform.data()[i];

    return std::make_shared<ccCylinder>(radius, length, &glMatrix);
}

Cell::Cell(unsigned int index, unsigned int level, float maxDist,
           float maxAngle)
    : index_{index},
      level_{level},
      maxDist_{maxDist},
      maxAngle_{maxAngle},
      planePtr_{nullptr} {
    assert(maxDist_ > 0);
    assert(maxAngle_ > 0);
}

void Cell::operator()(CCLib::ReferenceCloud const &referenceCloud,
                      std::vector<Eigen::Vector3f> const &vertices,
                      std::vector<Eigen::Vector3f> const &normals, bool plane,
                      bool cylinder) {
    if (plane) {
        this->planePtr_ = detectPlane(referenceCloud);
    }

    if (cylinder) {
        this->cylinderPtr_ = detectCylinder(referenceCloud, vertices, normals);
    }
}
