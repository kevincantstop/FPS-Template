#include "UI/DiaryData.h"

bool UDiaryData::FindEntry(FName DiaryID, FDiaryEntry& OutEntry) const
{
	if (const FDiaryEntry* Found = Entries.Find(DiaryID))
	{
		OutEntry = *Found;
		return true;
	}
	return false;
}