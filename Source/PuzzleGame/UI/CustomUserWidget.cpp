#include "PuzzleGame/UI/CustomUserWidget.h"

void UCustomUserWidget::SetText(const FText& text)
{
	if (TextBlock)
	{
		TextBlock->SetText(text);
	}
}

