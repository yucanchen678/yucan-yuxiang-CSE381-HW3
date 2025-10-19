// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "ItemDefinition.h"
#include "SportsCar/WolfieCartSportsCar.h"
#include "ChaosWheeledVehicleMovementComponent.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create this pickup's mesh component
	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	check(PickupMeshComponent != nullptr);

	// Create this pickup's sphere component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	check(SphereComponent != nullptr);

	// Attach the sphere component to the mesh component
	SphereComponent->SetupAttachment(PickupMeshComponent);

	SphereComponent->SetSphereRadius(CollisionSphereRadius);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup();
	
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::InitializePickup()
{
	TObjectPtr<UDataTable> DataTable = PickupDataTable.LoadSynchronous();
	if (PickupDataTable && PickupItemIDs.Num() > 0)
	{

		// Randomly chose a pickup item from the list
		int RandInt = FMath::RandRange(0, PickupItemIDs.Num() - 1);
		FName PickupItemID = PickupItemIDs[RandInt];


		// Retrieve the item data associated with this pickup from the Data Table
		const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

		TObjectPtr<UItemDefinition> TempItemDefinition = ItemDataRow->ItemBase.Get();

		// Create a copy of the item with the class type
		ReferenceItem = TempItemDefinition->CreateItemCopy();

		if (TempItemDefinition->WorldMesh.IsValid())
		{
			PickupMeshComponent->SetStaticMesh(TempItemDefinition->WorldMesh.Get());
		}
		else
		{
			TObjectPtr<UStaticMesh> WorldMesh = TempItemDefinition->WorldMesh.LoadSynchronous();
			PickupMeshComponent->SetStaticMesh(WorldMesh);
		}

		// Set the mesh to visible and collidable
		PickupMeshComponent->SetVisibility(true);
		PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// Register the Overlap Event
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnSphereBeginOverlap);

	}
}

/**
*	Broadcasts an event when a character overlaps this pickup's SphereComponent. Sets the pickup to invisible and uninteractable, and respawns it after a set time.
*	@param OverlappedComponent - the component that was overlapped.
*	@param OtherActor - the Actor overlapping this component.
*	@param OtherComp - the Actor's component that overlapped this component.
*	@param OtherBodyIndex - the index of the overlapped component.
*	@param bFromSweep - whether the overlap was generated from a sweep.
*	@param SweepResult - contains info about the overlap such as surface normals and faces.
*/
void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// Checking if it's an BombardmentCharacter overlapping
	TObjectPtr<AWolfieCartSportsCar> Character = Cast<AWolfieCartSportsCar>(OtherActor);

	if (Character != nullptr)
	{

		if (ReferenceItem->ItemType == EItemType::SpeedUp) {

			Character->DoSpeedUp();
			
		}
		else if (ReferenceItem->ItemType == EItemType::SpeedDown) {
			
			Character->isSpeedDown = true;

			GetWorldTimerManager().SetTimer(Character->SpeedDownTimerHandle, Character.Get(), &AWolfieCartSportsCar::EndSpeedDown, Character->SpeedDownTime, false, -1);

		}

		// Unregister from the Overlap Event so it is no longer triggered
		SphereComponent->OnComponentBeginOverlap.RemoveAll(this);

		// Set this pickup to be invisible and disable collision
		PickupMeshComponent->SetVisibility(false);
		PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// If the pickup should respawn, wait an fRespawnTime amount of seconds before calling InitializePickup() to respawn it
		if (bShouldRespawn)
		{
			GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::InitializePickup, RespawnTime, false, -1);
		}
	}


}

/**
*	Updates this pickup whenever a property is changed.
*	@param PropertyChangedEvent - contains info about the property that was changed.
*/
void APickupBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Handle parent class property changes
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// If a property was changed, get the name of the changed property. Otherwise use none.
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Verify that the changed property exists in this class and that the PickupDataTable is valid.
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, TempPickupItemID) && PickupDataTable)
	{
		if (PostEditEnabled)
		{
			// Retrieve the associated ItemData for this pickup.
			if (const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(TempPickupItemID, TempPickupItemID.ToString()))
			{
				UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase;

				// Set the pickup's mesh to the associated item's mesh
				PickupMeshComponent->SetStaticMesh(TempItemDefinition->WorldMesh.Get());

				// Set the sphere's collision radius
				SphereComponent->SetSphereRadius(CollisionSphereRadius);
			}
		}
	}
	else if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, CollisionSphereRadius))
	{
		if (PostEditEnabled)
		{
			// Set the sphere's collision radius
			SphereComponent->SetSphereRadius(CollisionSphereRadius);
		}

	}
	else if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, PostEditEnabled))
	{
		if (PostEditEnabled && PickupDataTable)
		{
			// Retrieve the associated ItemData for this pickup.
			if (const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(TempPickupItemID, TempPickupItemID.ToString()))
			{
				UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase;

				// Set the pickup's mesh to the associated item's mesh
				PickupMeshComponent->SetStaticMesh(TempItemDefinition->WorldMesh.Get());

				// Set the sphere's collision radius
				SphereComponent->SetSphereRadius(CollisionSphereRadius);
			}
		}
		else
		{
			PickupMeshComponent->SetStaticMesh(nullptr);
		}

	}

}

bool APickupBase::IsVisible()
{
	return PickupMeshComponent->GetStaticMesh() != nullptr && PickupMeshComponent->IsVisible();
}