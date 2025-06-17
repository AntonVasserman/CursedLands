// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/HUD/CLAttributeProgressBar.h"

#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CL_AttributeSet.h"
#include "AbilitySystem/Attributes/CL_HealthAttributeSet.h"
#include "AbilitySystem/Attributes/CL_ManaAttributeSet.h"
#include "AbilitySystem/Attributes/CL_StaminaAttributeSet.h"
#include "Characters/CLPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

struct FOnAttributeChangeData;

void UCLAttributeProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	// TODO (CL-58): Remove the usage of GameplayTag here and instead pass the attribute in an Init function of a sort...
	const ACLPlayerCharacter* PlayerCharacter = CastChecked<ACLPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UCL_AbilitySystemComponent* PlayerCharacterAbilitySystemComponent = PlayerCharacter->GetCLAbilitySystemComponent();
	// NOTE: we have a constraint here where the CurrentValue and MaxValue attributes MUST be in the same AttributeSet
	const UCL_AttributeSet* AttributeSet = GetAttributeSetByGameplayTag(PlayerCharacterAbilitySystemComponent, CurrentValueAttributeGameplayTag);
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

const UCL_AttributeSet* UCLAttributeProgressBar::GetAttributeSetByGameplayTag(const UCL_AbilitySystemComponent* PlayerCharacterAbilitySystemComponent, const FGameplayTag& GameplayTag)
{
	if (const UCL_HealthAttributeSet* HealthAttributeSet = CastChecked<UCL_HealthAttributeSet>(PlayerCharacterAbilitySystemComponent->GetAttributeSet(UCL_HealthAttributeSet::StaticClass()));
		HealthAttributeSet->GetAttributeByGameplayTag(GameplayTag) != nullptr)
	{
		return HealthAttributeSet;
	}

	if (const UCL_ManaAttributeSet* ManaAttributeSet = CastChecked<UCL_ManaAttributeSet>(PlayerCharacterAbilitySystemComponent->GetAttributeSet(UCL_ManaAttributeSet::StaticClass()));
		ManaAttributeSet->GetAttributeByGameplayTag(GameplayTag) != nullptr)
	{
		return ManaAttributeSet;
	}

	if (const UCL_StaminaAttributeSet* StaminaAttributeSet = CastChecked<UCL_StaminaAttributeSet>(PlayerCharacterAbilitySystemComponent->GetAttributeSet(UCL_StaminaAttributeSet::StaticClass()));
		StaminaAttributeSet->GetAttributeByGameplayTag(GameplayTag) != nullptr)
	{
		return StaminaAttributeSet;
	}

	return nullptr;
}