#pragma once

#include <memory>
#include "encryptionparams.h"
#include "util/modulus.h"
#include "util/polymodulus.h"
#include "bigpolyarray.h"
#include "util/ntt.h"

namespace seal
{
    /**
    Encrypts BigPoly objects into BigPolyArray objects.
    Constructing an Encryptor requires the encryption parameters (set through an EncryptionParameters object) and
    an BigPolyArray. The private and evaluation keys are not needed for encryption.
    */
    class Encryptor
    {
    public:
        /**
        Creates an Encryptor instances initialized with the specified encryption parameters and public key.

        @param[in] parms The encryption parameters
        @param[in] public_key The public key
        @throws std::invalid_argument if encryption parameters or public key are not valid
        @see EncryptionParameters for more details on valid encryption parameters.
        */
        Encryptor(const EncryptionParameters &parms, const BigPolyArray &public_key);

        /**
        Encrypts a plaintext and stores the result in the destination parameter. The destination parameter is resized if
        and only if its coefficient count or coefficient bit count does not match the encryption parameters. The plaintext polynomial
        must have a significant coefficient count smaller than the coefficient count specified by the encryption parameters, and with
        coefficient values less-than the plain modulus (EncryptionParameters::plain_modulus()).

        @warning encrypt() is not thread safe.
        @param[in] plain The plaintext to encrypt
        @param[out] destination The ciphertext to overwrite with the encrypted plaintext
        @throws std::invalid_argument if the plaintext polynomial's significant coefficient count or coefficient values are too large to
        represent with the encryption parameters
        @throws std::logic_error If destination is an alias but needs to be resized
        @see EncryptionParameters for more details on plain text limits for encryption parameters.
        */
        void encrypt(const BigPoly &plain, BigPolyArray &destination);

        /**
        Encrypts a plaintext and returns the result. The destination parameter is resized if
        and only if its coefficient count or coefficient bit count does not match the encryption parameters. The plaintext polynomial
        must have a significant coefficient count smaller than the coefficient count specified by the encryption parameters, and with
        coefficient values less-than the plain modulus (EncryptionParameters::plain_modulus()).

        @warning encrypt() is not thread safe.
        @param[in] plain The plaintext to encrypt
        @throws std::invalid_argument if the plaintext polynomial's significant coefficient count or coefficient values are too large to
        represent with the encryption parameters
        @see EncryptionParameters for more details on plaintext limits for encryption parameters.
        */
        BigPolyArray encrypt(const BigPoly &plain)
        {
            BigPolyArray result;
            encrypt(plain, result);
            return result;
        }

        /**
        Returns the public key used by the Encryptor.
        */
        const BigPolyArray &public_key() const
        {
            return public_key_;
        }

    private:
        Encryptor(const Encryptor &copy) = delete;
        
        Encryptor &operator =(const Encryptor &assign) = delete;

        void preencrypt(const std::uint64_t *plain, int plain_coeff_count, int plain_coeff_uint64_count, std::uint64_t *destination);
        
        void set_poly_coeffs_normal(std::uint64_t *poly, UniformRandomGenerator *random) const;
        
        void set_poly_coeffs_zero_one_negone(uint64_t *poly, UniformRandomGenerator *random) const;

        void set_poly_coeffs_zero_one(uint64_t *poly, UniformRandomGenerator *random) const;

        BigPoly poly_modulus_;
        
        BigUInt coeff_modulus_;
        
        BigUInt plain_modulus_;
        
        BigUInt upper_half_threshold_;
        
        BigUInt upper_half_increment_;
        
        BigUInt coeff_div_plain_modulus_;
        
        BigPolyArray public_key_;
        
        double noise_standard_deviation_;
        
        double noise_max_deviation_;

        UniformRandomGeneratorFactory *random_generator_;
        
        util::PolyModulus polymod_;
        
        util::Modulus mod_;

        util::NTTTables ntt_tables_; 

        EncryptionParameterQualifiers qualifiers_;
    };

}