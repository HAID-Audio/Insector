// Copyright Underflow Studios 2017

#include "DialogueUserWidget.h"
#include "UObject/UnrealType.h"
#include "UObject/Class.h"
#include "Internationalization/Regex.h"
#include "Dialogue.h"

bool UDialogueUserWidget::IsConditionsMetForNode_Implementation(FDialogueNode Node)
{
	for (UDialogueConditions* Condition : Node.Conditions)
	{
		if (IsValid(Condition))
		{		
			if (!Condition->IsConditionMet(GetOwningPlayer(), NPCActor))
			{
				return false;
			}
		}
	}
	return true;
}

void UDialogueUserWidget::RunEventsForNode_Implementation(FDialogueNode Node)
{
	for (UDialogueEvents* Event : Node.Events)
	{
		if (IsValid(Event))
		{
			Event->RecieveEventTriggered(GetOwningPlayer(), NPCActor);
		}
	}
}

/* If you supply this function with "charname", it'll run the function called Get_charname
 * It'll also make sure that your Get_charname function has no parameters and only returns a string
 * The resulting string will be returned in &resultString
*/
bool UDialogueUserWidget::RunStringReplacer(FString originalString, FString& resultString)
{
	const FString methodToCall = FString::Printf(TEXT("Get_%s"), *originalString);
	UFunction* Func = GetClass()->FindFunctionByName(FName(*methodToCall), EIncludeSuperFlag::ExcludeSuper);

	if (Func == nullptr) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Function \"%s\" wasn't found on the dialogue widget."), *methodToCall);
		return false;
	}

	int foundReturnStrings = 0;
	for (TFieldIterator<FProperty> It(Func); It; ++It)
	{
		FProperty* Prop = *It;

		// print property flags
		//uint64 flags = (uint64)Prop->GetPropertyFlags();
		//UE_LOG(LogTemp, Log, TEXT("Property Flags: %llu"), flags); // convert to hex: https://www.rapidtables.com/convert/number/decimal-to-hex.html

		// if it's a return type (in blueprints it's an out parameter), check that it's a string
		if (Prop->HasAllPropertyFlags(CPF_Parm | CPF_OutParm))
		{
			if (!Prop->GetCPPType().Equals(TEXT("FString")))
			{
				// if we land here, it means our method returns something other than a string
				UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Your method \"%s\" is returning something other than a string!"), *methodToCall);
				return false;
			}
			else
			{
				foundReturnStrings++;
			}			
		}
		// if it's a normal parameter, return false
		else if (Prop->HasAnyPropertyFlags(CPF_Parm) && !Prop->HasAnyPropertyFlags(CPF_OutParm))
		{
			// we have some parameters, but we shouldn't have them
			UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Your method \"%s\" must have no parameters!"), *methodToCall);
			return false;
		}
	}
	if (foundReturnStrings > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Your method \"%s\" must return only one string!"), *methodToCall);
		return false;
	}
	else if (foundReturnStrings == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Your method \"%s\" doesn't return anything, but must return a string!"), *methodToCall);
		return false;
	}

	FString retvalue;
	ProcessEvent(Func, &retvalue);
	resultString = retvalue;

	return true;
}

/* In the supplied inText, finds all strings of that fit the pattern %string% (a single word between two percentage signs) 
 * and returns an array of them without the percentage sign.
*/
TArray<FString> UDialogueUserWidget::FindVarStrings(FText inText)
{
	TArray<FString> varStrings;

	FString totalText = inText.ToString();

	int firstPercent = -1;
	for (int i = 0; i < totalText.Len(); i++)
	{
		if (totalText[i] == '%')
		{
			if (firstPercent == -1) // if we encounter the first % sign
			{
				firstPercent = i;
			}
			else if (firstPercent + 1 == i) //if we encounter "%%", disregard the first one
			{
				firstPercent = i;
			}
			else // if we encounter second % sign
			{
				FString foundVarString = totalText.Mid(firstPercent + 1, i - firstPercent - 1);
				varStrings.AddUnique(foundVarString);
				firstPercent = -1;
			}
		}
		if (totalText[i] == ' ')
		{
			firstPercent = -1;
		}
	}

	return varStrings;
}

