// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class zonbiTarget : TargetRules
{
	public zonbiTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "zonbi" } );
		ExtraModuleNames.AddRange(new string[] { "O369_Horror01" });

	}
}
