//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <ostream>
#include <tos/span.hpp>

namespace tip::nodes
{
    struct print
    {
    public:
        constexpr print(std::ostream& os, std::string_view sep)
            : m_osp{&os}, m_sep{sep} {
        }

        template <class T>
        void operator()(T&& t) const
        {
            *m_osp << std::forward<T>(t) << m_sep;
        }

        void operator()(tos::span<const char> s) const
        {
            for (auto c : s)
            {
                *m_osp << c;
            }
            *m_osp << m_sep;
        }

    private:
        mutable std::ostream* m_osp;
        std::string_view m_sep;
    };
}