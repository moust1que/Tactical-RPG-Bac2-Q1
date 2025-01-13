#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.generated.h"

// Declaration des delegues pour notifier les changements de sante et d'XP
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPChanged, float, XP, float, XPForNextLevel);

// Classe de base pour un personnage dans le jeu
UCLASS()
class UNREALPROJECT_API ABaseCharacter : public APawn {
	GENERATED_BODY()

	public:
		// Constructeur par defaut
		ABaseCharacter();

		// Appele pour lier les actions d'input du joueur
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		// Fonction de gestion du tour du personnage
		UFUNCTION(BlueprintCallable, Category = "Turn") virtual void TakeTurn();
		
		// --------------- Proprietes de base ---------------
		
		// Indicateur si le personnage est un heros
		bool bIsHero;

		// Reference a la grille de jeu
		class AGrid* Grid;

		// Cellule actuelle du personnage sur la grille
		UPROPERTY(EditAnywhere) class AGridCell* CurCell;

		// Reference a l'instance d'animation du personnage
		UPROPERTY(BlueprintReadOnly) class UBaseCharacterAnimInstance* AnimInstance;

		// --------------- Statistiques du personnage ---------------

		// Proprietes de sante
		UPROPERTY(BlueprintReadOnly)int32 Health; // Sante actuelle
		UPROPERTY(BlueprintReadOnly)int32 MaxHealth; // Sante maximale
		int32 Damage; // Degâts infliges par l'attaque

		// --------------- XP et niveau ---------------

		// XP et niveau du personnage
		int32 XP = 0; // XP actuel
		int32 XPForNextLevel = 100; // XP pour le niveau suivant
		int32 CurLevel = 1; // Niveau actuel

		// --------------- Proprietes de deplacement et d'attaque ---------------

		// Proprietes liees au tour du personnage
		int32 TurnSpeed; // Vitesse du tour
		int32 TurnProgress; // Progres actuel dans le tour

		// Plage de deplacement et d'attaque
		int32 DisplacementRange; // Plage de deplacement maximale
		int32 AttackRange;  // Plage d'attaque maximale

		// Variables pour la gestion du deplacement
		int32 CurDisplacementUsed = 0; // Deplacement utilise jusqu'a present
		int32 remainingDisplacement = 0; // Deplacement restant

		// Indicateur permettant de skip le tour si l'IA n'attaque pas apres son deplacement
		bool bWillAttack = true;

		// Cible actuelle de l'attaque
		ABaseCharacter* TargetToAttack = nullptr;

		// Indicateur indiquant si le personnage a deja attaque
		bool bCanAttack = true;

		// --------------- Interface utilisateur ---------------

		// Portrait du personnage dans l'interface utilisateur
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI") UTexture2D* PortraitTexture;

		// --------------- Delegues pour la gestion de la sante et de l'XP ---------------

		// Delegues pour notifier les changements de sante et d'XP
		// Ces delegues sont utilises pour appeler des event permettant la mise a jour des barres de vie et d'xp
		UPROPERTY(BlueprintAssignable, Category = "Health")
		FOnHealthChanged OnHealthChanged;
		UPROPERTY(BlueprintAssignable, Category = "XP")
		FOnXPChanged OnXPChanged;

		// --------------- Fonctions principales ---------------

		// Fonction de reception des degâts
		void ReveiveDamage(int32 DamageReceived);

		// Fonction de deplacement suivant un chemin
		void MoveAlongPath(TArray<AGridCell*> path);

		// Fonction de gestion de la mort du personnage
		void OnDeath();

		// Fonction pour donner de l'XP
		virtual void GiveXP();

		// Reinitialiser les plages de deplacement et d'attaque
		void ResetRange();

		// Verifier si l'ennemi est toujours en vie
		void CheckIfEnemyStillAlive();

		// --------------- Variables de combat et de mouvement ---------------

		// Chemin actuel du personnage pour son deplacement
		TArray<AGridCell*> Path;

		// Fonction d'attaque sur un autre personnage
		void Attack(ABaseCharacter* Enemy);

		// Dernier attaquant du personnage
		ABaseCharacter* LastAttacker;

	protected:
		// Appele lors du debut du jeu ou de l'initialisation du personnage
		virtual void BeginPlay() override;

	private:
		// --------------- Variables privees ---------------

		// Temps ecoule depuis le dernier tick
		float ElapsedTime = 0.0f;

		// Indice de la cellule actuelle dans le chemin
		int32 CurrentCellIndex = 0;

		// Vitesse de deplacement du personnage
		float MoveSpeed = 200.0f;

		// Position de depart du personnage
		FVector StartPosition;

		// Position cible du personnage
		FVector TargetPosition;

		// --------------- Fonction pour le deplacement ---------------

		// Fonction pour deplacer le personnage vers la cellule suivante
		void MoveToCell(AGridCell* NextCell);

		// --------------- Fonction de mise a jour ---------------

		// Appele a chaque tick de jeu
		virtual void Tick(float DeltaTime) override;
};