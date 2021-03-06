#ifndef CRYPTOPP_ITERHASH_H
#define CRYPTOPP_ITERHASH_H

#include "cryptlib.h"
#include "secblock.h"
#include "misc.h"
#include "simple.h"

NAMESPACE_BEGIN(CryptoPP)

//! exception thrown when trying to hash more data than is allowed by a hash function
class CRYPTOPP_DLL HashInputTooLong : public InvalidDataFormat
{
public:
	explicit HashInputTooLong(const std::string &alg)
		: InvalidDataFormat("IteratedHashBase: input data exceeds maximum allowed by hash function " + alg) {}
};

//! _
template <class T, class BASE>
class CRYPTOPP_NO_VTABLE IteratedHashBase : public BASE
{
public:
	typedef T HashWordType;

	IteratedHashBase() : m_countLo(0), m_countHi(0) {}
	unsigned int BlockSize() const {return (unsigned int)m_data.size() * sizeof(T);}
	unsigned int OptimalBlockSize() const {return BlockSize();}
	unsigned int OptimalDataAlignment() const {return sizeof(T);}
	void Update(const byte *input, size_t length);
	byte * CreateUpdateSpace(size_t &size);
	void Restart();
	void TruncatedFinal(byte *digest, size_t size);

protected:
	void SetBlockSize(unsigned int blockSize) {m_data.resize(blockSize / sizeof(HashWordType));}
	void SetStateSize(unsigned int stateSize) {m_digest.resize(stateSize / sizeof(HashWordType));}

	T GetBitCountHi() const {return (m_countLo >> (8*sizeof(T)-3)) + (m_countHi << 3);}
	T GetBitCountLo() const {return m_countLo << 3;}

	void PadLastBlock(unsigned int lastBlockSize, byte padFirst=0x80)
	{
		unsigned int blockSize = BlockSize();
		unsigned int num = ModPowerOf2(m_countLo, blockSize);
		((byte *)m_data.begin())[num++]=padFirst;
		if (num <= lastBlockSize)
			memset((byte *)m_data.begin()+num, 0, lastBlockSize-num);
		else
		{
			memset((byte *)m_data.begin()+num, 0, blockSize-num);
			HashBlock(m_data);
			memset(m_data, 0, lastBlockSize);
		}
	}
	virtual void Init() =0;

	virtual ByteOrder GetByteOrder() const =0;
	virtual void HashEndianCorrectedBlock(const HashWordType *data) =0;
	virtual size_t HashMultipleBlocks(const T *input, size_t length);
	void HashBlock(const HashWordType *input) {HashMultipleBlocks(input, BlockSize());}

	SecBlock<T> m_data;			// Data buffer
	SecBlock<T> m_digest;		// Message digest

private:
	T m_countLo, m_countHi;
};

//! _
template <class T_HashWordType, class T_Endianness, unsigned int T_BlockSize, class T_Base = HashTransformation>
class CRYPTOPP_NO_VTABLE IteratedHash : public IteratedHashBase<T_HashWordType, T_Base>
{
public:
	typedef T_Endianness ByteOrderClass;
	typedef T_HashWordType HashWordType;

	CRYPTOPP_CONSTANT(BLOCKSIZE = T_BlockSize)
	// BCB2006 workaround: can't use BLOCKSIZE here
	CRYPTOPP_COMPILE_ASSERT((T_BlockSize & (T_BlockSize - 1)) == 0);	// blockSize is a power of 2

	ByteOrder GetByteOrder() const {return T_Endianness::ToEnum();}

	inline static void CorrectEndianess(HashWordType *out, const HashWordType *in, size_t byteCount)
	{
		ConditionalByteReverse(T_Endianness::ToEnum(), out, in, byteCount);
	}

protected:
	IteratedHash() {this->SetBlockSize(T_BlockSize);}
};

//! _
template <class T_HashWordType, class T_Endianness, unsigned int T_BlockSize, unsigned int T_StateSize, class T_Transform, unsigned int T_DigestSize = 0>
class CRYPTOPP_NO_VTABLE IteratedHashWithStaticTransform
	: public ClonableImpl<T_Transform, AlgorithmImpl<IteratedHash<T_HashWordType, T_Endianness, T_BlockSize>, T_Transform> >
{
public:
	CRYPTOPP_CONSTANT(DIGESTSIZE = T_DigestSize ? T_DigestSize : T_StateSize)
	unsigned int DigestSize() const {return DIGESTSIZE;};

protected:
	IteratedHashWithStaticTransform()
	{
		this->SetStateSize(T_StateSize);
		Init();
	}
	void HashEndianCorrectedBlock(const T_HashWordType *data) {T_Transform::Transform(this->m_digest, data);}
	void Init() {T_Transform::InitState(this->m_digest);}
};

NAMESPACE_END

#ifdef CRYPTOPP_MANUALLY_INSTANTIATE_TEMPLATES
#include "iterhash.cpp"
#endif

NAMESPACE_BEGIN(CryptoPP)

#ifdef WORD64_AVAILABLE
CRYPTOPP_DLL_TEMPLATE_CLASS IteratedHashBase<word64, HashTransformation>;
CRYPTOPP_STATIC_TEMPLATE_CLASS IteratedHashBase<word64, MessageAuthenticationCode>;
#endif

CRYPTOPP_DLL_TEMPLATE_CLASS IteratedHashBase<word32, HashTransformation>;
CRYPTOPP_STATIC_TEMPLATE_CLASS IteratedHashBase<word32, MessageAuthenticationCode>;

NAMESPACE_END

#endif
