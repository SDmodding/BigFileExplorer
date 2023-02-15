#pragma once

struct BigFileIndex_t
{
	uint32_t Magic;
	STRUCT_PAD(0x24);
	uint32_t NameHash;
	STRUCT_PAD(0x14);
	uint32_t TypeUID;
	char Name[0x24];
	STRUCT_PAD(0x4);
	uint32_t SortKey;
	uint32_t EntryCount;
	STRUCT_PAD(0x14);
	uint16_t State;
	uint16_t MountIndex;
	STRUCT_PAD(0x8);
	char FileName[0x14];

	bool IsValid() { return (Magic == 0x2C5C40A8); }
};

struct BigFileIndexEntry_t
{
	uint32_t Hash;
	uint32_t FileOffsetDiv4;
	uint32_t LoadOffset;
	uint32_t CompressedSize;
	uint32_t CompressedExtra; 
	uint32_t UncompressedSize;

	uint32_t GetFileOffset() { return ((FileOffsetDiv4 * 0x4) + (LoadOffset & 0xFFF)); }

	// This is used to check if entry should exist in actual .big file
	bool IsValid() { return (CompressedSize > 0 && GetFileOffset() > 0); }
};

struct BixFile_t : BigFileIndex_t
{
	BigFileIndexEntry_t* GetEntry(uint32_t m_Index)
	{
		uintptr_t m_This = reinterpret_cast<uintptr_t>(this);
		return reinterpret_cast<BigFileIndexEntry_t*>(m_This + sizeof(BigFileIndex_t) + (sizeof(BigFileIndexEntry_t) * m_Index));
	}

	BigFileIndexEntry_t* FindEntry(uint32_t m_Hash)
	{
		for (uint32_t i = 0; EntryCount > i; ++i)
		{
			BigFileIndexEntry_t* m_Entry = GetEntry(i);
			if (m_Entry->Hash == m_Hash)
				return m_Entry;
		}

		return nullptr;
	}

	uint32_t GetMaxFileOffset()
	{
		uint32_t m_MaxFileOffset = 0;
		for (uint32_t i = 0; EntryCount > i; ++i)
		{
			BigFileIndexEntry_t* m_Entry = GetEntry(i);
			uint32_t m_FileOffset = m_Entry->GetFileOffset();
			if (m_Entry->IsValid() && m_FileOffset > m_MaxFileOffset)
				m_MaxFileOffset = m_FileOffset;
		}

		return m_MaxFileOffset;
	}

	uint32_t GetValidEntryCount()
	{
		uint32_t m_Count = 0;
		for (uint32_t i = 0; EntryCount > i; ++i)
		{
			if (GetEntry(i)->IsValid())
				++m_Count;
		}

		return m_Count;
	}
};