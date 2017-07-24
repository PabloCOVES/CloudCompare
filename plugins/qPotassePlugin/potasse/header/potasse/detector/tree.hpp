#ifndef _POTASSE_DETECTOR_TREE_HPP_
#define _POTASSE_DETECTOR_TREE_HPP_

#include <potasse/cloud/boundingBox.hpp>

#include <array>
#include <list>
#include <memory>

namespace potasse
{
  namespace detector
  {
    template <typename PointCloud>
    class Tree
    {
     public:
      class Parameter;
      class Node;
      class Branch;
      class Leaf;

      typedef cloud::boundingBox<PointCloud> boundingBox;
      typedef typename PointCloud::value_type Point;
      typedef typename Point::value_type Scalar;

      typedef std::list<std::size_t> Indices;
      typedef std::shared_ptr<Node> NodePtr;

      explicit Tree(Parameter const &parameter = Parameter());

      explicit operator bool() const;

      void compute(PointCloud const &pointCloud,
                   Indices const &indices,
                   boundingBox const &boundingBox);

      Scalar volume() const;

      Parameter &parameter();
      Parameter const &parameter() const;
      void parameter(Parameter const &parameter);

     protected:
     private:
      Parameter parameter_;
      NodePtr root_;

     public:
      class Parameter
      {
       public:
        typedef std::array<Scalar, std::tuple_size<Point>::value> Dimensions;
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

      class Node
      {
       public:
        static NodePtr create(PointCloud const &pointCloud, Parameter const &parameter, Indices const &indices, boundingBox const &boundingBox);

        virtual Scalar volume() const = 0;

        Node *parent();
        Node const *parent() const;
        void parent(Node *parent);

        boundingBox &boundingBox();
        boundingBox const &boundingBox() const;
        void boundingBox(boundingBox const &boundingBox);

       protected:
        Node(Node *parent, boundingBox const &boundingBox);

       private:
        Node *parent_;
        boundingBox boundingBox_;
      };

      class Branch : public Node
      {
       public:
        typedef std::array<NodePtr, 1 << std::tuple_size<typename PointCloud::value_type>::value> NodePtrArray;

        explicit Branch(Node *parent, PointCloud const &pointCloud, Parameter const &parameter, Indices indices, boundingBox const &boundingBox);
        explicit Branch(PointCloud const &pointCloud, Parameter const &parameter, Indices const &indices, boundingBox const &boundingBox);

        Scalar volume() const;

        NodePtrArray &nodePtrArray();
        NodePtrArray const &nodePtrArray() const;
        void nodePtrArray(NodePtrArray const &nodePtrArray);

       protected:
       private:
        NodePtrArray nodePrtArray_;
      };

      class Leaf : public Node
      {
       public:
        explicit Leaf(Node *parent, Indices const &indices, boundingBox const &boundingBox);
        explicit Leaf(Indices const &indices, boundingBox const &boundingBox);

        Scalar volume() const;

        Indices &indices();
        Indices const &indices() const;
        void indices(Indices const &indices);

       protected:
       private:
        Indices indices_;
      };
    };
  }
}

#endif  //_POTASSE_DETECTOR_TREE_HPP_
