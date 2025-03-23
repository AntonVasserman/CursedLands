// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/HUD/CLAttributeProgressBar.h"

#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"
#include "Characters/CLPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

struct FOnAttributeChangeData;

void UCLAttributeProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	const ACLPlayerCharacter* PlayerCharacter = CastChecked<ACLPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UCLAbilitySystemComponent* PlayerCharacterAbilitySystemComponent = PlayerCharacter->GetCLAbilitySystemComponent();
	const UCLAttributeSet* AttributeSet = CastChecked<UCLAttributeSet>(PlayerCharacterAbilitySystemComponent->GetAttributeSet(UCLAttributeSet::StaticClass()));
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
	AttributeCurrentValueChanged(AttributeSet->GetAttributeByGameplayTag(CurrentValueAttributeGameplayTag).GetNumericValue(AttributeSet));
	AttributeMaxValueChanged(AttributeSet->GetAttributeByGameplayTag(MaxValueAttributeGameplayTag).GetNumericValue(AttributeSet));
}
