#ifndef _POTASSE_DETECTOR_GRAPH_HPP_
#define _POTASSE_DETECTOR_GRAPH_HPP_

#include <potasse/cloud/boundingBox.hpp>

#include <array>
#include <list>
#include <memory>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud>
    class Graph
    {
     public:
      class Parameter;
      class Vertex;

      typedef std::list<Vertex> VertexList;
      typedef std::list<Vertex *> VertexPtrList;

      explicit Graph(Parameter const &parameter = Parameter());

      explicit operator bool() const;

      void compute(PointCloud const &pointCloud);

      Parameter &parameter();
      Parameter const &parameter() const;
      void parameter(Parameter const &parameter);

      VertexList &vertexList();
      VertexList const &vertexList() const;
      void vertexList(VertexList const &vertexList);

     protected:
     private:
      Parameter parameter_;
      VertexList vertexList_;

     public:
      class Parameter
      {
       public:
        typedef std::array<
            typename PointCloud::value_type::value_type,
            std::tuple_size<typename PointCloud::value_type>::value>
            Dimensions;
        typedef std::size_t Cardinal;

        explicit Parameter(Dimensions const &dimensions = {0}, Cardinal const &cardinal = 0);

        explicit operator bool() const;

        Dimensions &dimensions();
        Dimensions const &dimensions() const;
        void dimensions(Dimensions const &dimensions);

        Cardinal &cardinal();
        Cardinal const &cardinal() const;
        void cardinal(Cardinal const &cardinal);

       protected:
       private:
        Dimensions dimensions_;
        Cardinal cardinal_;
      };

      class Vertex
      {
       public:
        typedef potasse::cloud::BoundingBox<PointCloud> BoundingBox;
        typedef std::list<size_t> Indices;

        explicit Vertex(BoundingBox const &boundingBox, Indices const &indices);

        BoundingBox &boundingBox();
        BoundingBox const &boundingBox() const;
        void boundingBox(BoundingBox const &boundingBox);

        Indices &indices();
        Indices const &indices() const;
        void indices(Indices const &indices);

        VertexPtrList &vertexPtrList();
        VertexPtrList const &vertexPtrList() const;
        void vertexPtrList(VertexPtrList const &vertexPtrList);

       protected:
       private:
        BoundingBox boundingBox_;
        Indices indices_;

        VertexPtrList vertexPtrList_;
      };
    };
  }
}

#endif  //_POTASSE_DETECTOR_GRAPH_HPP_
