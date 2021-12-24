#pragma once

UENUM(Blueprintable,BlueprintType)
enum class ECharacterPosture : uint8
{
	Stand,
	Crouch,
	Sliding,
};

UENUM(Blueprintable,BlueprintType)
enum class ECharacterObstacleAction : uint8
{
	None,
	Vault,
	Clamb,
	LowClamb,
	Ladder,
	WallRun,
	Cliff,
	Push,
	Door,
	Hanging,
	Creeping,
	StandHiding,
	BalanceBeam,
	Crevice
};

UENUM(Blueprintable,BlueprintType)
enum class ECharacterWallRunSide : uint8
{
	Left,
	Right,
};

UENUM(Blueprintable,BlueprintType)
enum class ECharacterWallRunRelaseType : uint8
{
	None,
	Fall,
	Jumped,
	Land,
};

UENUM(Blueprintable,BlueprintType)
enum class ECharacterWallRunState : uint8
{
	Ready,
	Running,
	Release,
	CoolTime,
};