@echo off

%VULKAN_SDK%/Bin32/glslangValidator.exe -V surface.vert -o SPIR-V/surface_vert.spv
%VULKAN_SDK%/Bin32/glslangValidator.exe -V surface.frag -o SPIR-V/surface_frag.spv

%VULKAN_SDK%/Bin32/glslangValidator.exe -V default.vert -o SPIR-V/default_vert.spv
%VULKAN_SDK%/Bin32/glslangValidator.exe -V default.frag -o SPIR-V/default_frag.spv

pause