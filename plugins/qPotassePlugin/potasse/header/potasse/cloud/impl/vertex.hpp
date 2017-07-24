namespace potasse
{
  namespace cloud
  {
    template <std::size_t S>
      Vertex<S>::Vertex(Base const &base)
      : Base{base}
    {
    }

    template <std::size_t S>
      typename Vertex<S>::pointer
      Vertex<S>::vertex()
      {
        return Base::data();
      }

    template <std::size_t S>
      typename Vertex<S>::const_pointer
      Vertex<S>::vertex() const
      {
        return Base::data();
      }
  }
}
