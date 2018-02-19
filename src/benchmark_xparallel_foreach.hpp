/***************************************************************************
* Copyright (c) 2016, Johan Mabille, Sylvain Corlay and Wolf Vollprecht    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <benchmark/benchmark.h>


#include <vector>

#include "xtl/xthreadpool.hpp"
#include "xtl/xparallel_foreach.hpp"





namespace xt
{

        inline float hard_func(const float a, const std::size_t i){
            const auto ea = std::exp(a);
            const auto eb = std::exp(ea + 1.0f/i);
            const auto ec = std::exp(1.0/i);
            return std::exp(1.0/std::pow(ea+eb+ec, ec)) + a;
        }


        inline float easy_func(const float a, const std::size_t i){
            return a + i;
        }

        void simple_xthreadpool_parallel_foreach(benchmark::State& state)
        {
            // get ranges
            const auto size = static_cast<std::size_t>(state.range(0));

            // some data
            std::vector<float> a(size, 2.0f);

            // setup threadpool
            xtl::xthreadpool pool(xtl::n_thread_settings::default_n_threads);

            while (state.KeepRunning())
            {
                xtl::xparallel_foreach(size, pool, [&](auto worker_index, auto i)
                {
                    a[i] = hard_func(a[i], i);
                });
            }
        }



        void simple_openmp_parallel_foreach(benchmark::State& state)
        {
            // get ranges
            const auto size = static_cast<std::size_t>(state.range(0));
  

            // some data
            std::vector<float> a(size, 2.0f);

            // setup threadpool

            while (state.KeepRunning())
            {   
                #pragma omp parallel for
                for(std::size_t i=0; i<size; ++i)
                {
                    a[i] = hard_func(a[i], i);
                };
            }
        }


        void simple_seriell_foreach(benchmark::State& state)
        {
            // get ranges
            const auto size = static_cast<std::size_t>(state.range(0));
            //const auto n_workers = static_cast<std::size_t>(state.range(1));

            // some data
            std::vector<float> a(size, 2.0f);

            
            while (state.KeepRunning())
            {
                for(std::size_t i=0; i<size; ++i)
                {
                    a[i] = hard_func(a[i], i);
                };
            }
        }



    BENCHMARK(simple_xthreadpool_parallel_foreach)->Range(10000, 1000000);
    BENCHMARK(simple_openmp_parallel_foreach     )->Range(10000, 1000000);
    BENCHMARK(simple_seriell_foreach             )->Range(10000, 1000000);

}
