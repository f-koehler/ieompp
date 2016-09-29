
Algebra
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
    It is only present in instatiations with more then three indices.

    .. cpp:function:: bool operator==(const Operator& rhs) const

    This comparison operator will return *true* if :cpp:var:`indices` and :cpp:var:`creator` are the same in *this* operator and *rhs*.
    Otherwise *false* will be returned.

    .. cpp:function:: bool operator!=(const Operator& rhs) const

    This comparison operator will return *true* if :cpp:var:`indices` and/or :cpp:var:`creator` differ in *this* operator and *rhs*.
    Otherwise *false* will be returned.


.. cpp:class:: template <typename Index> Operator<Index>

    This class is a specialization of the template :cpp:class:`Operator\<IndexTs...>` with one index.

    .. cpp:type:: Index

        Type of the index.

    .. cpp:member:: Index index

        Index of the operator.


.. cpp:class:: template <typename Index1, typename Index2> Operator<Index1, Index2>

    This class is a specialization of the template :cpp:class:`Operator\<IndexTs...>` with two indices.

    .. cpp:type:: Index1

        Type of the index one.

    .. cpp:type:: Index2

        Type of the index two.

    .. cpp:member:: Index1 index1

        First index of the operator.

    .. cpp:member:: Index2 index2

        Second index of the operator.


.. cpp:function:: template <typename Index> make_creator(Index&& index)

        Creates a creation operator with one index.

.. cpp:function:: template <typename Index1, typename Index2> make_creator(Index1&& index1, Index2&& index2)

        Creates a creation operator with two indices.

.. cpp:function:: template <typename... Indices> make_creator(Indices&&... indices)

        Creates a creation operator with more then two indices.

.. cpp:function:: template <typename Index> make_annihilator(Index&& index)

        Creates an annihilation operator with one index.

.. cpp:function:: template <typename Index1, typename Index2> make_annihilator(Index1&& index1, Index2&& index2)

        Creates an annihilation operator with two indices.

.. cpp:function:: template <typename... Indices> make_annihilation(Indices&&... indices)

        Creates an annihilation operator with more then two indices.

.. cpp:type:: template <std::size_t I, typename Operator> using index_type

    This type represents the index type of index *I* of the operator type that was passed.

.. cpp:function:: constexpr template <std::size_t I, typename Operator> \
    typename index_type<I, Operator>::type get_index(Operator& op)

.. cpp:function:: template <Operator> bool anticommutates(const Operator& a, const Operator& b)

    This function returns *true* if the anticommutator of the operators *a* and *b* vanishes.
    Due to :math:`\lbrace c_a^†, c_b^†\rbrace=\lbrace c_a, c_b\rbrace=0` and :math:`\lbrace c_a, c_b^†\rbrace=δ_{a,b}` this is the case if the operators share the same creator flag or if any index is different.
