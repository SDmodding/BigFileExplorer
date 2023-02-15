#pragma once

struct BigFileEntry_t
{
	uint32_t Magic;
	uint16_t Type;
	uint16_t Version;
	uint32_t DataOffset;
	uint32_t InPlaceExtraNumBytes;
	uint64_t CompressedNumBytes;
	uint64_t UncompressedNumBytes;
	uint64_t UncompressedChecksum;

	bool IsValid() 
	{ 
		return (Magic == 0x51434D50); // PMCQ
	}

	BigFileEntry_t* Set(uint8_t* m_Buffer, uint64_t m_Length)
	{
		return nullptr;
	}

	bool Get(uint8_t** m_Buffer)
	{
		if (!IsValid())
			return false;

		*m_Buffer = reinterpret_cast<uint8_t*>(malloc(UncompressedNumBytes));

        uint8_t* m_CompressedBytes      = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + DataOffset);
        uint8_t* m_UncompressedBytes    = *m_Buffer;

		uint64_t m_CompressedOffset = 0;
		uint64_t m_UncompressedOffset = 0;

		uint16_t m_Lengths[32];
		uint16_t m_Offsets[32];

		uint64_t m_ZOffset = 0;

		while (UncompressedNumBytes > m_UncompressedOffset)
		{
			uint8_t m_CompressedByte = m_CompressedBytes[m_CompressedOffset++];
			if (m_CompressedByte >= 0x20)
			{
				uint32_t m_Mode		= (m_CompressedByte >> 5) & 0x07;
				uint32_t m_Index	= (m_CompressedByte >> 0) & 0x1F;

				uint16_t m_Length = 0;
				uint16_t m_Offset = 0;
				if (m_Mode == 1)
				{
					m_Length = m_Lengths[m_Index];
					m_Offset = m_Offsets[m_Index];
				}
				else
				{
					m_Offset = (uint16_t)(m_CompressedBytes[m_CompressedOffset++] | (m_Index << 8));
					m_Length = (uint16_t)((m_Mode == 7 ? m_CompressedBytes[m_CompressedOffset++] : m_Mode) + 1);

					m_Offsets[m_ZOffset] = m_Offset;
					m_Lengths[m_ZOffset] = m_Length;
					m_ZOffset = (m_ZOffset + 1) % 32;
				}

				for (uint64_t i = 0, j = m_UncompressedOffset - m_Offset; i < m_Length && m_UncompressedOffset < UncompressedNumBytes; i++, j++)
				{
					m_UncompressedBytes[m_UncompressedOffset] = m_UncompressedBytes[j];
					m_UncompressedOffset++;
				}
			}
			else
			{
				uint64_t m_Length = min(static_cast<uint64_t>(m_CompressedByte) + 1, UncompressedNumBytes - m_UncompressedOffset);
				memcpy(&m_UncompressedBytes[m_UncompressedOffset], &m_CompressedBytes[m_CompressedOffset], m_Length);
				m_CompressedOffset		+= m_Length;
				m_UncompressedOffset	+= m_Length;
			}
		}

		return true;
	}
};

struct BigFile_t
{
	BigFileEntry_t* GetEntry(uint32_t m_Offset)
	{
		uintptr_t m_This = reinterpret_cast<uintptr_t>(this);
		return reinterpret_cast<BigFileEntry_t*>(m_This + m_Offset);
	}
};