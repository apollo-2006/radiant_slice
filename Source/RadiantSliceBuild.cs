using UnrealBuildTool;

public class RadiantSlice : ModuleRules
{
    public RadiantSlice(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Core Engine Dependencies
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",
            "NetCore" // Crucial for our Lag Compensation RPCs later
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
}