//
// Created by fatih on 10/1/18.
//

#pragma once

#include <cwpack.hpp>
#include <nlohmann/json.hpp>
#include <cwpack.h>

namespace tip::nodes
{

    struct msgpack_to_json
    {
        nlohmann::json
        operator()(tos::span<const char> msgpack)
        {
            cw_unpack_context uc;
            cw_unpack_context_init (&uc, (void*)msgpack.data(), msgpack.size(), nullptr);

            return parse_one(uc);
        }

        nlohmann::json parse_one(cw_unpack_context& uc)
        {
            cw_unpack_next(&uc);
            switch (uc.item.type)
            {
                case CWP_ITEM_NIL:
                    return nullptr;
                case CWP_ITEM_BOOLEAN:
                    return bool{ uc.item.as.boolean };
                case CWP_ITEM_POSITIVE_INTEGER:
                    return uint32_t (uc.item.as.u64);
                case CWP_ITEM_NEGATIVE_INTEGER:
                    return int32_t (uc.item.as.i64);
                case CWP_ITEM_FLOAT:
                    return uc.item.as.real;
                case CWP_ITEM_DOUBLE:
                    return uc.item.as.long_real;
                case CWP_ITEM_STR:
                    return std::string(
                            (const char*)uc.item.as.str.start,
                            (const char*)uc.item.as.str.start + uc.item.as.str.length);
                case CWP_ITEM_BIN:
                    return std::string(
                            (const char*)uc.item.as.str.start,
                            (const char*)uc.item.as.str.start + uc.item.as.str.length);
                case CWP_ITEM_ARRAY:
                {
                    nlohmann::json arr;
                    auto len = uc.item.as.map.size;
                    for (int i = 0; i < len; ++i)
                    {
                        arr.push_back(parse_one(uc));
                    }
                    return arr;
                }
                case CWP_ITEM_MAP:
                {
                    nlohmann::json arr;
                    auto len = uc.item.as.map.size;
                    for (int i = 0; i < len; ++i)
                    {
                        auto key = parse_one(uc);
                        auto val = parse_one(uc);
                        if (!key.is_string())
                        {
                            return nullptr;
                        }
                        arr[(std::string)key] = val;
                    }
                    return arr;
                }

                case CWP_ITEM_EXT:break;
                case CWP_ITEM_MIN_RESERVED_EXT:break;
                case CWP_ITEM_MAX_RESERVED_EXT:break;
                case CWP_ITEM_MIN_USER_EXT:break;
                case CWP_ITEM_MAX_USER_EXT:break;
                case CWP_NOT_AN_ITEM:break;
            }

            return nullptr;
        }
    };
}