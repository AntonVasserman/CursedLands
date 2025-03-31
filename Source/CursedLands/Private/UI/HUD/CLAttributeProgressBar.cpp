// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/HUD/CLAttributeProgressBar.h"

#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"
#include "AbilitySystem/Attributes/CLHealthAttributeSet.h"
#include "AbilitySystem/Attributes/CLManaAttributeSet.h"
#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"
#include "Characters/CLPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

struct FOnAttributeChangeData;

void UCLAttributeProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	// TODO (CL-58): Remove the usage of GameplayTag here and instead pass the attribute in an Init function of a sort...
	const ACLPlayerCharacter* PlayerCharacter = CastChecked<ACLPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UCLAbilitySystemComponent* PlayerCharacterAbilitySystemComponent = PlayerCharacter->GetCLAbilitySystemComponent();
	// NOTE: we have a constraint here where the CurrentValue and MaxValue attributes MUST be in the same AttributeSet
	const UCLAttributeSet* AttributeSet = GetAttributeSetByGameplayTag(PlayerCharacterAbilitySystemComponent, CurrentValueAttributeGameplayTag);
	checkf(AttributeSet, TEXT("No AttributeSet with attribute for tag: %s, was found in the PlayerCharacterAbilitySystemComponent"), *CurrentValueAttributeGameplayTag.ToString());
	const FGameplayAttribute& CurrentValueAttribute = AttributeSet->GetAttributeByGameplayTag(CurrentValueAttributeGameplayTag);
	const FGameplayAttribute& MaxValueAttribute = AttributeSet->GetAttributeByGameplayTag(MaxValueAttributeGameplayTag);

	// Bind to Value Change Delegate
	PlayerCharacterAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentValueAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			AttributeCurrentValueChanged(Data.NewValue);
		});

	PlayerCharacterAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxValueAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			AttributeMaxValueChanged(Data.NewValue);
		});

	// Broadcast Initial Values
	AttributeCurrentValueChanged(CurrentValueAttribute.GetNumericValue(AttributeSet));
	AttributeMaxValueChanged(MaxValueAttribute.GetNumericValue(AttributeSet));
}

const UCLAttributeSet* UCLAttributeProgressBar::GetAttributeSetByGameplayTag(const UCLAbilitySystemComponent* PlayerCharacterAbilitySystemComponent, const FGameplayTag& GameplayTag)
{
	if (const UCLHealthAttributeSet* HealthAttributeSet = CastChecked<UCLHealthAttributeSet>(PlayerCharacterAbilitySystemComponent->GetAttributeSet(UCLHealthAttributeSet::StaticClass()));
		HealthAttributeSet->GetAttributeByGameplayTag(GameplayTag) != nullptr)
	{
		return HealthAttributeSet;
	}

	if (const UCLManaAttributeSet* ManaAttributeSet = CastChecked<UCLManaAttributeSet>(PlayerCharacterAbilitySystemComponent->GetAttributeSet(UCLManaAttributeSet::StaticClass()));
		ManaAttributeSet->GetAttributeByGameplayTag(GameplayTag) != nullptr)
	{
		return ManaAttributeSet;
	}

	if (const UCLStaminaAttributeSet* StaminaAttributeSet = CastChecked<UCLStaminaAttributeSet>(PlayerCharacterAbilitySystemComponent->GetAttributeSet(UCLStaminaAttributeSet::StaticClass()));
		StaminaAttributeSet->GetAttributeByGameplayTag(GameplayTag) != nullptr)
	{
		return StaminaAttributeSet;
	}

	return nullptr;
}