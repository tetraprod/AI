using UnrealBuildTool;
public class WizardWarI : ModuleRules
{
    public WizardWarI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PhysicsCore", "OnlineSubsystem", "OnlineSubsystemUtils", "UMG" });
=======
=======

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PhysicsCore", "OnlineSubsystem", "OnlineSubsystemUtils", "UMG" });
=======

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PhysicsCore", "OnlineSubsystem", "OnlineSubsystemUtils", "UMG" });
=======

=======

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PhysicsCore", "OnlineSubsystem", "OnlineSubsystemUtils", "UMG" });
=======

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });


    }
}
