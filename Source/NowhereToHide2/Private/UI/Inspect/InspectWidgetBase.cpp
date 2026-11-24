#include "UI/Inspect/InspectWidgetBase.h"
#include "Interaction/Inspect/InspectItem.h"

void UInspectWidgetBase::SetInspectItem(AInspectItem* InItem)
{
	InspectItem = InItem;
	OnInspectItemSet(InItem);
}