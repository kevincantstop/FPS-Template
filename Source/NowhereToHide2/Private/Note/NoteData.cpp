#include "Note/NoteData.h"

bool UNoteData::FindEntry(FName InNoteID, FNoteEntry& OutEntry) const
{
	if (const FNoteEntry* Found = Entries.Find(InNoteID))
	{
		OutEntry = *Found;
		return true;
	}
	return false;
}