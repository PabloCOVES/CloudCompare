namespace potasse
{
  namespace shape
  {
    template <std::size_t S>
      Shape<S>::Shape(Type type,
          Vertex const &vertex,
          Normal const &normal)
      : type_{type},
      vertex_{vertex},
      normal_{normal}
    {
    }

    template <std::size_t S>
      SCALAR
      Shape<S>::angle(Vector const &lhs, Vector const &rhs)
      {
        return std::acos(lhs.dot(rhs) / (lhs.norm() * rhs.norm()));
      }

    template <std::size_t S>
      typename Shape<S>::Normal
      Shape<S>::normal(Vertex const &vertex0, Vertex const &vertex1)
      {
        Normal normal{};
        Map map{normal.normal()};
        map = ConstMap{vertex1.vertex()} - ConstMap{vertex0.vertex()};
        map.normalize();
        return normal;
      }

    template <std::size_t S>
      SCALAR
      Shape<S>::angle(Vertex const &vertex, Normal const &normal) const
      {
        return Shape<S>::angle(ConstMap{Shape<S>::normal(vertex).normal()}, ConstMap{normal.normal()});
      }

    template <std::size_t S>
      typename Shape<S>::Vertex
      Shape<S>::project(Vertex const &vertex) const
      {
        Vertex output{vertex};

        Map map{output.vertex()};
        map -= Shape<S>::distance(vertex) * ConstMap{Shape<S>::normal(vertex).normal()};

        return output;
      }

    template <std::size_t S>
      typename Shape<S>::Vertex &
      Shape<S>::vertex()
      {
        return vertex_;
      }

    template <std::size_t S>
      typename Shape<S>::Vertex const &
      Shape<S>::vertex() const
      {
        return vertex_;
      }

    template <std::size_t S>
      typename Shape<S>::Normal &
      Shape<S>::normal()
      {
        return normal_;
      }

    template <std::size_t S>
      typename Shape<S>::Normal const &
      Shape<S>::normal() const
      {
        return normal_;
      }
  }
}
