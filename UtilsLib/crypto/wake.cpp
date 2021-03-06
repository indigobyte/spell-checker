// wake.cpp - written and placed in the public domain by Wei Dai

#include "pch.h"
#include "wake.h"

NAMESPACE_BEGIN(CryptoPP)

void WAKE_TestInstantiations()
{
	WAKE_CFB<>::Encryption x1;
	WAKE_CFB<>::Decryption x3;
	WAKE_OFB<>::Encryption x2;
	WAKE_OFB<>::Decryption x4;
}

inline word32 WAKE_Base::M(word32 x, word32 y)
{
	word32 w = x+y;
	return (w>>8) ^ t[(byte)w];
}

void WAKE_Base::GenKey(word32 k0, word32 k1, word32 k2, word32 k3)
{
	long x, z;
	int p ;
	static unsigned long tt[10]= {
		0x726a8f3bUL,								 // table
		0xe69a3b5cUL,
		0xd3c71fe5UL,
		0xab3c73d2UL,
		0x4d3a8eb3UL,
		0x0396d6e8UL,
		0x3d4c2f7aUL,
		0x9ee27cf3UL, } ;
	t[0] = k0;
	t[1] = k1;
	t[2] = k2;
	t[3] = k3;
	for (p=4 ; p<256 ; p++)
	{
	  x=t[p-4]+t[p-1] ; 					   // fill t
	  t[p]= (x>>3) ^ tt[byte(x&7)] ;
	}

	for (p=0 ; p<23 ; p++)
		t[p]+=t[p+89] ; 		  // mix first entries
	x=t[33] ; z=t[59] | 0x01000001L ;
	z=z&0xff7fffffL ;
	for (p=0 ; p<256 ; p++) {		//change top byte to
	  x=(x&0xff7fffffL)+z ; 		 // a permutation etc
	  t[p]=(t[p] & 0x00ffffffL) ^ x ; }

	t[256]=t[0] ;
	byte y=byte(x);
	for (p=0 ; p<256 ; p++) {	  // further change perm.
	  t[p]=t[y=byte(t[p^y]^y)] ;  // and other digits
	  t[y]=t[p+1] ;  }
}

template <class B>
void WAKE_Policy<B>::CipherSetKey(const NameValuePairs &params, const byte *key, size_t length)
{
	word32 k0, k1, k2, k3;
	BlockGetAndPut<word32, BigEndian, false>::Get(key)(r3)(r4)(r5)(r6)(k0)(k1)(k2)(k3);
	GenKey(k0, k1, k2, k3);
}

// CFB
template <class B>
void WAKE_Policy<B>::Iterate(byte *output, const byte *input, CipherDir dir, size_t iterationCount)
{
	RegisterOutput<B> registerOutput(output, input, dir);

	while (iterationCount--)
	{
		r3 = M(r3, ConditionalByteReverse(B::ToEnum(), r6));
		r4 = M(r4, r3);
		r5 = M(r5, r4);
		r6 = M(r6, r5);
		registerOutput(r6);
	}
}

// OFB
template <class B>
void WAKE_Policy<B>::OperateKeystream(KeystreamOperation operation, byte *output, const byte *input, size_t iterationCount)
{
	KeystreamOutput<B> keystreamOperation(operation, output, input);

	while (iterationCount--)
	{
		keystreamOperation(r6);
		r3 = M(r3, r6);
		r4 = M(r4, r3);
		r5 = M(r5, r4);
		r6 = M(r6, r5);
	}
}
/*
template <class B>
void WAKE_ROFB_Policy<B>::Iterate(KeystreamOperation operation, byte *output, const byte *input, unsigned int iterationCount)
{
	KeystreamOutput<B> keystreamOperation(operation, output, input);

	while (iterationCount--)
	{
		keystreamOperation(r6);
		r3 = M(r3, r6);
		r4 = M(r4, r3);
		r5 = M(r5, r4);
		r6 = M(r6, r5);
	}
}
*/
template class WAKE_Policy<BigEndian>;
template class WAKE_Policy<LittleEndian>;
//template class WAKE_ROFB_Policy<BigEndian>;
//template class WAKE_ROFB_Policy<LittleEndian>;

NAMESPACE_END
