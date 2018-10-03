//
// Created by Mehmet Fatih BAKIR on 02/10/2018.
//

#pragma once

#include <vector>
#include <stddef.h>
#include <mutex>

namespace tip::nodes
{
    template <class T>
    struct aggregate
    {
        explicit aggregate(size_t sz) : m_needed{sz} {}
        aggregate(aggregate&& a) noexcept : m_elems{std::move(a.m_elems)}, m_needed{a.m_needed}, m_prot{} {}

        template <class NextT>
        void operator()(NextT&& next, T&& t) const
        {
            std::lock_guard<std::mutex> lk{m_prot};

            m_elems.push_back(std::forward<T>(t));

            if (m_elems.size() < m_needed)
            {
                return;
            }

            next(std::move(m_elems));
            m_elems.clear();
        }

    private:
        /**
         * due to const requirement, we have these mutables
         */
        mutable std::vector<T> m_elems;
        mutable std::mutex m_prot;
        size_t m_needed;
    };
}
