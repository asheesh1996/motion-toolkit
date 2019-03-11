/*  MoTo - Motion Toolkit
    Copyright (c) 2006-2019 Gino van den Bergen, DTECTA

    Source published under the terms of the MIT License. 
    For details please see COPYING file or visit 
    http://opensource.org/licenses/MIT
*/

#ifndef MT_RANDOM_HPP
#define MT_RANDOM_HPP

#include <complex>

#include <moto/Scalar.hpp>
#include <moto/ScalarTraits.hpp>
#include <moto/Vector2.hpp>
#include <moto/Vector3.hpp>
#include <moto/Vector4.hpp>

namespace mt
{
    template <typename Scalar>
    class Random
    {  
    public:
        Random(uint32_t seed = 12345UL);

        uint32_t rand();
        uint32_t seed() const;
        void setSeed(uint32_t seed);

        Scalar uniform();
        Scalar uniform(Scalar a, Scalar b);
        Scalar angle();
        Vector2<Scalar> uniformVector2();
        Vector2<Scalar> uniformVector2(Scalar a, Scalar b);
        Vector3<Scalar> uniformVector3();
        Vector3<Scalar> uniformVector3(Scalar a, Scalar b);
        Vector3<Scalar> direction();
        Vector4<Scalar> rotation();

    private:
        uint32_t mSeed;
    };


    template<typename Scalar>
    Random<Scalar>::Random(uint32_t seed)
        : mSeed(seed)
    {}


    template<typename Scalar>
    uint32_t Random<Scalar>::rand()
    {
         mSeed = 1664525UL * mSeed + 1013904223UL;
         return mSeed;
    }
    
    template<typename Scalar>
    uint32_t Random<Scalar>::seed() const
    {
         return mSeed;
    }
    
    template<typename Scalar>
    void Random<Scalar>::setSeed(uint32_t seed)
    {
        mSeed = seed;
    }
    
    template <typename Scalar>
    FORCEINLINE
    Scalar Random<Scalar>::uniform()
    { 
        static const Scalar ONE_OVER_MAX_UINT32 = Scalar(1) / Scalar(std::numeric_limits<uint32_t>::max());
        return Scalar(rand()) * ONE_OVER_MAX_UINT32;
    }

#if USE_IEEE_754 

    template <>
    FORCEINLINE
    float Random<float>::uniform()
    {
		uint32_t bits = 0x3f800000UL | (rand() & 0x007fffffUL);
        return bitcast<float>(bits) - 1.0f;
    }

#endif
     
    template <typename Scalar>
    FORCEINLINE
    Scalar Random<Scalar>::uniform(Scalar a, Scalar b)
    {
        return lerp(a, b, uniform());
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar Random<Scalar>::angle()
    {
        return uniform(-ScalarTraits<Scalar>::pi(), ScalarTraits<Scalar>::pi());
    } 

	template <typename Scalar>
    Vector2<Scalar> Random<Scalar>::uniformVector2()
    {
        Scalar x = uniform();
        Scalar y = uniform();
        return Vector2<Scalar>(x, y);
    }

    template <typename Scalar>
    Vector2<Scalar> Random<Scalar>::uniformVector2(Scalar a, Scalar b)
    {
        Scalar x = uniform(a, b);
        Scalar y = uniform(a, b);
        return Vector2<Scalar>(x, y);
    }
    
        
    template <typename Scalar>
    Vector3<Scalar> Random<Scalar>::uniformVector3()
    {
        Scalar x = uniform();
        Scalar y = uniform();
        Scalar z = uniform();
        return Vector3<Scalar>(x, y, z);
    }

    template <typename Scalar>
    Vector3<Scalar> Random<Scalar>::uniformVector3(Scalar a, Scalar b)
    {
        Scalar x = uniform(a, b);
        Scalar y = uniform(a, b);
        Scalar z = uniform(a, b);
        return Vector3<Scalar>(x, y, z);
    }
    
    template <typename Scalar>
    Vector3<Scalar> Random<Scalar>::direction()
    {
        Scalar z = uniform(Scalar(-1), Scalar(1));
        Scalar r = sqrt(Scalar(1) - square(z));
        Scalar t = angle();
        std::complex<Scalar> e = std::polar(r, t);
        return Vector3<Scalar>(e.real(), e.imag(), z);
    }
    
    template <typename Scalar>
    Vector4<Scalar> Random<Scalar>::rotation()
    {
        // From: "Uniform Random Rotations", Ken Shoemake, Graphics Gems III, 
        //       pg. 124-132
        Scalar x0 = uniform();
        Scalar r1 = sqrt(Scalar(1) - x0);
        Scalar r2 = sqrt(x0);
        Scalar t1 = angle();
        Scalar t2 = angle();
        std::complex<Scalar> c1 = std::polar(r1, t1);
        std::complex<Scalar> c2 = std::polar(r2, t2);
        return Vector4<Scalar>(c1.imag(), c1.real(), c2.imag(), c2.real());
    }
}

#endif
