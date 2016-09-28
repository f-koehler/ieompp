
algebra
=======

.. cpp:namespace:: ieompp::algebra


.. cpp:class:: template <typename... IndexTs> Operator

    This struct describes a fermionic creation :math:`c^†_{a_1,a_2,\ldots}` or annihilation :math:`c_{a_1,a_2,\ldots}` operator with a variadic set of indices determined by *IndexTs*.

    .. cpp:type:: Indices = std::tuple<IndexTs...>

    Alias for a tuple of the index types *IndexTs*

    .. cpp:member:: bool creator

    If true this operator is a creator and otherwise an annihilator.

    .. cpp:member:: Indices indices

    This tuple contains the values for all indices.

    .. cpp:function:: bool operator==(const Operator& rhs) const

    This comparison operator will return *true* if :cpp:var:`indices` and :cpp:var:`creator` are the same in *this* operator and *rhs*.
    Otherwise *false* will be returned.

    .. cpp:function:: bool operator!=(const Operator& rhs) const

    This comparison operator will return *true* if :cpp:var:`indices` and/or :cpp:var:`creator` differ in *this* operator and *rhs*.
    Otherwise *false* will be returned.



.. cpp:function:: template <Operator> bool anticommutates(const Operator& a, const Operator& b)

    This function returns *true* if the anticommutator of the operators *a* and *b* vanishes.
    Due to :math:`{c_a^†, c_b^†}={c_a, c_b}=0` and `{c_a, c_b^†}=δ_{a,b}` this is the case if the operators share the same creator flag or if any index is different.
