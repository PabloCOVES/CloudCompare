namespace potasse
{
  namespace cloud
  {
    template <std::size_t S>
      Normal<S>::Normal(Base const &base)
      : Base{base}
    {
    }

    template <std::size_t S>
      typename Normal<S>::pointer
      Normal<S>::normal()
      {
        return Base::data();
      }

    template <std::size_t S>
      typename Normal<S>::const_pointer
      Normal<S>::normal() const
      {
        return Base::data();
      }
  }
}
