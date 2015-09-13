// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class BoardGame : ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }
 
	public BoardGame(TargetInfo Target)
	{
        PrivateDependencyModuleNames.AddRange(new string[] { "Engine", "Core"
        
        });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI" });

        bUseRTTI = true;
        // Neccessary configuration to make the UEB work with boost exceptions
        bEnableExceptions = true;
        UEBuildConfiguration.bForceEnableExceptions = true;

        //LoadMathFuncsLib(Target);
        LoadBGCoreLib(Target);
	}

    public bool LoadBGCoreLib(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "BGCoreLib", "Libraries");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "BGCoreLib." + PlatformString + ".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "BGCoreLib", "Includes"));

            // Include boost
            PrivateIncludePaths.Add(Path.Combine("C:", "Boost", "boost_1_58_0"));
        }

        Definitions.Add(string.Format("WITH_MATH_FUNCS_LIB_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
