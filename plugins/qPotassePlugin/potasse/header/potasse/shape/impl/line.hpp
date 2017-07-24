namespace potasse
{
  namespace shape
  {
    template <std::size_t S>
      Line<S>::Line(Vertex const &vertex, Normal const &normal)
      : Base{Base::Type::Line, vertex, normal}
    {
    }

    template <std::size_t S>
      Line<S>::Line(Vertex const &vertex0, Vertex const &vertex1)
      : Line{vertex0, Shape<S>::normal(vertex0, vertex1)}
    {
    }

    template <std::size_t S>
      SCALAR
      Line<S>::distance(Vertex const &vertex) const
      {
        Vector vector{ConstMap{Base::vertex_.vertex()} - ConstMap{vertex.vertex()}};
        return vector.norm() * std::acos(ConstMap{Base::normal_.normal()}.dot(vector) / vector.norm());
      }

    template <std::size_t S>
      typename Line<S>::Normal
      Line<S>::normal(Vertex const &vertex) const
      {
        ConstMap normalConstMap{Base::normal_.normal()};
        ConstMap vertexConstMap{vertex.vertex()};

        Vector vector{vertexConstMap - ConstMap{Base::vertex_.vertex()}};

        Normal output;

        Map map{output.normal()};
        map = ConstMap{Base::vertex_.vertex()} + vector.dot(normalConstMap) / normalConstMap.dot(normalConstMap) * normalConstMap;
        map = vertexConstMap - map;
        map.normalize();

        return output;
      }
  }
}
