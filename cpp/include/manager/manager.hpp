#pragma once
#include <algorithm>
#include <cstddef>
#include <ranges>
#include <tuple>
#include <type_traits>

#include "simd/traits.hpp"
#include "utils/compositions.hpp"
#include "utils/lambda_traits.hpp"
#include "utils/packs.hpp"

namespace compecs {
template <typename T>
concept TableQueryable = requires(T t) {
    T::RelevantTablesComponents;
    t.getTable();
};

template <typename T>
concept HasComponentTypePack = requires { typename std::remove_cvref_t<T>::ComponentTypePack; };

template <typename T, typename Q>
class Manager {
    static_assert("Improper instantiation of manager");
};

template <typename... Ts, typename... Qs>
    requires((HasComponentTypePack<Ts> && ...))
class Manager<TypePack<Ts...>, TypePack<Qs...>> {
   public:
    /**
     * @brief Goes through every table and executes queries
     */
    void execute() {
        // Should do some level of parallelization figure that out later
        std::apply([&](auto&... table) { (execute_table(table), ...); }, tables);
    }

   private:
    /**
     * @brief For a given table execute relevant queries on it
     *
     */
    template <typename T>
        requires HasComponentTypePack<T>
    void execute_table(T& table) {
        // Executes query for range between start and non inclusive end
        auto execute_query = []<typename Table, typename Query>(Table& table, Query query,
                                                                size_t start, size_t end) {
            if constexpr (BoundedPacks<typename FunctionInfo<Query>::DecayedArgPack,
                                       typename Table::ComponentTypePack>) {
                // execute the function else no-op
                auto zipped = table.template columns<FunctionInfo<Query>::DecayedArgPack>() |
                              std::views::drop(start) | std::views::take(end - start);

                std::ranges::for_each(zipped, [&](auto&& tuple) {
                    std::apply(query, std::forward<decltype(tuple)>(tuple));
                });
            }
        };

        size_t num_elements{table.size()};
        for (size_t i{}; i < num_elements - 16; i += 16) {
            (execute_query(table, Qs{}, i, i + 16), ...);
        }
    }

    std::tuple<Ts...> tables;
};

}  // namespace compecs