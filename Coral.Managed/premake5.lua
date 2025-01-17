project "Coral.Managed"
    language "C#"
    dotnetframework "net8.0"
    kind "SharedLib"
	clr "Unsafe"
	
    -- Don't specify architecture here. (see https://github.com/premake/premake-core/issues/1758)

		vsprops {
			AppendTargetFrameworkToOutputPath = "false",
			Nullable = "enable",
			CopyLocalLockFileAssemblies = "true",
			EnableDynamicLoading = "true",
		}

        disablewarnings {
            "CS8500"
        }

    files {
        "Source/**.cs"
    }

    
	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
