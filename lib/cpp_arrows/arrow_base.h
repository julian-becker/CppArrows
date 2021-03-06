//
// Created by Yar Sh.
// Copyright (c) 2017 Yar Sh.
// MIT License. See LICENCE for more details.
//

#ifndef CPPARROWS_ARROW_BASE_H
#define CPPARROWS_ARROW_BASE_H

#include <arrow_base/make_arrow_base.h>
#include <utility>

namespace cpp_arrows
{
    /** `arrow_base` defines basic methods for doing arrow composition and
     *  implementation specifics. Generally you should use extended `cpp_arrows::arrow`.
     *  @see `arrow`
     */
    template<typename In, class F, typename Out>
    class arrow_base
    {
    public:
        arrow_base(const F& f) : f{f} {}
        arrow_base(const arrow_base<In, F, Out>& a) : f{a.f} {}

        /** Apply this arrow to its argument.
          * @param in The input value (of type `In`) passed to the
          * computation `f` represented by a callable (function,
          * lambda or class method)
          * @return The result (of type `Out`) of application of the
          * computation `f` to the input value.
          */
        auto operator()(In in) const
        {
            return f(in);
        }

        /** Compose this arrow with a new arrow.
          * @tparam Arrow_next Inferred type of the following arrow to be applied
          * after the current one.
          * @param a_n The following arrow to be applied
          * after the current one.
          * @return A new arrow composed of the current arrow and the following arrow,
          * which represents sequential application of this arrow, and then the following
          * arrow to the input.
          */
        template<class Arrow_next>
        auto operator>>(const Arrow_next& a_n) const
        {
            auto f_composed = [this, &a_n](In in)
            {
                return a_n(apply(in));
            };
            return make_arrow_base<In>(f_composed);
        }

        /** Convert this arrow into a new arrow that accepts a pair of values.
          * This arrow is applied to the first value in the pair, the second
          * value remains unchanged.
          * @tparam In_bypass The type of the second value which is bypassed.
          * @return a pair of values with the first value of the returned pair
          * being the result of the computation and the second value left
          * unchanged.
          */
        template <typename In_bypass>
        auto fst() const
        {
            auto f_of_fst = [this](std::pair<In, In_bypass> in)
            {
                auto computation_res = apply(in.first);
                return std::make_pair(computation_res, in.second);
            };
            return make_arrow_base<std::pair<In, In_bypass> >(f_of_fst);
        }

        /** Works the same as `fst<In_bypass>()` with the type of the bypassed
          * second value set to the arrow input type In
          */
        auto fst() const
        {
            return fst<In>();
        }

        /** The same as fst(), but the computation is applied to the second value
          * in the pair while the first value remains unchanged.
          * @tparam In_bypass The type of the first value which is bypassed.
          * @return a pair of values with the second value of the returned pair
          * being the result of the computation and the first value left
          * unchanged.
          */
        template <typename In_bypass>
        auto snd() const
        {
            auto f_of_snd = [this](std::pair<In_bypass, In> in)
            {
                auto computation_res = apply(in.second);
                return std::make_pair(in.first, computation_res);
            };
            return make_arrow_base<std::pair<In_bypass, In> >(f_of_snd);
        }

        /** Works the same as `snd<In_bypass>()` with the type of the bypassed
          * first value set to the arrow input type In
          */
        auto snd() const
        {
            return snd<In>();
        }

        /** Convinience synonym for `operator()`
          * @see `operator()`
          */
        auto apply(In in) const
        {
            return this->operator()(in);
        }

    private:
        F f;

    public:
        typedef In  In_type;
        typedef Out Out_type;
    };
}

#endif //CPPARROWS_ARROW_BASE_H
