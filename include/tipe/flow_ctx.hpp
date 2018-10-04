//
// Created by fatih on 10/3/18.
//

#pragma once

namespace tip
{
    class base_ctx {};

    template <class T>
    class ctx : public base_ctx {};

    class empty_ctx : ctx<empty_ctx> {};

    template <class CtxT>
    class flow_data
    {
        CtxT ctx;
    };
}