// This function is only useful for the TypeWriter effect.
void UDialogueUserWidget::GetFirstChars(FString originalString, bool isRichText, int32 chars, FString& outputString, bool& finished)
{	
	if (chars >= originalString.Len())
	{
		finished = true;
		outputString = originalString;
		return;
	}

	finished = false;
	outputString = "";

	if (!isRichText) 
	{
		outputString = originalString.Mid(0, chars);
		return;
	}

	// If rich text is involved, we must display the text char by char, while correctly jumping over rich text tags, and closing any unclosed tags if we're only displaying half of a sentence.
	// Note: using inexisting tags in UE turns off Rich Text entirely for the string, so we will assume that all tags are real!
	// Therefore, don't use <AnySingleWord> in your text if it's not a valid tag!
	
	// This is a regex pattern for an opened tag, i.e. starts with '<', followed by a single word (only letters and numbers), ends with '>'
	const FRegexPattern openedTagPattern(TEXT("^(\\<[a-z,A-Z,0-9]+\\>)"));
	int openedTags = 0;
	int skippedSymbols = 0;

	/* Note to self: 
	* If UE4 starts supporting nested tags in rich text, simply remove the checks "openedTags == 1" and "openedTags == 0" in the code below.
	* Because nested tags aren't supported,t here's an edge-case where "<yellow><yellow>text</>" actually works in rich text, and the second "<yellow>" is displayed as text.
	* Currently, we deal with this case correctly. But if nested tags get supported, we'll lose support for this. Oh well!
	*/

	for (int32 i = 0; i < originalString.Len() && i < chars+1+skippedSymbols; i++)
	{
		//UE_LOG(LogTemp, Error, TEXT("Evaluating symbol: %s"), *originalString.Mid(i, 1));

		// If it's not a symbol of either opening or closing rich text tag, just append it.
		if (originalString[i] != TCHAR('<'))
		{
			outputString += originalString[i];
		}
		else // If we're possibly dealing with a tag.
		{			
			FString stringTail = originalString.Mid(i, originalString.Len() - i); // The rest of the string starting from i.

			// If we encountered a closing tag and one tag is currently open (UE4 doesn't support nested tags, hence the check).
			if (stringTail.StartsWith("</>") && openedTags == 1)
			{
				//UE_LOG(LogTemp, Error, TEXT("Found closing tag"));
				outputString += "</>";
				skippedSymbols += 3;
				i += 2;
				openedTags--;
			}
			else 
			{
				// Check if we're dealing with one word, followed by a closing brace. If not, just treat it as a non-tag-related symbol (i.e. just append it).
				FRegexMatcher tagMatcher(openedTagPattern, stringTail);
				if (tagMatcher.FindNext() && openedTags == 0) // Also, since no nested tags are allowed, check that no tags are open right now.
				{					
					//UE_LOG(LogTemp, Error, TEXT("Found opening tag: %s (length: %d)"), *stringTail.Mid(0, myMatcher.GetMatchEnding()), myMatcher.GetMatchEnding());
					int tagLength = tagMatcher.GetMatchEnding();
					outputString += stringTail.Mid(0, tagLength);
					skippedSymbols += tagLength;
					i += tagLength - 1;
					openedTags++;
				}
				// If the '<' symbol we encountered is not part of anything related to tags (or it's part of a nested tag, which we don't treat), just append it like any other symbol.
				else 
				{
					outputString += originalString[i];
				}
			}
		}
	}	
	for (int i = 0; i < openedTags; i++)
	{
		outputString += "</>";
	}
	if (chars + skippedSymbols == originalString.Len())
	{
		finished = true;
	}
}