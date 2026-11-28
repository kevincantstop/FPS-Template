// NoteItem.cpp
#include "Interaction/Note/NoteItem.h"

#include "Components/StaticMeshComponent.h"
#include "Note/NoteData.h"

ANoteItem::ANoteItem()
{
	bAllowRotation = false;

	PromptText = FText::FromString(TEXT("[E] Read"));
}

void ANoteItem::BeginPlay()
{
	Super::BeginPlay();

	RefreshFromDatabase();
}

void ANoteItem::RefreshFromDatabase()
{
	if (!NoteDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Note] %s: NoteDatabase is not set"), *GetName());
		return;
	}

	const FNoteEntry* Entry = NoteDatabase->Entries.Find(NoteID);
	if (!Entry)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Note] %s: entry '%s' not found"), *GetName(), *NoteID.ToString());
		return;
	}

	CachedTitle    = Entry->Title;
	CachedSubtitle = Entry->Subtitle;
	CachedBody     = Entry->Body;
	
	if (Entry->Mesh && ItemMesh)
	{
		ItemMesh->SetStaticMesh(Entry->Mesh);
	}
}