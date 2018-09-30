//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#include <ostream>

#pragma once

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
            *m_osp << t << m_sep;
        }

    private:
        mutable std::ostream* m_osp;
        std::string_view m_sep;
    };
}