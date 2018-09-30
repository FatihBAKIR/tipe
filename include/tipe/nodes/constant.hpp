//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

namespace tip::nodes
{
    template <auto Value>
    struct constant
    {
        auto operator()() const
        {
            return Value;
        }
    };
}
