//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#include <utility>

namespace tip::nodes
{
    template <class>
    struct many_t {};

    template <class T>
    struct is_many : std::false_type {};

    template <class T>
    struct is_many<many_t<T>> : std::true_type {};

    struct split
    {
        template <class ContainerT, class NextT>
        many_t<ContainerT> operator()(NextT&& next, ContainerT&& x) const
        {
            for (auto& elem : x)
            {
                std::forward<NextT>(next)(elem);
            }
            return {};
        }
    };
}
